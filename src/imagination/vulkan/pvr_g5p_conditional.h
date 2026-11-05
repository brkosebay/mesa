/* SPDX-License-Identifier: MIT */
/* CPU predicate resolution at a split render boundary; qualification remains opt-in. */
#ifndef PVR_G5P_CONDITIONAL_H
#define PVR_G5P_CONDITIONAL_H
#include <stdbool.h>
#include <vulkan/vulkan_core.h>
struct pvr_cmd_buffer;
struct pvr_device;
struct pvr_queue;
struct pvr_sub_cmd_gfx;
bool pvr_g5p_conditional_enabled(void);
VkResult pvr_g5p_conditional_draw(struct pvr_cmd_buffer *cmd);
VkResult pvr_g5p_conditional_test(struct pvr_device *device, struct pvr_queue *queue,
                                const struct pvr_sub_cmd_gfx *sub, bool *run);
void pvr_g5p_conditional_free(struct pvr_cmd_buffer *cmd);
#endif
