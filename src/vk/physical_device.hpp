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
  uint32_t ChooseQueueFamily(VkQueueFlags requirements);
  friend class Device;
};

} // namespace vk
