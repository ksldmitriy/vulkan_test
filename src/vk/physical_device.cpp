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

uint32_t PhysicalDevice::ChooseQueueFamily(VkQueueFlags requirements) {
  for (uint32_t i = 0; i < queue_families_properties.size(); i++) {
    if ((queue_families_properties[i].queueFlags & requirements) ==
        requirements) {
      return i;
    }
  }

  throw VulkanException("cant find needed queue");
};

} // namespace vk
