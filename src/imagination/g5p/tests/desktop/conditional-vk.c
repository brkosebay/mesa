/* SPDX-License-Identifier: MIT
 * Generated-by: LLM
 * Reuse the merged driver's format-test resource and pipeline helpers. Every
 * conditional result is checked independently; record-only never certifies pixels.
 */
#define main formats_suite_main
#include "formats.c"
#undef main

static PFN_vkCmdBeginConditionalRenderingEXT cond_begin;
static PFN_vkCmdEndConditionalRenderingEXT cond_end;
static bool record_only;

static int submit_keep(VkCommandBuffer cmd)
{
   const VkFenceCreateInfo fi = { .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
   const VkSubmitInfo si = { .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                            .commandBufferCount = 1, .pCommandBuffers = &cmd };
   VkFence fence;
   CHECK(vkCreateFence(dev, &fi, NULL, &fence));
   CHECK(vkQueueSubmit(queue, 1, &si, fence));
   CHECK(vkWaitForFences(dev, 1, &fence, VK_TRUE, 5000000000ull));
   vkDestroyFence(dev, fence, NULL);
   return 0;
}

static int check_pixels(const char *name, const struct buf *rb, bool drawn, bool halves)
{
   /* BGRA bytes: solid.frag, solid-b.frag, initial blue clear. */
   const uint8_t red[4] = {32, 48, 239, 64}, purple[4] = {191, 48, 239, 64};
   const uint8_t blue[4] = {255, 0, 0, 255};
   if (record_only) {
      printf("CONDITIONAL %s RECORDED pixels-unjudged\n", name);
      return 0;
   }
   unsigned bad = 0;
   for (unsigned y = 0; y < H; y++) {
      for (unsigned x = 0; x < W; x++) {
         const uint8_t *want = halves ? (x < W/2 ? red : purple) : drawn ? red : blue;
         const uint8_t *got = rb->map + 4 * (y * W + x);
         if (memcmp(got, want, 4) && !bad++)
            printf("CONDITIONAL %s mismatch x=%u y=%u want=%u,%u,%u,%u got=%u,%u,%u,%u\n",
                   name, x, y, want[0], want[1], want[2], want[3], got[0], got[1], got[2], got[3]);
      }
   }
   printf("CONDITIONAL %s %s pixels=%u bad=%u\n", name, bad ? "FAIL" : "PASS", W*H, bad);
   return bad != 0;
}

static int run_case(const char *name, uint32_t value, bool inverted, bool outside,
                    bool gpu_write, bool halves, bool resubmit, bool clear_command)
{
   cur = name;
   struct img target;
   struct buf predicate, full, left, right;
   if (make_image(&target, VK_FORMAT_B8G8R8A8_UNORM, W, H, 1,
                  VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT) ||
       make_buffer(&predicate, 16, VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT |
                                  VK_BUFFER_USAGE_TRANSFER_DST_BIT) ||
       quad(&full, 1, 0, 1, 0, 1) || quad(&left, 0, 0, 1, 0, 1) ||
       quad(&right, 1, 0, 1, 0, 1)) return 1;
   /* Translate the right quad's left edge to NDC zero. */
   ((float *)right.map)[0] = ((float *)right.map)[8] = 0;
   if (sync_mem(right.mem, false)) return 1;
   const uint32_t words[4] = {0xfeed1234, gpu_write ? !value : value, 0xcafe5678, 0xf00d9876};
   memcpy(predicate.map, words, sizeof(words));
   if (sync_mem(predicate.mem, false)) return 1;
   VkRenderPass rp;
   VkFramebuffer fb;
   if (make_pass(VK_FORMAT_B8G8R8A8_UNORM, target.view, VK_ATTACHMENT_LOAD_OP_CLEAR, &rp, &fb)) return 1;
   const VkPipelineLayoutCreateInfo li = { .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
   VkPipelineLayout layout;
   CHECK(vkCreatePipelineLayout(dev, &li, NULL, &layout));
   VkPipeline red, purple;
   if (make_pipeline(rp, layout, spv_solid_frag, sizeof(spv_solid_frag), false, &red) ||
       make_pipeline(rp, layout, spv_solid_b_frag, sizeof(spv_solid_b_frag), false, &purple)) return 1;
   VkCommandBuffer cmd;
   if (begin(&cmd)) return 1;
   if (gpu_write) {
      vkCmdFillBuffer(cmd, predicate.b, 4, 4, value);
      const VkBufferMemoryBarrier b = { .sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
         .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
         .dstAccessMask = VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT,
         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
         .buffer = predicate.b, .offset = 4, .size = 4 };
      vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT,
                           VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT, 0, 0, NULL, 1, &b, 0, NULL);
   }
   const VkConditionalRenderingBeginInfoEXT ci = {
      .sType = VK_STRUCTURE_TYPE_CONDITIONAL_RENDERING_BEGIN_INFO_EXT, .buffer = predicate.b, .offset = 4,
      .flags = inverted ? VK_CONDITIONAL_RENDERING_INVERTED_BIT_EXT : 0 };
   const VkClearValue clear = { .color.float32 = {0, 0, 1, 1} };
   const VkRenderPassBeginInfo bi = { .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
      .renderPass = rp, .framebuffer = fb, .renderArea = {{0, 0}, {W, H}},
      .clearValueCount = 1, .pClearValues = &clear };
   const VkDeviceSize zero = 0;
   if (outside) cond_begin(cmd, &ci);
   vkCmdBeginRenderPass(cmd, &bi, VK_SUBPASS_CONTENTS_INLINE);
   vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, red);
   if (halves) {
      vkCmdBindVertexBuffers(cmd, 0, 1, &left.b, &zero);
      vkCmdDraw(cmd, 4, 1, 0, 0);
   }
   if (!outside) cond_begin(cmd, &ci);
   vkCmdBindVertexBuffers(cmd, 0, 1, &full.b, &zero);
   if (clear_command) {
      const VkClearAttachment ca = { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .colorAttachment = 0,
         .clearValue.color.float32 = {0.9375f, 0.1875f, 0.125f, 0.25f} };
      const VkClearRect cr = { .rect = {{0,0},{W,H}}, .layerCount = 1 };
      vkCmdClearAttachments(cmd, 1, &ca, 1, &cr);
   } else {
      vkCmdDraw(cmd, 4, 1, 0, 0);
   }
   if (!outside) cond_end(cmd);
   if (halves) {
      vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, purple);
      vkCmdBindVertexBuffers(cmd, 0, 1, &right.b, &zero);
      vkCmdDraw(cmd, 4, 1, 0, 0);
   }
   vkCmdEndRenderPass(cmd);
   if (outside) cond_end(cmd);
   CHECK(vkEndCommandBuffer(cmd));
   int failed = 0;
   const unsigned rounds = resubmit ? 3 : 1;
   for (unsigned i = 0; i < rounds; i++) {
      uint32_t actual = resubmit ? (i == 1 ? 7 : 0) : value;
      if (resubmit) {
         memcpy(predicate.map + 4, &actual, 4);
         if (sync_mem(predicate.mem, false)) return 1;
      }
      struct buf rb;
      if (submit_keep(cmd) || read_target(target.image, VK_FORMAT_B8G8R8A8_UNORM, 4, &rb)) return 1;
      char label[128];
      snprintf(label, sizeof(label), "%s-%u", name, i);
      failed |= check_pixels(label, &rb, (actual != 0) != inverted, halves);
      free_buffer(&rb);
      if (sync_mem(predicate.mem, true)) return 1;
      if (memcmp(predicate.map, words, 4) || memcmp(predicate.map + 8, words + 2, 8)) {
         printf("CONDITIONAL %s FAIL predicate guards\n", label); failed = 1;
      }
   }
   vkFreeCommandBuffers(dev, pool, 1, &cmd);
   vkDestroyPipeline(dev, purple, NULL); vkDestroyPipeline(dev, red, NULL);
   vkDestroyPipelineLayout(dev, layout, NULL);
   vkDestroyFramebuffer(dev, fb, NULL); vkDestroyRenderPass(dev, rp, NULL);
   free_buffer(&right); free_buffer(&left); free_buffer(&full); free_buffer(&predicate); free_image(&target);
   return failed;
}

int main(int argc, char **argv)
{
   setvbuf(stdout, NULL, _IOLBF, 0);
   if (argc == 2 && !strcmp(argv[1], "--record-only")) record_only = true;
   else if (argc != 1) { fprintf(stderr, "usage: %s [--record-only]\n", argv[0]); return 2; }
   const VkApplicationInfo app = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = "g5p-conditional", .apiVersion = VK_API_VERSION_1_1};
   const VkInstanceCreateInfo ii = {.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, .pApplicationInfo = &app};
   VkInstance instance;
   CHECK(vkCreateInstance(&ii, NULL, &instance));
   uint32_t n = 1;
   CHECK(vkEnumeratePhysicalDevices(instance, &n, &pd));
   if (!n) { puts("CONDITIONAL RESULT FAIL no-device"); return 1; }
   VkPhysicalDeviceProperties props;
   vkGetPhysicalDeviceProperties(pd, &props);
   printf("CONDITIONAL device=\"%s\"\n", props.deviceName);
   VkPhysicalDeviceConditionalRenderingFeaturesEXT features = {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT};
   VkPhysicalDeviceFeatures2 f = {.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, .pNext = &features};
   vkGetPhysicalDeviceFeatures2(pd, &f);
   if (!features.conditionalRendering) { puts("CONDITIONAL RESULT FAIL feature-missing"); return 1; }
   uint32_t nq = 8;
   VkQueueFamilyProperties families[8];
   vkGetPhysicalDeviceQueueFamilyProperties(pd, &nq, families);
   queue_family = UINT32_MAX;
   for (uint32_t i = 0; i < nq; i++) if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {queue_family = i; break;}
   if (queue_family == UINT32_MAX) return 1;
   const float priority = 1;
   const VkDeviceQueueCreateInfo qi = {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueFamilyIndex = queue_family, .queueCount = 1, .pQueuePriorities = &priority};
   const char *extensions[] = {VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME};
   features.inheritedConditionalRendering = false;
   const VkDeviceCreateInfo di = {.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, .pNext = &features,
      .queueCreateInfoCount = 1, .pQueueCreateInfos = &qi, .enabledExtensionCount = 1, .ppEnabledExtensionNames = extensions};
   CHECK(vkCreateDevice(pd, &di, NULL, &dev));
   vkGetDeviceQueue(dev, queue_family, 0, &queue);
   cond_begin = (PFN_vkCmdBeginConditionalRenderingEXT)vkGetDeviceProcAddr(dev, "vkCmdBeginConditionalRenderingEXT");
   cond_end = (PFN_vkCmdEndConditionalRenderingEXT)vkGetDeviceProcAddr(dev, "vkCmdEndConditionalRenderingEXT");
   if (!cond_begin || !cond_end) {puts("CONDITIONAL RESULT FAIL null-entrypoint"); return 1;}
   const VkCommandPoolCreateInfo pi = {.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .queueFamilyIndex = queue_family};
   CHECK(vkCreateCommandPool(dev, &pi, NULL, &pool));
   struct test {const char *name; uint32_t v; bool inv, outside, gpu, halves, replay, clear_command;} tests[] = {
      {"clear-attachments-zero",0,0,0,0,0,0,1}, {"clear-attachments-nonzero",7,0,0,0,0,0,1},
      {"zero-clear-unconditional",0,0,0,0,0,0,0}, {"nonzero",0x80000000,0,0,0,0,0,0},
      {"zero-inverted",0,1,0,0,0,0,0}, {"nonzero-inverted",7,1,0,0,0,0,0},
      {"outside-zero-clear-unconditional",0,0,1,0,0,0,0}, {"outside-nonzero",7,0,1,0,0,0,0},
      {"gpu-write-zero",0,0,0,1,0,0,0}, {"gpu-write-nonzero",7,0,0,1,0,0,0},
      {"continuation-keeps-before-and-after",0,0,0,0,1,0,0}, {"resubmit-current-predicate",0,0,0,0,0,1,0},
   };
   unsigned pass = 0;
   for (unsigned i = 0; i < ARRAY_LEN(tests); i++) {
      const struct test *t = &tests[i];
      if (!run_case(t->name,t->v,t->inv,t->outside,t->gpu,t->halves,t->replay,t->clear_command)) pass++;
   }
   vkDestroyCommandPool(dev,pool,NULL); vkDestroyDevice(dev,NULL); vkDestroyInstance(instance,NULL);
   printf("CONDITIONAL RESULT %s %u of %zu (14 image comparisons; record-only=%u)\n",
          pass != ARRAY_LEN(tests) ? "FAIL" : record_only ? "RECORDED" : "PASS",pass,ARRAY_LEN(tests),record_only);
   return pass != ARRAY_LEN(tests);
}
