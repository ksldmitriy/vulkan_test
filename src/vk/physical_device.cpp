#include "physical_device.hpp"

namespace vk {

PhysicalDevice::PhysicalDevice(VkPhysicalDevice handle) {
  this->handle = handle;

  vkGetPhysicalDeviceProperties(handle, &properties);

  vkGetPhysicalDeviceMemoryProperties(handle, &memory_properties);

  uint32_t queue_families_count;
  vkGetPhysicalDeviceQueueFamilyProperties(handle, &queue_families_count,
                                           nullptr);
  queue_families_properties.resize(queue_families_count);
  vkGetPhysicalDeviceQueueFamilyProperties(handle, &queue_families_count,
                                           queue_families_properties.data());
}

VkPhysicalDevice PhysicalDevice::GetHandle() { return handle; }

uint32_t PhysicalDevice::ChooseQueueFamily(VkQueueFlags requirements) {
  for (uint32_t i = 0; i < queue_families_properties.size(); i++) {
    if ((queue_families_properties[i].queueFlags & requirements) ==
        requirements) {
      return i;
    }
  }

  throw VulkanException("cant find needed queue");
};

uint32_t PhysicalDevice::ChooseMemoryType(VkMemoryPropertyFlags properties,
                                          VkMemoryHeapFlags heap_properties,
                                          uint32_t memory_types) {
  bool valid_heaps[VK_MAX_MEMORY_HEAPS] = {0};

  for (int i = 0; i < memory_properties.memoryHeapCount; i++) {
    VkMemoryHeap heap = memory_properties.memoryHeaps[i];
    if ((heap.flags & heap_properties) == heap_properties) {
      valid_heaps[i] = true;
    };
  }

  for (int i = 0; i < memory_properties.memoryTypeCount; i++) {
    VkMemoryType memory_type = memory_properties.memoryTypes[i];
    if (valid_heaps[memory_type.heapIndex] &&
        (memory_type.propertyFlags & properties) == properties &&
        (1 << i & memory_types)) {
      return i;
    }
  }

  throw VulkanException("cant find needed memory type");
}

} // namespace vk
