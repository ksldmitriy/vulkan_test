#include "vulkan_application.hpp"
#include "vk/device_memory.hpp"

void VulkanApplication::Run() {
  // create instance
  vk::InstanceCreateInfo instance_create_info;
  instance = unique_ptr<vk::Instance>(new vk::Instance(instance_create_info));

  // create physical device
  vk::PhysicalDevice &physical_device = *instance->physical_devices[0];

  uint32_t queue_family = physical_device.ChooseQueueFamily(0);

  // create device
  vk::DeviceCreateInfo device_create_info;
  device_create_info.features = {};
  device_create_info.queue_family = queue_family;

  device = unique_ptr<vk::Device>(
      new vk::Device(physical_device, device_create_info));

  // create buffers
  vk::BufferCreateInfo buffer_create_info;
  buffer_create_info.size = 1024;
  buffer_create_info.usage =
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
  buffer_create_info.queue = queue_family;
  vector<unique_ptr<vk::Buffer>> buffers;
  for (int i = 0; i < 4; i++) {
    buffers.push_back(unique_ptr<vk::Buffer>(
        new vk::Buffer(device->GetHandle(), buffer_create_info)));
  }

  // allocate memory
  VkMemoryPropertyFlags memory_requerments =
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
  VkMemoryHeapFlags heap_requerments = 0;
  uint32_t memory_type = physical_device.ChooseMemoryType(
      memory_requerments, heap_requerments, buffers[0]->GetMemoryTypes());

  unique_ptr<vk::DeviceMemory> memory(
      new vk::DeviceMemory(device->GetHandle(), 1024 * 4, memory_type));

  memory->PrintSegments();

  for (int i = 0; i < buffers.size(); i++) {
    memory->BindBuffer(*buffers[i]);
    cout << endl;
    memory->PrintSegments();
  }

  for (int i = 0; i < buffers.size(); i+=2) {
	buffers[i]->Destroy();
    cout << endl;
    memory->PrintSegments();
  }

  for (int i = 1; i < buffers.size(); i+=2) {
	buffers[i]->Destroy();
    cout << endl;
    memory->PrintSegments();
  }
}
