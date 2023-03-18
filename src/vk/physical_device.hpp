#pragma once
#include <vector>
#include <vulkan/vulkan.h>

using namespace std;

namespace vk {

class PhysicalDevice {
private:
  VkPhysicalDevice handle;
  VkPhysicalDeviceProperties properties;
public:
  PhysicalDevice(VkPhysicalDevice handle);
};

} // namespace vk
