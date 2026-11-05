/* SPDX-License-Identifier: MIT
 * CPU fallback: finish preceding work, invalidate/read a 32-bit predicate, then submit or skip
 * a continuation render. Initial render-pass clears are never put in that conditional job.
 * No dispatch support is claimed; general inherited/secondary/XFB and multi-attachment continuation
 * admission stays disabled. PVR_G5P_CONDITIONAL=probe is an explicit qualification switch.
 */
#include "pvr_g5p_conditional.h"
#include "pvr_entrypoints.h"
#include "pvr_physical_device.h"
#include "vk_command_pool.h"
#include "pvr_buffer.h"
#include "pvr_cmd_buffer.h"
#include "pvr_device.h"
#include "pvr_g5p.h"
#include "pvr_queue.h"
#include "pvr_winsys.h"
#include "winsys/apple_g5p/pvr_g5p_winsys.h"
#include "util/os_time.h"
#include "vk_alloc.h"
#include "vk_log.h"
#include "vk_sync.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pvr_g5p_conditional_state {
   struct pvr_buffer *buffer;
   VkDeviceSize offset;
   bool active, inverted;
};

bool pvr_g5p_conditional_enabled(void)
{
   const char *v = getenv("PVR_G5P_CONDITIONAL");
   return v && !strcmp(v, "probe");
}

static VkResult split(struct pvr_cmd_buffer *cmd)
{
   return pvr_cmd_buffer_insert_mid_frag_barrier_event(cmd, PVR_PIPELINE_STAGE_ALL_BITS,
                                                      PVR_PIPELINE_STAGE_ALL_BITS);
}

VKAPI_ATTR void VKAPI_CALL
PVR_PER_ARCH(CmdBeginConditionalRenderingEXT)(VkCommandBuffer commandBuffer,
                                    const VkConditionalRenderingBeginInfoEXT *info)
{
   VK_FROM_HANDLE(pvr_cmd_buffer, cmd, commandBuffer);
   VK_FROM_HANDLE(pvr_buffer, buffer, info->buffer);
   VkResult result = VK_SUCCESS;
   struct pvr_g5p_conditional_state *s = cmd->state.g5p_conditional;
   if (!PVR_HAS_FEATURE(&cmd->device->pdevice->dev_info, apple_g5p) ||
       !pvr_g5p_conditional_enabled() || cmd->vk.level != VK_COMMAND_BUFFER_LEVEL_PRIMARY ||
       (s && s->active) || info->offset % 4 || info->offset > buffer->vk.size ||
       buffer->vk.size - info->offset < 4) {
      vk_command_buffer_set_error(&cmd->vk, vk_errorf(cmd, VK_ERROR_FEATURE_NOT_PRESENT,
                                  "G5P conditional qualification: primary, bounded aligned predicate only"));
      return;
   }
   if (!s) {
      s = vk_zalloc(&cmd->vk.pool->alloc, sizeof(*s), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      if (!s) { vk_command_buffer_set_error(&cmd->vk, VK_ERROR_OUT_OF_HOST_MEMORY); return; }
      cmd->state.g5p_conditional = s;
   }
   /* A begin inside a render first completes the unconditional loadOp/earlier draws. */
   if (cmd->state.current_sub_cmd && cmd->state.current_sub_cmd->type == PVR_SUB_CMD_TYPE_GRAPHICS)
      result = split(cmd);
   if (result != VK_SUCCESS) { vk_command_buffer_set_error(&cmd->vk, result); return; }
   *s = (struct pvr_g5p_conditional_state){buffer, info->offset, true,
             !!(info->flags & VK_CONDITIONAL_RENDERING_INVERTED_BIT_EXT)};
}

VKAPI_ATTR void VKAPI_CALL
PVR_PER_ARCH(CmdEndConditionalRenderingEXT)(VkCommandBuffer commandBuffer)
{
   VK_FROM_HANDLE(pvr_cmd_buffer, cmd, commandBuffer);
   struct pvr_g5p_conditional_state *s = cmd->state.g5p_conditional;
   if (!s || !s->active) { vk_command_buffer_set_error(&cmd->vk, VK_ERROR_UNKNOWN); return; }
   if (cmd->state.current_sub_cmd && cmd->state.current_sub_cmd->type == PVR_SUB_CMD_TYPE_GRAPHICS &&
       cmd->state.current_sub_cmd->gfx.job.g5p.conditional_buffer) {
      VkResult result = split(cmd);
      if (result != VK_SUCCESS) { vk_command_buffer_set_error(&cmd->vk, result); return; }
   }
   s->active = false;
}

VkResult pvr_g5p_conditional_draw(struct pvr_cmd_buffer *cmd)
{
   struct pvr_g5p_conditional_state *s = cmd->state.g5p_conditional;
   if (!s || !s->active) return VK_SUCCESS;
   if (cmd->state.g5p_xfb)
      return vk_errorf(cmd, VK_ERROR_FEATURE_NOT_PRESENT, "G5P conditional XFB is not qualified");
   assert(cmd->state.current_sub_cmd &&
          cmd->state.current_sub_cmd->type == PVR_SUB_CMD_TYPE_GRAPHICS);
   struct pvr_sub_cmd_gfx *sub = &cmd->state.current_sub_cmd->gfx;
   if (!sub->job.g5p.conditional_buffer) {
      /* A begin outside the render must not make its initial loadOp conditional. */
      if (!sub->barrier_load) {
         VkResult result = split(cmd);
         if (result != VK_SUCCESS) return result;
         sub = &cmd->state.current_sub_cmd->gfx;
      }
      sub->job.g5p.conditional_buffer = s->buffer;
      sub->job.g5p.conditional_offset = s->offset;
      sub->job.g5p.conditional_inverted = s->inverted;
   }
   return VK_SUCCESS;
}

VkResult pvr_g5p_conditional_test(struct pvr_device *device, struct pvr_queue *queue,
                                const struct pvr_sub_cmd_gfx *sub, bool *run)
{
   *run = true;
   if (!sub->job.g5p.conditional_buffer) return VK_SUCCESS;
   const int64_t deadline = os_time_get_absolute_timeout(2000000000ull);
   for (unsigned i = 0; i < PVR_JOB_TYPE_MAX; i++) {
      struct vk_sync *syncs[] = {queue->last_job_signal_sync[i], queue->next_job_wait_sync[i]};
      for (unsigned j = 0; j < 2; j++)
         if (syncs[j] && vk_sync_wait(&device->vk, syncs[j], 0, VK_SYNC_WAIT_COMPLETE, deadline) != VK_SUCCESS)
            return vk_errorf(device, VK_ERROR_DEVICE_LOST, "G5P predicate dependency did not complete");
   }
   const struct pvr_buffer *b = sub->job.g5p.conditional_buffer;
   uint32_t value;
   if (!b->vma || !b->vma->bo)
      return vk_errorf(device, VK_ERROR_DEVICE_LOST, "G5P predicate buffer is not bound");
   VkResult result = pvr_g5p_winsys_read_u32(b->vma->bo,
                        b->vma->bo_offset + sub->job.g5p.conditional_offset, &value);
   if (result != VK_SUCCESS) return result;
   *run = (value != 0) != sub->job.g5p.conditional_inverted;
   if (getenv("PVR_G5P_CONDITIONAL_LOG"))
      fprintf(stderr, "g5p predicate value=%u inverted=%u run=%u\n", value,
              sub->job.g5p.conditional_inverted, *run);
   return VK_SUCCESS;
}

void pvr_g5p_conditional_free(struct pvr_cmd_buffer *cmd)
{
   vk_free(&cmd->vk.pool->alloc, cmd->state.g5p_conditional);
   cmd->state.g5p_conditional = NULL;
}
