#include "device_memory.hpp"

namespace vk {

DeviceMemory::DeviceMemory(VkDevice device, VkDeviceSize size, uint32_t type) {
  host_device = device;
  this->size = size;

  VkMemoryAllocateInfo vk_allocate_info;
  vk_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  vk_allocate_info.pNext = nullptr;
  vk_allocate_info.allocationSize = size;
  vk_allocate_info.memoryTypeIndex = type;

  VkResult result =
      vkAllocateMemory(host_device, &vk_allocate_info, nullptr, &handle);
  if (result) {
    throw VulkanException("cant allocate memory");
  }

  MemorySegment segment;
  segment.empty = true;
  segment.offset = 0;
  segment.size = size;

  memory_segments.push_back(segment);
}

DeviceMemory::~DeviceMemory() { Free(); }

void DeviceMemory::Free() {
  if (handle) {
    vkFreeMemory(host_device, handle, nullptr);
  }
}

void DeviceMemory::PrintSegments() {
  for (auto &s : memory_segments) {
    cout << s.offset << " " << s.size << " " << (s.empty ? "empty" : "occupied")
         << endl;
  };
}

void DeviceMemory::BindBuffer(Buffer &buffer) {
  VkMemoryRequirements requirements = buffer.GetMemoryRequirements();
  uint32_t segment_index =
      FindSuitableSegment(requirements.size, requirements.alignment);

  MemorySegment &segment = memory_segments[segment_index];
  VkDeviceSize pos, size;
  GetAlignedSegments(segment, requirements.alignment, pos, size);
  OccupieSegment(segment_index, requirements.size);

  VkResult result =
      vkBindBufferMemory(host_device, buffer.GetHandle(), handle, pos);
  if (result) {
    throw VulkanException("cant bind buffer to memory");
  }
}

void DeviceMemory::OccupieSegment(uint32_t segment_index, VkDeviceSize size) {
  MemorySegment initial_segment = memory_segments[segment_index];

  MemorySegment occupied_segment;
  occupied_segment.empty = false;
  occupied_segment.offset = initial_segment.offset;
  occupied_segment.size = size;

  memory_segments[segment_index] = occupied_segment;

  MemorySegment empty_segment;
  empty_segment.empty = true;
  empty_segment.offset = initial_segment.offset + size;
  empty_segment.size = initial_segment.size - size;

  if (empty_segment.size) {
    memory_segments.insert(memory_segments.begin() + segment_index + 1,
                           empty_segment);
  }
}

uint32_t DeviceMemory::FindSuitableSegment(VkDeviceSize size,
                                           VkDeviceSize alignment) {
  int optimal_segment = -1;
  VkDeviceSize optimal_segment_size = numeric_limits<VkDeviceSize>::max();

  for (int i = 0; i < memory_segments.size(); i++) {
    MemorySegment &segment = memory_segments[i];
    if (!segment.empty) {
      continue;
    }

    VkDeviceSize aligned_pos, aligned_size;
    GetAlignedSegments(segment, alignment, aligned_pos, aligned_size);

    if (aligned_size < size) {
      continue;
    }

    if (segment.size < optimal_segment_size) {
      optimal_segment = i;
      optimal_segment_size = segment.size;
    }
  }

  if (optimal_segment == -1) {
    throw VulkanException("cant find empty memory range");
  }

  return optimal_segment;
}

void DeviceMemory::GetAlignedSegments(MemorySegment &segment,
                                      VkDeviceSize alignment,
                                      VkDeviceSize &aligned_pos,
                                      VkDeviceSize &aligned_size) {
  aligned_pos = GetAlignedPos(segment.offset, alignment);
  aligned_size = segment.size - aligned_pos + segment.offset;
}

VkDeviceSize DeviceMemory::GetAlignedPos(VkDeviceSize pos,
                                         VkDeviceSize alignment) {
  return ((pos + alignment - 1) / alignment) * alignment;
}

} // namespace vk
