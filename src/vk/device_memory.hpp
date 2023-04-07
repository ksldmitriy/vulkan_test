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

  VkDevice device;
  VkDeviceMemory handle;
  VkDeviceSize size;
  VkDeviceSize non_coherent_atom_size;
  vector<MemorySegment> memory_segments;

  MemorySegment mapped_segment;

  void FreeBuffer(VkBuffer buffer);
  uint32_t FindSegment(VkBuffer buffer);
  void FreeSegment(uint32_t segment_index);
  void MergeSegment(uint32_t segment1_index, uint32_t segment2_index);

  void OccupieSegment(uint32_t segment_index, VkDeviceSize size,
                      VkBuffer buffer);
  uint32_t FindSuitableSegment(VkDeviceSize size, VkDeviceSize alignment);
  void GetAlignedSegments(MemorySegment &segment, VkDeviceSize alignment,
                          VkDeviceSize &aligned_pos,
                          VkDeviceSize &aligned_size);

  void *MapMemory(VkBuffer buffer);
  void Flush();
  void Unmap();
  MemorySegment CreateAlignedMemorySegment(MemorySegment& buffer_segment);

public:
  DeviceMemory(Device& device, VkDeviceSize size, uint32_t type);
  DeviceMemory(DeviceMemory &) = delete;
  DeviceMemory &operator=(DeviceMemory &) = delete;
  ~DeviceMemory();

  static VkDeviceSize CalculateMemorySize(vector<Buffer *> buffers);
  void PrintSegments();
  void BindBuffer(Buffer &buffer);
  void Free();

  friend Buffer;
};

} // namespace vk
