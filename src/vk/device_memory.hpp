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
	VkBuffer buffer;
  };

  VkDevice host_device;
  VkDeviceMemory handle;
  VkDeviceSize size;
  vector<MemorySegment> memory_segments;

  void FreeBuffer(VkBuffer buffer);
  void FreeSegment(uint32_t segment_index);
  void MergeSegment(uint32_t segment1_index, uint32_t segment2_index);
  void OccupieSegment(uint32_t segment_index, VkDeviceSize size, VkBuffer buffer);
  uint32_t FindSuitableSegment(VkDeviceSize size, VkDeviceSize alignment);
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

  friend Buffer;
};

} // namespace vk
