#include "vulkan_application.hpp"
#include "vk/templates.hpp"

void VulkanApplication::Run() {
  InitVulkan();

  CreateBuffers();

  CreateCommandPool();
  CreateCommandBuffer();

  FillCommandBuffer();
  RunQueue();
}

void VulkanApplication::InitVulkan() {
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

void VulkanApplication::FillCommandBuffer() {
  VkCommandBufferBeginInfo begin_info;
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.pNext = nullptr;
  begin_info.flags = 0;
  begin_info.pInheritanceInfo = nullptr;

  VkResult result = vkBeginCommandBuffer(command_buffer, &begin_info);
  if (result) {
    throw VulkanException("cant begin command buffer");
  }

  // flush mapped memory barrier
  VkBufferMemoryBarrier buffer_memory_barrier1 = vk::buffer_barrier_template;
  buffer_memory_barrier1.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
  buffer_memory_barrier1.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  buffer_memory_barrier1.buffer = buffers[0]->GetHandle();

  vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_HOST_BIT,
                       VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 1,
                       &buffer_memory_barrier1, 0, nullptr);

  // mode data
  VkBufferCopy region;
  region.dstOffset = 0;
  region.srcOffset = 0;
  region.size = buffers[0]->GetSize();

  vkCmdCopyBuffer(command_buffer, buffers[0]->GetHandle(),
                  buffers[1]->GetHandle(), 1, &region);

  // wait to read memory from host barrier
  VkBufferMemoryBarrier buffer_memory_barrier2 = vk::buffer_barrier_template;
  buffer_memory_barrier2.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  buffer_memory_barrier2.dstAccessMask = VK_ACCESS_HOST_READ_BIT;
  buffer_memory_barrier2.buffer = buffers[1]->GetHandle();

  vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                       VK_PIPELINE_STAGE_HOST_BIT, 0, 0, nullptr, 1,
                       &buffer_memory_barrier2, 0, nullptr);

  vkEndCommandBuffer(command_buffer);
}

void VulkanApplication::RunQueue() {
  VkSubmitInfo submit_info;
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.pNext = nullptr;
  submit_info.waitSemaphoreCount = 0;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &command_buffer;
  submit_info.signalSemaphoreCount = 0;

  VkResult result = vkQueueSubmit(device->GetQueue(), 1, &submit_info, 0);
  if (result) {
    throw VulkanException("cant submit queue");
  }

  result = vkQueueWaitIdle(device->GetQueue());
  if (result) {
    throw VulkanException("queue wait idle failed");
  }

  void *mapped_data = buffers[1]->Map();

  cout << (char *)mapped_data << endl;

  buffers[1]->Unmap();
}

void VulkanApplication::CreateCommandPool() {
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

void VulkanApplication::CreateCommandBuffer() {
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

void VulkanApplication::CreateBuffers() {
  // create buffers
  vk::BufferCreateInfo buffer_create_info;
  buffer_create_info.size = 1024;
  buffer_create_info.usage =
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
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

  void *mapped_memory = buffers[0]->Map();

  memcpy((char *)mapped_memory, "test", 5);

  buffers[0]->Flush();
  buffers[0]->Unmap();
}
