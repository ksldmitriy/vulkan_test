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
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext = nullptr,
    .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT};

VkPresentInfoKHR present_info_template = {
    .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR, .pNext = nullptr};

VkRenderPassBeginInfo render_pass_begin_info_template = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO, .pNext = nullptr};

VkImageMemoryBarrier image_memory_barrier_template = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER, .pNext = 0};

VkCommandPoolCreateInfo command_pool_create_info_template{
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = nullptr,
    .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT};

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
};

VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0};

VkPipelineLayoutCreateInfo pipeline_layout_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .pushConstantRangeCount = 0};

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
    .descriptorCount = 1};

VkSemaphoreCreateInfo semaphore_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0};

VkSwapchainCreateInfoKHR swapchain_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = nullptr,
    .flags = 0,
    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = nullptr};

VkRenderPassCreateInfo render_pass_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0};

VkAttachmentDescription attachment_descriptor_template = {
    .flags = 0,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE};

VkSubpassDescription subpass_description_template = {
    .flags = 0,
    .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
    .inputAttachmentCount = 0,
    .pInputAttachments = nullptr,
    .colorAttachmentCount = 0,
    .pColorAttachments = nullptr,
    .pResolveAttachments = nullptr,
    .pDepthStencilAttachment = nullptr,
    .preserveAttachmentCount = 0,
    .pPreserveAttachments = nullptr};

VkImageViewCreateInfo image_view_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0};

VkImageSubresourceRange image_subresource_range_template = {
    .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1};

VkFramebufferCreateInfo framebuffer_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0};

VkPipelineVertexInputStateCreateInfo vertex_input_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0};

VkPipelineInputAssemblyStateCreateInfo
    pipeline_input_assembly_create_info_template = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0};

VkPipelineViewportStateCreateInfo pipeline_viewport_create_info_template{
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0};

VkPipelineRasterizationStateCreateInfo
    pipeline_rasterization_state_create_info_template = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0,
        .depthBiasClamp = 0,
        .depthBiasSlopeFactor = 0};

VkPipelineMultisampleStateCreateInfo
    pipeline_multisample_state_create_info_template = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 1,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE};

VkPipelineColorBlendStateCreateInfo
    pipeline_color_blend_state_create_info_template = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .blendConstants = {1, 1, 1, 1}};

VkGraphicsPipelineCreateInfo graphics_pipeline_create_info_template = {
    .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0};

VkCommandBufferAllocateInfo command_buffer_allocate_info_template = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, .pNext = nullptr};

} // namespace vk
