#include "graphics_vulkan_application.hpp"

void GraphicsVulkanApplication::Run() {
  CreateWindow();
  Prepare();
  RunRenderLoop();
}

void WindowResizeCallback(GLFWwindow *window, int w, int h) {
  cout << "window resized " << w << ", " << h << endl;
}

void GlfwErrorCallback(int error_code, const char *description) {
  cout << "glfw error: " << description << endl;
}

void GraphicsVulkanApplication::CreateWindow() {
  if (!glfwInit()) {
    throw VulkanException("cant init glfw");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  window = glfwCreateWindow(900, 900, "hui", nullptr, nullptr);
  if (!window) {
    throw VulkanException("glfw cant create window");
  }

  glfwSetWindowSizeCallback(window, WindowResizeCallback);
}



void GraphicsVulkanApplication::Prepare() {
  InitVulkan();
  CreateWindowSurface();
  // TODO: Check swapchain support
  CreateCommandPool();
  CreateSemaphores();
  CreateVertexBuffer();
  CreateUniformBuffer();
  CreateSwapchain();
  CreateImagesView();
  CreateRenderPass();
  CreateFramebuffer();
  CreateGraphicsPipeline();
  CreateDescriptorPool();
  CreateDescriptorSet();
  CreateCommandBuffer();
}
void GraphicsVulkanApplication::CreateDescriptorPool() {
  VkDescriptorPoolSize pool_size;
  pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  pool_size.descriptorCount = 1;

  VkDescriptorPoolCreateInfo create_info =
      vk::descriptor_pool_create_info_template;
  create_info.poolSizeCount = 1;
  create_info.pPoolSizes = &pool_size;
  create_info.maxSets = 1;

  VkResult result = vkCreateDescriptorPool(device->GetHandle(), &create_info,
                                           nullptr, &descriptor_pool);
  if (result) {
    throw VulkanException("cant create descriptor pool");
  }
}

void GraphicsVulkanApplication::CreateDescriptorSet() {
  VkDescriptorSetAllocateInfo allocate_info =
      vk::descriptor_set_allocate_info_template;
  allocate_info.descriptorPool = descriptor_pool;
  allocate_info.descriptorSetCount = 1;
  allocate_info.pSetLayouts = &descriptor_set_layout;

  VkResult result = vkAllocateDescriptorSets(device->GetHandle(),
                                             &allocate_info, &descriptor_set);
  if (result) {
    throw VulkanException("cant allocate descriptor set");
  }

  VkDescriptorBufferInfo descriptor_buffer_info;
  descriptor_buffer_info.buffer = uniform_buffer->GetHandle();
  descriptor_buffer_info.offset = 0;
  descriptor_buffer_info.range = sizeof(UniformData);

  VkWriteDescriptorSet write_descriptor_set = vk::write_descriptor_set_template;
  write_descriptor_set.dstSet = descriptor_set;
  write_descriptor_set.descriptorCount = 1;
  write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  write_descriptor_set.pBufferInfo = &descriptor_buffer_info;
  write_descriptor_set.dstBinding = 0;

  vkUpdateDescriptorSets(device->GetHandle(), 1, &write_descriptor_set, 0,
                         nullptr);
}

void GraphicsVulkanApplication::CreateCommandBuffer() {
  command_buffers.resize(swapchain_images.size());

  VkCommandBufferAllocateInfo allocate_info =
      vk::command_buffer_allocate_info_template;
  allocate_info.commandPool = command_pool;
  allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocate_info.commandBufferCount = command_buffers.size();

  VkResult result;

  result = vkAllocateCommandBuffers(device->GetHandle(), &allocate_info,
                                    command_buffers.data());
  if (result) {
    throw VulkanException("cant allocate command buffers");
  }

  VkCommandBufferBeginInfo begin_info = vk::command_buffer_begin_info_template;

  VkImageSubresourceRange image_subresource_range;
  image_subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  image_subresource_range.baseMipLevel = 0;
  image_subresource_range.levelCount = 1;
  image_subresource_range.baseArrayLayer = 0;
  image_subresource_range.layerCount = 1;

  VkClearValue clear_color = {0.1, 0.3, 0.1, 0.1};

  for (size_t i = 0; i < command_buffers.size(); i++) {
    vkBeginCommandBuffer(command_buffers[i], &begin_info);

    VkImageMemoryBarrier present_to_draw_barrier =
        vk::image_memory_barrier_template;
    present_to_draw_barrier.srcAccessMask = 0;
    present_to_draw_barrier.dstAccessMask =
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    present_to_draw_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    present_to_draw_barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    present_to_draw_barrier.srcQueueFamilyIndex = queue_family;
    present_to_draw_barrier.dstQueueFamilyIndex = queue_family;
    present_to_draw_barrier.image = swapchain_images[i];
    present_to_draw_barrier.subresourceRange = image_subresource_range;

    vkCmdPipelineBarrier(command_buffers[i],
                         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0,
                         nullptr, 0, nullptr, 1, &present_to_draw_barrier);

    VkRenderPassBeginInfo render_pass_begin_info =
        vk::render_pass_begin_info_template;
    render_pass_begin_info.renderPass = render_pass;
    render_pass_begin_info.framebuffer = swapchain_framebuffers[i];
    render_pass_begin_info.renderArea.offset.x = 0;
    render_pass_begin_info.renderArea.offset.y = 0;
    render_pass_begin_info.renderArea.extent = swapchain_extent;
    render_pass_begin_info.clearValueCount = 1;
    render_pass_begin_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(command_buffers[i], &render_pass_begin_info,
                         VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindDescriptorSets(command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                            pipeline_layout, 0, 1, &descriptor_set, 0, nullptr);

    vkCmdBindPipeline(command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                      pipeline);

    VkDeviceSize offset = 0;
    VkBuffer vertex_buffer_handle = vertex_buffer->GetHandle();
    vkCmdBindVertexBuffers(command_buffers[i], 0, 1, &vertex_buffer_handle,
                           &offset);

    vkCmdBindIndexBuffer(command_buffers[i], index_buffer->GetHandle(), 0,
                         VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(command_buffers[i], 3, 1, 0, 0, 0);

    vkCmdEndRenderPass(command_buffers[i]);

    result = vkEndCommandBuffer(command_buffers[i]);
    if (result) {
      throw VulkanException("cant end command buffer");
    }
  }
}

void GraphicsVulkanApplication::CreateGraphicsPipeline() {
  CreateShaderModule("shaders/test_v.spv", vertex_shader_module);
  CreateShaderModule("shaders/test_f.spv", fragment_shader_module);

  // shader stages
  VkPipelineShaderStageCreateInfo vertex_shader_create_info =
      vk::pipeline_shader_stage_create_info_template;
  vertex_shader_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertex_shader_create_info.module = vertex_shader_module;
  vertex_shader_create_info.pName = "main";

  VkPipelineShaderStageCreateInfo fragment_shader_create_info =
      vk::pipeline_shader_stage_create_info_template;
  fragment_shader_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragment_shader_create_info.module = fragment_shader_module;
  fragment_shader_create_info.pName = "main";

  VkPipelineShaderStageCreateInfo shader_stages[] = {
      vertex_shader_create_info, fragment_shader_create_info};

  // vertex input
  VkPipelineVertexInputStateCreateInfo vertex_input_create_info =
      vk::vertex_input_create_info_template;
  vertex_input_create_info.vertexBindingDescriptionCount = 1;
  vertex_input_create_info.pVertexBindingDescriptions =
      &vertex_binding_description;
  vertex_input_create_info.vertexAttributeDescriptionCount = 2;
  vertex_input_create_info.pVertexAttributeDescriptions =
      vertex_attribute_desctiptions;

  // input assembly
  VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info =
      vk::pipeline_input_assembly_create_info_template;
  input_assembly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly_create_info.primitiveRestartEnable = false;

  // viewport and scissor
  VkViewport viewport = {};
  viewport.x = 0;
  viewport.y = 0;
  viewport.width = swapchain_extent.width;
  viewport.height = swapchain_extent.height;
  viewport.minDepth = 0;
  viewport.maxDepth = 1;

  VkRect2D scissor = {};
  scissor.offset.x = 0;
  scissor.offset.y = 0;
  scissor.extent.width = swapchain_extent.width;
  scissor.extent.height = swapchain_extent.height;

  VkPipelineViewportStateCreateInfo viewport_create_info =
      vk::pipeline_viewport_create_info_template;
  viewport_create_info.viewportCount = 1;
  viewport_create_info.pViewports = &viewport;
  viewport_create_info.scissorCount = 1;
  viewport_create_info.pScissors = &scissor;

  // rasterization
  VkPipelineRasterizationStateCreateInfo rasterization_state_create_info =
      vk::pipeline_rasterization_state_create_info_template;
  rasterization_state_create_info.depthClampEnable = VK_FALSE;
  rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
  rasterization_state_create_info.polygonMode = VK_POLYGON_MODE_FILL;
  rasterization_state_create_info.cullMode = 0;
  rasterization_state_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterization_state_create_info.lineWidth = 1;

  VkPipelineMultisampleStateCreateInfo multisample_state_create_info =
      vk::pipeline_multisample_state_create_info_template;

  VkPipelineColorBlendAttachmentState blend_attachment_state = {};
  blend_attachment_state.blendEnable = VK_FALSE;
  blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
  blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;
  blend_attachment_state.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

  VkPipelineColorBlendStateCreateInfo color_blend_create_info =
      vk::pipeline_color_blend_state_create_info_template;
  color_blend_create_info.attachmentCount = 1;
  color_blend_create_info.pAttachments = &blend_attachment_state;

  VkDescriptorSetLayoutBinding descriptor_set_layout_binding;
  descriptor_set_layout_binding.binding = 0;
  descriptor_set_layout_binding.descriptorType =
      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  descriptor_set_layout_binding.descriptorCount = 1;
  descriptor_set_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

  VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info =
      vk::descriptor_set_layout_create_info_template;
  descriptor_set_layout_create_info.bindingCount = 1;
  descriptor_set_layout_create_info.pBindings = &descriptor_set_layout_binding;

  VkResult result;

  result = vkCreateDescriptorSetLayout(device->GetHandle(),
                                       &descriptor_set_layout_create_info,
                                       nullptr, &descriptor_set_layout);
  if (result) {
    throw VulkanException("cant create descriptor set");
  }

  VkPipelineLayoutCreateInfo layout_create_info =
      vk::pipeline_layout_create_info_template;
  layout_create_info.setLayoutCount = 1;
  layout_create_info.pSetLayouts = &descriptor_set_layout;

  result = vkCreatePipelineLayout(device->GetHandle(), &layout_create_info,
                                  nullptr, &pipeline_layout);
  if (result) {
    throw VulkanException("cant create pipeline layout");
  }

  VkGraphicsPipelineCreateInfo pipeline_create_info =
      vk::graphics_pipeline_create_info_template;
  pipeline_create_info.stageCount = 2;
  pipeline_create_info.pStages = shader_stages;
  pipeline_create_info.pVertexInputState = &vertex_input_create_info;
  pipeline_create_info.pInputAssemblyState = &input_assembly_create_info;
  pipeline_create_info.pViewportState = &viewport_create_info;
  pipeline_create_info.pRasterizationState = &rasterization_state_create_info;
  pipeline_create_info.pMultisampleState = &multisample_state_create_info;
  pipeline_create_info.pColorBlendState = &color_blend_create_info;
  pipeline_create_info.layout = pipeline_layout;
  pipeline_create_info.renderPass = render_pass;
  pipeline_create_info.subpass = 0;
  pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
  pipeline_create_info.basePipelineIndex = -1;

  result = vkCreateGraphicsPipelines(device->GetHandle(), 0, 1,
                                     &pipeline_create_info, nullptr, &pipeline);
  if (result) {
    throw VulkanException("cant create graphics pipeline");
  }
}

void GraphicsVulkanApplication::CreateImagesView() {
  swapchain_image_views.resize(swapchain_images.size());

  for (int i = 0; i < swapchain_image_views.size(); i++) {
    VkImageViewCreateInfo create_info = vk::image_view_create_info_template;
    create_info.image = swapchain_images[i];
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = surface_format.format;
    create_info.components = {}; // identity
    create_info.subresourceRange = vk::image_subresource_range_template;
    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    VkResult result = vkCreateImageView(device->GetHandle(), &create_info,
                                        nullptr, &swapchain_image_views[i]);
    if (result) {
      throw VulkanException("cant create swapchain image view");
    }
  }
}

void GraphicsVulkanApplication::CreateFramebuffer() {
  swapchain_framebuffers.resize(swapchain_images.size());

  for (int i = 0; i < swapchain_framebuffers.size(); i++) {
    VkFramebufferCreateInfo create_info = vk::framebuffer_create_info_template;
    create_info.renderPass = render_pass;
    create_info.attachmentCount = 1;
    create_info.pAttachments = &swapchain_image_views[i];
    create_info.height = swapchain_extent.height;
    create_info.width = swapchain_extent.width;
    create_info.layers = 1;

    VkResult result = vkCreateFramebuffer(device->GetHandle(), &create_info,
                                          nullptr, &swapchain_framebuffers[i]);
    if (result) {
      throw VulkanException("cant create frambuffer");
    }
  }
}

void GraphicsVulkanApplication::CreateRenderPass() {
  VkAttachmentDescription attachment_descriptor =
      vk::attachment_descriptor_template;
  attachment_descriptor.format = surface_format.format;
  attachment_descriptor.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attachment_descriptor.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attachment_descriptor.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  attachment_descriptor.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_attachment_reference;
  color_attachment_reference.attachment = 0;
  color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass_description = vk::subpass_description_template;
  subpass_description.colorAttachmentCount = 1;
  subpass_description.pColorAttachments = &color_attachment_reference;

  VkRenderPassCreateInfo create_info = vk::render_pass_create_info_template;
  create_info.attachmentCount = 1;
  create_info.pAttachments = &attachment_descriptor;
  create_info.subpassCount = 1;
  create_info.pSubpasses = &subpass_description;

  VkResult result = vkCreateRenderPass(device->GetHandle(), &create_info,
                                       nullptr, &render_pass);
  if (result) {
    throw VulkanException("cant create render pass");
  }
}

void GraphicsVulkanApplication::CreateSwapchain() { // TOLRN
  VkResult result;

  if (swapchain != VK_NULL_HANDLE) {
    vkDestroySwapchainKHR(device->GetHandle(), swapchain, nullptr);
  }

  vk::PhysicalDevice &physical_device = device->GetPhysicalDevice();

  // get capabilities
  VkSurfaceCapabilitiesKHR surface_capabilities =
      physical_device.GetSurfaceCapabilities(window_surface);
  vector<VkSurfaceFormatKHR> supported_surface_formats =
      physical_device.GetSurfaceFormats(window_surface);
  vector<VkPresentModeKHR> supported_present_modes =
      physical_device.GetSurfacePresentModes(window_surface);

  uint32_t desired_swapchain_images_count =
      surface_capabilities.minImageCount + 1;
  if (surface_capabilities.maxImageCount &&
      desired_swapchain_images_count > surface_capabilities.maxImageCount) {
    desired_swapchain_images_count = surface_capabilities.maxImageCount;
  }

  // choose swapchain details
  surface_format = ChooseSurfaceFormat(supported_surface_formats);

  VkPresentModeKHR present_mode = ChoosePresentMode(supported_present_modes);

  VkSurfaceTransformFlagBitsKHR surface_transform =
      ChooseSurfaceTransform(surface_capabilities);

  swapchain_extent = ChooseSwapchainExtend(surface_capabilities);

  VkSwapchainCreateInfoKHR create_info = vk::swapchain_create_info_template;
  create_info.surface = window_surface;
  create_info.minImageCount = desired_swapchain_images_count;
  create_info.imageFormat = surface_format.format;
  create_info.imageColorSpace = surface_format.colorSpace;
  create_info.imageExtent = swapchain_extent;
  create_info.imageArrayLayers = 1;
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  create_info.preTransform = surface_transform;
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  create_info.presentMode = present_mode;
  create_info.clipped = VK_TRUE;
  create_info.oldSwapchain = nullptr; // TODO

  result = vkCreateSwapchainKHR(device->GetHandle(), &create_info, nullptr,
                                &swapchain);
  if (result) {
    throw VulkanException("cant create swapchain");
  }

  uint32_t swapchain_images_count = 0;
  result = vkGetSwapchainImagesKHR(device->GetHandle(), swapchain,
                                   &swapchain_images_count, nullptr);
  if (result) {
    throw VulkanException("cant get swapchain images count");
  } else if (!swapchain_images_count) {
    throw VulkanException("swapchain images count is 0");
  }

  swapchain_images.resize(swapchain_images_count);

  result =
      vkGetSwapchainImagesKHR(device->GetHandle(), swapchain,
                              &swapchain_images_count, swapchain_images.data());
  if (result) {
    throw VulkanException("cant get swapchain images");
  }
}

VkExtent2D GraphicsVulkanApplication::ChooseSwapchainExtend(
    VkSurfaceCapabilitiesKHR surface_capabilities) {
  return surface_capabilities.currentExtent; // TODO
}

VkSurfaceTransformFlagBitsKHR GraphicsVulkanApplication::ChooseSurfaceTransform(
    VkSurfaceCapabilitiesKHR surface_capabilities) {
  return surface_capabilities.currentTransform; // TODO
}

VkPresentModeKHR GraphicsVulkanApplication::ChoosePresentMode(
    vector<VkPresentModeKHR> &supported_present_modes) {
  return supported_present_modes[0]; // TODO
}

VkSurfaceFormatKHR GraphicsVulkanApplication::ChooseSurfaceFormat(
    vector<VkSurfaceFormatKHR> &supported_formats) { // TOLRN
  if (supported_formats.size() == 1 &&
      supported_formats[0].format == VK_FORMAT_UNDEFINED) {
    return {VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR};
  }

  for (const auto &format : supported_formats) {
    if (format.format == VK_FORMAT_R8G8B8A8_UNORM) {
      return format;
    }
  }

  return supported_formats[0];
}

void GraphicsVulkanApplication::CreateWindowSurface() {
  if (glfwCreateWindowSurface(instance->GetHandle(), window, nullptr,
                              &window_surface)) {
    throw VulkanException("cant create glfw window surface");
  }
}

void GraphicsVulkanApplication::RunRenderLoop() {
  UpdateUniformBuffer();

  while (!glfwWindowShouldClose(window)) {
	Draw();

    glfwPollEvents();
  }
}

void GraphicsVulkanApplication::Draw() {
  VkResult result;

  uint32_t image_index;
  result = vkAcquireNextImageKHR(device->GetHandle(), swapchain, UINT64_MAX,
                                 image_ready_semaphore, VK_NULL_HANDLE,
                                 &image_index);
  if (result) {
    throw VulkanException("cant get next swapchain image");
  }

  VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

  VkSubmitInfo submit_info = vk::submit_info_template;
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = &image_ready_semaphore;
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = &render_finished_semaphore;
  submit_info.pWaitDstStageMask = &wait_dst_stage_mask;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &command_buffers[image_index];

  result = vkQueueSubmit(device->GetQueue(), 1, &submit_info, VK_NULL_HANDLE);
  if (result) {
    throw VulkanException("cant submit to queue");
  }

  VkPresentInfoKHR present_info = vk::present_info_template;
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = &render_finished_semaphore;
  present_info.swapchainCount = 1;
  present_info.pSwapchains = &swapchain;
  present_info.pImageIndices = &image_index;

  result = vkQueuePresentKHR(device->GetQueue(), &present_info);
  if(result){
	throw VulkanException("queue present failed");
  }
}

void GraphicsVulkanApplication::CreateVertexBuffer() {
  std::vector<Vertex> vertices = {{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                                  {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                                  {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}};
  uint32_t vertices_size = (uint32_t)(vertices.size() * sizeof(vertices[0]));

  std::vector<uint32_t> indices = {0, 1, 2};
  uint32_t indices_size = (uint32_t)(indices.size() * sizeof(indices[0]));

  // create buffers
  vk::BufferCreateInfo vertex_buffer_create_info;
  vertex_buffer_create_info.queue = queue_family;
  vertex_buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  vertex_buffer_create_info.size = vertices_size;

  vertex_buffer = unique_ptr<vk::Buffer>(
      new vk::Buffer(device->GetHandle(), vertex_buffer_create_info));

  vk::BufferCreateInfo index_buffer_create_info;
  index_buffer_create_info.queue = queue_family;
  index_buffer_create_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  index_buffer_create_info.size = indices_size;

  index_buffer = unique_ptr<vk::Buffer>(
      new vk::Buffer(device->GetHandle(), index_buffer_create_info));

  // allocate memory
  uint32_t memory_types_bits =
      vertex_buffer->GetMemoryTypes() | index_buffer->GetMemoryTypes();

  uint32_t memory_type = device->ChooseMemoryType(
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, 0, memory_types_bits);

  vector<vk::Buffer *> buffers{vertex_buffer.get(), index_buffer.get()};

  VkDeviceSize memory_size = vk::DeviceMemory::CalculateMemorySize(buffers);

  vertex_buffer_memory = unique_ptr<vk::DeviceMemory>(
      new vk::DeviceMemory(*device, memory_size, memory_type));

  vertex_buffer_memory->BindBuffer(*vertex_buffer);
  vertex_buffer_memory->BindBuffer(*index_buffer);

  // load data
  char *mapped_ptr;

  mapped_ptr = (char *)vertex_buffer->Map();
  memcpy(mapped_ptr, vertices.data(), vertices_size);
  index_buffer->Flush();
  index_buffer->Unmap();

  mapped_ptr = (char *)index_buffer->Map();
  memcpy(mapped_ptr, indices.data(), indices_size);
  vertex_buffer->Flush();
  vertex_buffer->Unmap();

  vertex_binding_description.binding = 0;
  vertex_binding_description.stride = sizeof(Vertex);
  vertex_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

  vertex_attribute_desctiptions[0].binding = 0;
  vertex_attribute_desctiptions[0].location = 0;
  vertex_attribute_desctiptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  vertex_attribute_desctiptions[0].offset = 0;

  vertex_attribute_desctiptions[1].binding = 0;
  vertex_attribute_desctiptions[1].location = 1;
  vertex_attribute_desctiptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  vertex_attribute_desctiptions[1].offset = sizeof(float) * 3;
}

void GraphicsVulkanApplication::CreateUniformBuffer() {
  vk::BufferCreateInfo create_info;
  create_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  create_info.size = sizeof(UniformData);
  create_info.queue = queue_family;

  uniform_buffer =
      unique_ptr<vk::Buffer>(new vk::Buffer(device->GetHandle(), create_info));

  uint32_t memory_type = device->ChooseMemoryType(
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, 0, uniform_buffer->GetMemoryTypes());

  vector<vk::Buffer> memory_buffers;
  VkDeviceSize memory_size =
      vk::DeviceMemory::CalculateMemorySize({uniform_buffer.get()});

  uniform_buffer_memory = unique_ptr<vk::DeviceMemory>(
      new vk::DeviceMemory(*device, memory_size, memory_type));

  uniform_buffer_memory->BindBuffer(*uniform_buffer);

  UpdateUniformBuffer();
}

void GraphicsVulkanApplication::UpdateUniformBuffer() {
  // auto timeNow = std::chrono::high_resolution_clock::now();
  // long long millis =
  //     std::chrono::duration_cast<std::chrono::milliseconds>(timeStart -
  //     timeNow)
  //         .count();
  // float angle = (millis % 4000) / 4000.0f * glm::radians(360.f);

  // glm::mat4 modelMatrix;
  // modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 0, 1));
  // modelMatrix =
  //     glm::translate(modelMatrix, glm::vec3(0.5f / 3.0f, -0.5f / 3.0f,
  //     0.0f));

  // // Set up view
  // auto viewMatrix =
  //     glm::lookAt(glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0, 0,
  //     -1));

  // // Set up projection
  // auto projMatrix = glm::perspective(
  //     glm::radians(70.f), swapChainExtent.width /
  //     (float)swapChainExtent.height, 0.1f, 10.0f);

  // uniformBufferData.transformationMatrix =
  //     projMatrix * viewMatrix * modelMatrix;

  // void *data;
  // vkMapMemory(device, uniformBufferMemory, 0, sizeof(uniformBufferData), 0,
  //             &data);
  // memcpy(data, &uniformBufferData, sizeof(uniformBufferData));
  // vkUnmapMemory(device, uniformBufferMemory);
} // TODO

void GraphicsVulkanApplication::CreateSemaphores() {
  VkSemaphoreCreateInfo create_info = vk::semaphore_create_info_template;

  VkResult result;

  result = vkCreateSemaphore(device->GetHandle(), &create_info, nullptr,
                             &image_ready_semaphore);
  if (result) {
    throw VulkanException("cant create semaphore");
  }

  result = vkCreateSemaphore(device->GetHandle(), &create_info, nullptr,
                             &render_finished_semaphore);
  if (result) {
    throw VulkanException("cant create semaphore");
  }
}

void GraphicsVulkanApplication::CreateCommandPool() {
  VkCommandPoolCreateInfo create_info = vk::command_pool_create_info_template;
  create_info.queueFamilyIndex = queue_family;

  VkResult result = vkCreateCommandPool(device->GetHandle(), &create_info,
                                        nullptr, &command_pool);
  if (result) {
    throw VulkanException("cant create command pool");
  }
}

void GraphicsVulkanApplication::InitVulkan() {
  // create instance
  vk::InstanceCreateInfo instance_create_info;

  uint32_t glfw_extensions_count;
  const char **glfw_extensions =
      glfwGetRequiredInstanceExtensions(&glfw_extensions_count);

  for (int i = 0; i < glfw_extensions_count; i++) {
    instance_create_info.extensions.push_back(glfw_extensions[i]);
  }

  instance = unique_ptr<vk::Instance>(new vk::Instance(instance_create_info));

  // choose physical device
  shared_ptr<vk::PhysicalDevice> physical_device =
      instance->physical_devices[0];

  queue_family = physical_device->ChooseQueueFamily(VK_QUEUE_GRAPHICS_BIT |
                                                    VK_QUEUE_TRANSFER_BIT);

  // create device
  vk::DeviceCreateInfo device_create_info;
  device_create_info.queue_family = queue_family;
  device_create_info.extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
  device_create_info.features = {};
  device_create_info.features.shaderClipDistance = VK_TRUE; // TODO wtf
  device_create_info.features.shaderCullDistance = VK_TRUE;

  device = unique_ptr<vk::Device>(
      new vk::Device(physical_device, device_create_info));
}

void GraphicsVulkanApplication::CreateShaderModule(
    string path, VkShaderModule &shader_module) {
  ifstream file(path);
  if (!file) {
    throw VulkanException("cant open shader file: " + path);
  }

  file.seekg(0, ios::end);
  vector<char> code(file.tellg());

  file.seekg(0, ios::beg);
  file.read(code.data(), code.size());

  VkShaderModuleCreateInfo create_info = vk::shader_module_create_info_template;
  create_info.codeSize = code.size();
  create_info.pCode = (uint32_t *)code.data();

  VkResult result = vkCreateShaderModule(device->GetHandle(), &create_info,
                                         nullptr, &shader_module);
  if (result) {
    throw VulkanException("cant create shader module");
  }
}
