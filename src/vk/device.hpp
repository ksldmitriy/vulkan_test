#pragma once
#include "exception.hpp"
#include "physical_device.hpp"
#include "tools.hpp"

using namespace std;

namespace vk {

class DeviceMemory;

struct DeviceCreateInfo {
  vector<string> extensions;
  vector<string> layers;
  uint32_t queue_family;
  VkPhysicalDeviceFeatures features;
};

class Device {
private:
  VkDevice handle;

public:
  Device(PhysicalDevice &physical_device, DeviceCreateInfo &create_info);
  Device(Device &) = delete;
  Device &operator=(Device &) = delete;

  VkDevice GetHandle();
};

} // namespace vk
