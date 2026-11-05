/* SPDX-License-Identifier: MIT
 * Generated-by: LLM. Negative creation test; no native GS packet or submission. */
#define main formats_suite_main
#include "formats.c"
#undef main
#include "depth-spirv.h"
#include "geometry-spirv.h"
static int
make_refusal_pipeline(VkRenderPass rp, VkPipelineLayout layout, const uint32_t *fs, size_t fs_size,
              VkPipeline *pipe)
{
   VkShaderModuleCreateInfo smi = { .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                                    .codeSize = sizeof(spv_depth_vert), .pCode = spv_depth_vert };
   VkShaderModule vsm, fsm, gsm;
   CHECK(vkCreateShaderModule(dev, &smi, NULL, &vsm));
   smi.codeSize = fs_size;
   smi.pCode = fs;
   CHECK(vkCreateShaderModule(dev, &smi, NULL, &fsm));
   smi.codeSize=sizeof(spv_geometry);smi.pCode=spv_geometry;
   CHECK(vkCreateShaderModule(dev,&smi,NULL,&gsm));
   const VkPipelineShaderStageCreateInfo stages[3] = {
      { .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vsm, .pName = "main" },
      { .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, .stage = VK_SHADER_STAGE_GEOMETRY_BIT,
        .module = gsm, .pName = "main" },
      { .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = fsm, .pName = "main" },
   };
   const VkVertexInputBindingDescription vib = { 0, 16, VK_VERTEX_INPUT_RATE_VERTEX };
   const VkVertexInputAttributeDescription attr[1] = { { 0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, 0 } };
   const VkPipelineVertexInputStateCreateInfo vi = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO, .vertexBindingDescriptionCount = 1,
      .pVertexBindingDescriptions = &vib, .vertexAttributeDescriptionCount = 1, .pVertexAttributeDescriptions = attr };
   const VkPipelineInputAssemblyStateCreateInfo ia = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
      .topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST };
   const VkViewport vpt = { 0, 0, W, H, 0, 1 };
   const VkRect2D sc = { { 0, 0 }, { W, H } };
   const VkPipelineViewportStateCreateInfo vps = { .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                                                   .viewportCount = 1, .pViewports = &vpt, .scissorCount = 1,
                                                   .pScissors = &sc };
   const VkPipelineRasterizationStateCreateInfo rs = {

      .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, .polygonMode = VK_POLYGON_MODE_FILL,
      .cullMode = VK_CULL_MODE_NONE, .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE, .lineWidth = 1.0f };
   const VkPipelineMultisampleStateCreateInfo ms = { .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                                                     .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT };
   const VkPipelineColorBlendAttachmentState cba = {
      .blendEnable = false, .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, .colorBlendOp = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, .dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      .alphaBlendOp = VK_BLEND_OP_ADD, .colorWriteMask = 0xf };
   const VkPipelineColorBlendStateCreateInfo cb = { .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                                                    .attachmentCount = 1, .pAttachments = &cba };
   const VkGraphicsPipelineCreateInfo gci = {
      .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, .stageCount = 3, .pStages = stages,
      .pVertexInputState = &vi, .pInputAssemblyState = &ia, .pViewportState = &vps, .pRasterizationState = &rs,
      .pMultisampleState = &ms, .pColorBlendState = &cb, .layout = layout, .renderPass = rp };
   VkResult result=vkCreateGraphicsPipelines(dev, VK_NULL_HANDLE, 1, &gci, NULL, pipe);
   vkDestroyShaderModule(dev, vsm, NULL);
   vkDestroyShaderModule(dev, fsm, NULL);
   vkDestroyShaderModule(dev,gsm,NULL);
   return result;
}

int main(void) {
 setvbuf(stdout,NULL,_IOLBF,0);
 const VkApplicationInfo ai={.sType=VK_STRUCTURE_TYPE_APPLICATION_INFO,.apiVersion=VK_API_VERSION_1_1};
 const VkInstanceCreateInfo ii={.sType=VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,.pApplicationInfo=&ai};VkInstance instance;
 CHECK(vkCreateInstance(&ii,NULL,&instance));uint32_t count=1;CHECK(vkEnumeratePhysicalDevices(instance,&count,&pd));if(!count)return 1;
 VkPhysicalDeviceProperties props;vkGetPhysicalDeviceProperties(pd,&props);VkPhysicalDeviceFeatures features;vkGetPhysicalDeviceFeatures(pd,&features);
 printf("GS-REFUSAL device=\"%s\" geometryShader=%u default-caps no-overrides\n",props.deviceName,features.geometryShader);
 if(features.geometryShader){puts("GS-REFUSAL RESULT FAIL unimplemented geometryShader advertised");return 1;}
 VkQueueFamilyProperties families[8];count=8;vkGetPhysicalDeviceQueueFamilyProperties(pd,&count,families);
 for(queue_family=0;queue_family<count;queue_family++) {
  if(families[queue_family].queueFlags&VK_QUEUE_GRAPHICS_BIT)break;
 }
 if(queue_family==count)return 1;
 const float priority=1;const VkDeviceQueueCreateInfo qi={.sType=VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,.queueFamilyIndex=queue_family,.queueCount=1,.pQueuePriorities=&priority};
 const VkDeviceCreateInfo di={.sType=VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,.queueCreateInfoCount=1,.pQueueCreateInfos=&qi};
 CHECK(vkCreateDevice(pd,&di,NULL,&dev));
 struct img target;VkRenderPass rp;VkFramebuffer fb;VkPipelineLayout layout;VkPipeline pipe=VK_NULL_HANDLE;
 if(make_image(&target,VK_FORMAT_B8G8R8A8_UNORM,W,H,1,VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)||make_pass(VK_FORMAT_B8G8R8A8_UNORM,target.view,VK_ATTACHMENT_LOAD_OP_CLEAR,&rp,&fb))return 1;
 const VkPipelineLayoutCreateInfo li={.sType=VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};CHECK(vkCreatePipelineLayout(dev,&li,NULL,&layout));
 /* Intentionally request the unsupported stage with its feature disabled.
  * Driver robustness test, not a valid-feature or rendered-pixel claim. */
 VkResult result=make_refusal_pipeline(rp,layout,spv_solid_frag,sizeof(spv_solid_frag),&pipe);
 printf("GS-REFUSAL pipeline-result=%d submitted=0 handle=%s\n",result,pipe==VK_NULL_HANDLE?"null":"non-null");
 int failed=result!=VK_ERROR_FEATURE_NOT_PRESENT||pipe!=VK_NULL_HANDLE;
 if(pipe)vkDestroyPipeline(dev,pipe,NULL);
 vkDestroyPipelineLayout(dev,layout,NULL);vkDestroyFramebuffer(dev,fb,NULL);vkDestroyRenderPass(dev,rp,NULL);free_image(&target);
 vkDestroyDevice(dev,NULL);vkDestroyInstance(instance,NULL);
 printf("GS-REFUSAL RESULT %s geometryShader=0 pipeline-rejected submitted=0\n",failed?"FAIL":"PASS");return failed;
}
