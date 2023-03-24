#include "vulkan_application.hpp"

void VulkanApplication::Run() {
  InitVulkan();
  CreateBuffers();
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

  buffers[0]->Map();
  buffers[0]->Flush();
  buffers[0]->Unmap();

  // unbind buffer
  for (int i = 0; i < 4; i++) {
    buffers[i]->Destroy();
  }
}
