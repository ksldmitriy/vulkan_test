#pragma once
#include "device.hpp"

using namespace std;

namespace vk {

class DeviceMemory {
private:
  VkDevice host_device;
  VkDeviceMemory handle;

  DeviceMemory(VkDevice device, VkDeviceSize size, uint32_t type);

public:
  DeviceMemory(DeviceMemory &) = delete;
  DeviceMemory &operator=(DeviceMemory &) = delete;

  friend Device;
};

} // namespace vk
