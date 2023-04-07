#pragma once
#include <vulkan/vulkan.h>

namespace vk {

extern VkBufferMemoryBarrier buffer_barrier_template;
extern VkCommandBufferBeginInfo command_buffer_begin_info_template;
extern VkCommandPoolCreateInfo command_pool_create_info_template;
extern VkSubmitInfo submit_info_template;
extern VkShaderModuleCreateInfo shader_module_create_info_template;
extern VkComputePipelineCreateInfo compute_pipeline_create_info_template;
extern VkPipelineShaderStageCreateInfo
    pipeline_shader_stage_create_info_template;
extern VkDescriptorSetLayoutCreateInfo descriptor_set_create_info_template;
extern VkPipelineLayoutCreateInfo pipeline_layout_create_info_template;
extern VkDescriptorPoolCreateInfo descriptor_pool_create_info_template;
extern VkDescriptorSetAllocateInfo descriptor_set_allocate_info_template;
extern VkWriteDescriptorSet write_descriptor_set_template;
extern VkSemaphoreCreateInfo semaphore_create_info_template;
extern VkSwapchainCreateInfoKHR swapchain_create_info_template;
extern VkRenderPassCreateInfo render_pass_create_info_template;
extern VkAttachmentDescription attachment_descriptor_template;
extern VkSubpassDescription subpass_description_template;
extern VkImageViewCreateInfo image_view_create_info_template;
extern VkImageSubresourceRange image_subresource_range_template;
extern VkFramebufferCreateInfo framebuffer_create_info_template;
extern VkPipelineVertexInputStateCreateInfo vertex_input_create_info_template;
extern VkPipelineInputAssemblyStateCreateInfo
    pipeline_input_assembly_create_info_template;
extern VkPipelineViewportStateCreateInfo pipeline_viewport_create_info_template;
extern VkPipelineRasterizationStateCreateInfo
    pipeline_rasterization_state_create_info_template;
extern VkPipelineMultisampleStateCreateInfo
    pipeline_multisample_state_create_info;

} // namespace vk
