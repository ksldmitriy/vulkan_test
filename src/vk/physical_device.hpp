#pragma once

#include "exception.hpp"
#include <vector>
#include <vulkan/vulkan.h>

using namespace std;

namespace vk {

class PhysicalDevice {
private:
  VkPhysicalDevice handle;
  VkPhysicalDeviceProperties properties;
  VkPhysicalDeviceMemoryProperties memory_properties;
  vector<VkQueueFamilyProperties> queue_families_properties;

public:
  PhysicalDevice(VkPhysicalDevice handle);
  PhysicalDevice(PhysicalDevice &) = delete;
  PhysicalDevice &operator=(PhysicalDevice &) = delete;

  VkPhysicalDevice GetHandle();
  uint32_t ChooseQueueFamily(VkQueueFlags requirements);
  uint32_t ChooseMemoryType(VkMemoryPropertyFlags properties,
                            VkMemoryHeapFlags heap_properties,
                            uint32_t memory_types);
};

} // namespace vk
