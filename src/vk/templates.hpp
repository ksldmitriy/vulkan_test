#pragma once
#include <vulkan/vulkan.h>

namespace vk {

extern VkBufferMemoryBarrier buffer_barrier_template;
extern VkCommandBufferBeginInfo command_buffer_begin_info_template;
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
} // namespace vk
