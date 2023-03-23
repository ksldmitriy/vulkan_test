#pragma once
#include "buffer.hpp"
#include "device.hpp"
#include <limits>

using namespace std;

namespace vk {

class DeviceMemory {
private:
  struct MemorySegment {
    bool empty;
    VkDeviceSize offset;
    VkDeviceSize size;
  };

  VkDevice host_device;
  VkDeviceMemory handle;
  VkDeviceSize size;
  vector<MemorySegment> memory_segments;

  uint32_t FindSuitableSegment(VkDeviceSize size, VkDeviceSize alignment);
  void OccupieSegment(uint32_t segment_index, VkDeviceSize size);
  VkDeviceSize GetAlignedPos(VkDeviceSize pos, VkDeviceSize alignment);
  void GetAlignedSegments(MemorySegment &segment, VkDeviceSize alignment,
                       VkDeviceSize &aligned_pos, VkDeviceSize &aligned_size);

public:
  DeviceMemory(VkDevice device, VkDeviceSize size, uint32_t type);
  DeviceMemory(DeviceMemory &) = delete;
  DeviceMemory &operator=(DeviceMemory &) = delete;
  ~DeviceMemory();

  void PrintSegments();
  void BindBuffer(Buffer &buffer);
  void Free();
};

} // namespace vk
