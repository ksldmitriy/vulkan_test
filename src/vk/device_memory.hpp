#pragma once
#include "device.hpp"
#include "buffer.hpp"

using namespace std;

namespace vk {

class DeviceMemory {
private:
  VkDevice host_device;
  VkDeviceMemory handle;
  VkDeviceSize size;

  VkDeviceSize FindPlaceForBuffer(VkDeviceSize size, VkDeviceSize alignment);
public:
  DeviceMemory(VkDevice device, VkDeviceSize size, uint32_t type);
  DeviceMemory(DeviceMemory &) = delete;
  DeviceMemory &operator=(DeviceMemory &) = delete;
  ~DeviceMemory();

  void BindBuffer(Buffer& buffer);
  void Free();
};

} // namespace vk
