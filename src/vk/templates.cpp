#include "templates.hpp"

namespace vk {

VkBufferMemoryBarrier buffer_barrier_template = {
    .sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
    .pNext = nullptr,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .offset = 0,
    .size = VK_WHOLE_SIZE};

VkCommandBufferBeginInfo command_buffer_begin_info_template = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .pNext = nullptr};

VkSubmitInfo submit_info_template = {.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                                     .pNext = nullptr,
                                     .waitSemaphoreCount = 0,
                                     .signalSemaphoreCount = 0};

VkShaderModuleCreateInfo shader_module_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
};

VkComputePipelineCreateInfo compute_pipeline_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
};

VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .stage = VK_SHADER_STAGE_COMPUTE_BIT};

VkDescriptorSetLayoutCreateInfo descriptor_set_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0};

VkPipelineLayoutCreateInfo pipeline_layout_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0};

VkDescriptorPoolCreateInfo descriptor_pool_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    .pNext = nullptr,
    .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT};

VkDescriptorSetAllocateInfo descriptor_set_allocate_info_template = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, .pNext = nullptr};

VkWriteDescriptorSet write_descriptor_set_template = {
    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    .pNext = nullptr,
    .dstArrayElement = 0,
    .descriptorCount = 1

};

} // namespace vk
