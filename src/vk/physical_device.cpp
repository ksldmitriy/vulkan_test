#include "physical_device.hpp"

namespace vk {
PhysicalDevice::PhysicalDevice(VkPhysicalDevice handle) {
  this->handle = handle;
  vkGetPhysicalDeviceProperties(handle, &properties);
}

} // namespace vk
