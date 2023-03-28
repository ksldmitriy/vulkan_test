#include "compute_vulkan_application.hpp"
#include "vk/templates.hpp"

void ComputeVulkanApplication::Run() {
  InitVulkan();

  CreateBuffers();

  CreateShaderModule();
  CreatePipeline();

  CreateCommandPool();
  CreateCommandBuffer();

  FillCommandBuffer();
  RunQueue();
}

void ComputeVulkanApplication::InitVulkan() {
  // create instance
  vk::InstanceCreateInfo instance_create_info;
  instance = unique_ptr<vk::Instance>(new vk::Instance(instance_create_info));

  shared_ptr<vk::PhysicalDevice> physical_device =
      instance->physical_devices[0];

  queue_family = physical_device->ChooseQueueFamily(0);

  // create device
  vk::DeviceCreateInfo device_create_info;
  device_create_info.features = {};
  device_create_info.queue_family = queue_family;

  device = unique_ptr<vk::Device>(
      new vk::Device(physical_device, device_create_info));
}

void ComputeVulkanApplication::CreateShaderModule() {
  // read shader from file

  ifstream file("shaders/test.spv");
  if (!file) {
    throw VulkanException("cant open shader file");
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
void ComputeVulkanApplication::CreatePipeline() {
  VkResult result;

  VkDescriptorSetLayoutBinding descriptor_set_layout_binding;
  descriptor_set_layout_binding.binding = 0;
  descriptor_set_layout_binding.descriptorType =
      VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  descriptor_set_layout_binding.descriptorCount = 1;
  descriptor_set_layout_binding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
  descriptor_set_layout_binding.pImmutableSamplers = nullptr;

  VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info =
      vk::descriptor_set_create_info_template;
  descriptor_set_layout_create_info.bindingCount = 1;
  descriptor_set_layout_create_info.pBindings = &descriptor_set_layout_binding;

  result = vkCreateDescriptorSetLayout(device->GetHandle(),
                                       &descriptor_set_layout_create_info,
                                       nullptr, &descriptor_set_layout);
  if (result) {
    throw VulkanException("cant create descriptor set layout");
  }

  VkPipelineLayoutCreateInfo pipeline_layout_create_info =
      vk::pipeline_layout_create_info_template;
  pipeline_layout_create_info.setLayoutCount = 1;
  pipeline_layout_create_info.pSetLayouts = &descriptor_set_layout;
  pipeline_layout_create_info.pushConstantRangeCount = 0;
  pipeline_layout_create_info.pPushConstantRanges = nullptr;

  result =
      vkCreatePipelineLayout(device->GetHandle(), &pipeline_layout_create_info,
                             nullptr, &pipeline_layout);

  VkDescriptorPoolSize descriptor_pool_size;
  descriptor_pool_size.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  descriptor_pool_size.descriptorCount = 1;

  VkDescriptorPoolCreateInfo descriptor_pool_create_info =
      vk::descriptor_pool_create_info_template;
  descriptor_pool_create_info.maxSets = 1;
  descriptor_pool_create_info.poolSizeCount = 1;
  descriptor_pool_create_info.pPoolSizes = &descriptor_pool_size;

  result =
      vkCreateDescriptorPool(device->GetHandle(), &descriptor_pool_create_info,
                             nullptr, &descriptor_pool);
  if (result) {
    throw VulkanException("cant create descriptor pool");
  }

  VkDescriptorSetAllocateInfo descriptor_set_allocate_info =
      vk::descriptor_set_allocate_info_template;
  descriptor_set_allocate_info.descriptorPool = descriptor_pool;
  descriptor_set_allocate_info.descriptorSetCount = 1;
  descriptor_set_allocate_info.pSetLayouts = &descriptor_set_layout;

  result = vkAllocateDescriptorSets(
      device->GetHandle(), &descriptor_set_allocate_info, &descriptor_set);
  if (result) {
    throw VulkanException("cant allocate descriptor set");
  }

  VkDescriptorBufferInfo descriptor_buffer_info;
  descriptor_buffer_info.buffer = buffers[0]->GetHandle();
  descriptor_buffer_info.offset = 0;
  descriptor_buffer_info.range = VK_WHOLE_SIZE;

  VkWriteDescriptorSet write_descriptor_set = vk::write_descriptor_set_template;
  write_descriptor_set.dstSet = descriptor_set;
  write_descriptor_set.dstBinding = 0;
  write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  write_descriptor_set.pBufferInfo = &descriptor_buffer_info;

  vkUpdateDescriptorSets(device->GetHandle(), 1, &write_descriptor_set, 0,
                         nullptr);

  VkSpecializationInfo specialization_info;
  specialization_info.mapEntryCount = 0;

  VkPipelineShaderStageCreateInfo stage_create_info =
      vk::pipeline_shader_stage_create_info_template;
  stage_create_info.stage = VK_SHADER_STAGE_COMPUTE_BIT;
  stage_create_info.module = shader_module;
  stage_create_info.pName = "main";
  stage_create_info.pSpecializationInfo = &specialization_info;

  VkComputePipelineCreateInfo pipeline_create_info =
      vk::compute_pipeline_create_info_template;
  pipeline_create_info.stage = stage_create_info;
  pipeline_create_info.layout = pipeline_layout;

  result = vkCreateComputePipelines(device->GetHandle(), 0, 1,
                                    &pipeline_create_info, nullptr, &pipeline);
  if (result) {
    throw VulkanException("cant create pipeline");
  }
}

void ComputeVulkanApplication::FillCommandBuffer() {
  VkCommandBufferBeginInfo begin_info = vk::command_buffer_begin_info_template;
  begin_info.flags = 0;
  begin_info.pInheritanceInfo = nullptr;

  VkResult result = vkBeginCommandBuffer(command_buffer, &begin_info);
  if (result) {
    throw VulkanException("cant begin command buffer");
  }

  // flush mapped memory barrier
  VkBufferMemoryBarrier buffer_memory_barrier1 = vk::buffer_barrier_template;
  buffer_memory_barrier1.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
  buffer_memory_barrier1.dstAccessMask =
      VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;
  buffer_memory_barrier1.buffer = buffers[0]->GetHandle();

  vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_HOST_BIT,
                       VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, nullptr, 1,
                       &buffer_memory_barrier1, 0, nullptr);

  vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                          pipeline_layout, 0, 1, &descriptor_set, 0, nullptr);

  vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);

  vkCmdDispatch(command_buffer, 3, 1, 1);

  // wait to read memory from host barrier
  VkBufferMemoryBarrier buffer_memory_barrier2 = vk::buffer_barrier_template;
  buffer_memory_barrier2.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
  buffer_memory_barrier2.dstAccessMask = VK_ACCESS_HOST_READ_BIT;
  buffer_memory_barrier2.buffer = buffers[0]->GetHandle();

  vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                       VK_PIPELINE_STAGE_HOST_BIT, 0, 0, nullptr, 1,
                       &buffer_memory_barrier2, 0, nullptr);

  vkEndCommandBuffer(command_buffer);
}

void ComputeVulkanApplication::RunQueue() {
  VkSubmitInfo submit_info = vk::submit_info_template;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &command_buffer;

  VkResult result = vkQueueSubmit(device->GetQueue(), 1, &submit_info, 0);
  if (result) {
    throw VulkanException("cant submit queue");
  }

  result = vkQueueWaitIdle(device->GetQueue());
  if (result) {
    throw VulkanException("queue wait idle failed");
  }

  uint32_t *mapped_data = (uint32_t *)buffers[0]->Map();

  for (int i = 0; i < 10; i++) {
    cout << mapped_data[i] << endl;
  }

  buffers[0]->Unmap();
}

void ComputeVulkanApplication::CreateCommandPool() {
  VkCommandPoolCreateInfo create_info;
  create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  create_info.pNext = nullptr;
  create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  create_info.queueFamilyIndex = device->GetQueueFamily();

  VkResult result = vkCreateCommandPool(device->GetHandle(), &create_info,
                                        nullptr, &command_pool);
  if (result) {
    throw VulkanException("cant create command pool");
  }
}

void ComputeVulkanApplication::CreateCommandBuffer() {
  VkCommandBufferAllocateInfo allocate_info;
  allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocate_info.pNext = nullptr;
  allocate_info.commandPool = command_pool;
  allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocate_info.commandBufferCount = 1;

  VkResult result = vkAllocateCommandBuffers(device->GetHandle(),
                                             &allocate_info, &command_buffer);
  if (result) {
    throw VulkanException("cant allocate command buffer");
  }
}

void ComputeVulkanApplication::CreateBuffers() {
  // create buffers
  vk::BufferCreateInfo buffer_create_info;
  buffer_create_info.size = 1024;
  buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                             VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                             VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
  buffer_create_info.queue = queue_family;

  for (int i = 0; i < 4; i++) {
    buffers.push_back(unique_ptr<vk::Buffer>(
        new vk::Buffer(device->GetHandle(), buffer_create_info)));
  }

  // allocate memory
  VkMemoryPropertyFlags memory_requerments =
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
  VkMemoryHeapFlags heap_requerments = 0;
  uint32_t memory_type = device->ChooseMemoryType(
      memory_requerments, heap_requerments, buffers[0]->GetMemoryTypes());

  memory = unique_ptr<vk::DeviceMemory>(
      new vk::DeviceMemory(device->GetHandle(), 1024 * 4, memory_type));

  // bind buffers
  for (int i = 0; i < 4; i++) {
    memory->BindBuffer(*buffers[i]);
  }

  uint32_t *mapped_memory = (uint32_t *)buffers[0]->Map();

  for (int i = 0; i < 10; i++) {
    mapped_memory[i] = 228;
  }

  buffers[0]->Flush();
  buffers[0]->Unmap();
}
