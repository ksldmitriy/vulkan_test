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

  MemoryRange memory_range;
  memory_range.empty = true;
  memory_range.offset = 0;
  memory_range.size = size;

  memory_ranges.push_back(memory_range);
}

DeviceMemory::~DeviceMemory() { Free(); }

void DeviceMemory::Free() {
  if (handle) {
    vkFreeMemory(host_device, handle, nullptr);
  }
}

void DeviceMemory::BindBuffer(Buffer &buffer) {
  VkMemoryRequirements requirements = buffer.GetMemoryRequirements();
  uint32_t suitable_range_index =
      FindSuitableRange(requirements.size, requirements.alignment);

  MemoryRange &suitable_range = memory_ranges[suitable_range_index];
  VkDeviceSize pos = suitable_range.offset;

  VkResult result =
      vkBindBufferMemory(host_device, buffer.GetHandle(), handle, pos);
  if (result) {
    throw VulkanException("cant bind buffer to memory");
  }
}

uint32_t DeviceMemory::FindSuitableRange(VkDeviceSize size,
                                         VkDeviceSize alignment) {
  int optimal_memory_range = -1;
  VkDeviceSize optimal_memory_range_size;

  for (int i = 0; i < memory_ranges.size(); i++) {
    MemoryRange &memory_range = memory_ranges[i];
    if (!memory_range.empty) {
      continue;
    }

    VkDeviceSize aligned_pos = GetAlignedPos(memory_range.offset, alignment);
    VkDeviceSize aligned_size =
        memory_range.size - aligned_pos + memory_range.offset;

    if (aligned_size < size) {
      continue;
    }

	
    if (memory_range.size <= optimal_memory_range_size || optimal_memory_range == -1) {
      optimal_memory_range = i;
      optimal_memory_range_size = memory_range.size;
    }
  }

  if (optimal_memory_range == -1) {
    throw VulkanException("cant find empty memory range");
  }

  return optimal_memory_range;
}

VkDeviceSize DeviceMemory::GetAlignedPos(VkDeviceSize pos,
                                         VkDeviceSize alignment) {
  return ((pos + alignment - 1) / alignment) * alignment;
}

} // namespace vk
