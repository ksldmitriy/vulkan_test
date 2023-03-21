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

  DeviceMemory(VkDevice device, VkDeviceSize size, uint32_t type);

public:
  DeviceMemory(DeviceMemory &) = delete;
  DeviceMemory &operator=(DeviceMemory &) = delete;
  ~DeviceMemory();

  unique_ptr<Buffer> CreateBuffer(BufferCreateInfo& create_info);
  void Free();
 
  friend Device;
};

} // namespace vk
