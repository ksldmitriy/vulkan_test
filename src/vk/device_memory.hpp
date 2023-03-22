#pragma once
#include "buffer.hpp"
#include "device.hpp"

using namespace std;

namespace vk {

class DeviceMemory {
private:
  struct MemoryRange {
	bool empty;
	VkDeviceSize offset;
	VkDeviceSize size;
 };

  VkDevice host_device;
  VkDeviceMemory handle;
  VkDeviceSize size;
  vector<MemoryRange> memory_ranges;

  uint32_t FindSuitableRange(VkDeviceSize size, VkDeviceSize alignment);
  VkDeviceSize GetAlignedPos(VkDeviceSize pos, VkDeviceSize alignment);
public:
  DeviceMemory(VkDevice device, VkDeviceSize size, uint32_t type);
  DeviceMemory(DeviceMemory &) = delete;
  DeviceMemory &operator=(DeviceMemory &) = delete;
  ~DeviceMemory();

  void BindBuffer(Buffer &buffer);
  void Free();
};

} // namespace vk
