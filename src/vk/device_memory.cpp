#include "device_memory.hpp"

namespace vk {

DeviceMemory::DeviceMemory(VkDevice device, VkDeviceSize size, uint32_t type) {
  host_device = device;
  this->size = size;

  VkMemoryAllocateInfo vk_allocate_info;
  vk_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  vk_allocate_info.pNext = nullptr;
  vk_allocate_info.allocationSize = size;
  vk_allocate_info.memoryTypeIndex = type;

  VkResult result =
      vkAllocateMemory(host_device, &vk_allocate_info, nullptr, &handle);
  if (result) {
    throw VulkanException("cant allocate memory");
  }
}

DeviceMemory::~DeviceMemory() { Free(); }

void DeviceMemory::Free() {
  if (handle) {
    vkFreeMemory(host_device, handle, nullptr);
  }
}

void DeviceMemory::BindBuffer(Buffer &buffer) {
  VkMemoryRequirements requirements;
  VkDeviceSize pos =
      FindPlaceForBuffer(requirements.size, requirements.alignment);

  VkResult result =
      vkBindBufferMemory(host_device, buffer.GetHandle(), handle, pos);
  if (result) {
    throw VulkanException("cant bind buffer to memory");
  }
}

VkDeviceSize DeviceMemory::FindPlaceForBuffer(VkDeviceSize size,
                                              VkDeviceSize alignment) {
  return 0;
}

} // namespace vk
