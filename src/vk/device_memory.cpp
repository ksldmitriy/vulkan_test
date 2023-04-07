#include "device_memory.hpp"

namespace vk {

DeviceMemory::DeviceMemory(Device &device, VkDeviceSize size, uint32_t type) {
  this->device = device.GetHandle();
  this->size = size;

  VkPhysicalDeviceLimits device_limits = device.GetPhysicalDevice().GetLimits();
  non_coherent_atom_size = device_limits.nonCoherentAtomSize;

  VkMemoryAllocateInfo vk_allocate_info;
  vk_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  vk_allocate_info.pNext = nullptr;
  vk_allocate_info.allocationSize = size;
  vk_allocate_info.memoryTypeIndex = type;

  VkResult result =
      vkAllocateMemory(this->device, &vk_allocate_info, nullptr, &handle);
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

VkDeviceSize DeviceMemory::CalculateMemorySize(vector<Buffer *> buffers) {
  VkDeviceSize size = 0;

  for (int i = 0; i < buffers.size(); i++) {
    VkMemoryRequirements requirements = buffers[i]->GetMemoryRequirements();
    size += requirements.alignment;
    size += requirements.size;
  }

  return size;
}

void *DeviceMemory::MapMemory(VkBuffer buffer) {
  uint32_t segment_index = FindSegment(buffer);
  MemorySegment &buffer_segment = memory_segments[segment_index];

  mapped_segment = CreateAlignedMemorySegment(buffer_segment);

  void *mapped_ptr;
  VkResult result = vkMapMemory(device, handle, mapped_segment.offset,
                                mapped_segment.size, 0, &mapped_ptr);
  if (result) {
    throw VulkanException("cant map memory");
  }

  return mapped_ptr;
}

DeviceMemory::MemorySegment
DeviceMemory::CreateAlignedMemorySegment(MemorySegment &buffer_segment) {
  MemorySegment aligned_segment;

  // align pos
  aligned_segment.offset =
      tools::align_down(buffer_segment.offset, non_coherent_atom_size);
  aligned_segment.offset = max(aligned_segment.offset, (VkDeviceSize)0);

  VkDeviceSize aligned_size =
      buffer_segment.size + (buffer_segment.offset - aligned_segment.offset);

  // align size
  aligned_segment.size = tools::align_up(aligned_size, non_coherent_atom_size);
  aligned_segment.size =
      min(aligned_segment.size, size - aligned_segment.offset);

  return aligned_segment;
}

void DeviceMemory::Flush() {
  VkMappedMemoryRange mapped_range;
  mapped_range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
  mapped_range.pNext = nullptr;
  mapped_range.memory = handle;
  mapped_range.offset = mapped_segment.offset;
  mapped_range.size = mapped_segment.size;

  VkResult result = vkFlushMappedMemoryRanges(device, 1, &mapped_range);
  if (result) {
    throw VulkanException("cant flush mapped memory");
  }
}

void DeviceMemory::Unmap() { vkUnmapMemory(device, handle); };

void DeviceMemory::Free() {
  if (handle) {
    vkFreeMemory(device, handle, nullptr);
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
  OccupieSegment(segment_index, requirements.size, buffer.handle);

  VkResult result = vkBindBufferMemory(device, buffer.GetHandle(), handle, pos);
  if (result) {
    throw VulkanException("cant bind buffer to memory");
  }

  buffer.memory = this;
  buffer.is_binded = true;
}

void DeviceMemory::FreeBuffer(VkBuffer buffer) {
  uint32_t segment_index = FindSegment(buffer);
  FreeSegment(segment_index);
};

void DeviceMemory::FreeSegment(uint32_t segment_index) {
  MemorySegment &segment = memory_segments[segment_index];
  segment.empty = true;

  // merge left segment
  if (segment_index > 0) {
    if (memory_segments[segment_index - 1].empty) {
      MergeSegment(segment_index - 1, segment_index);
      segment_index--;
    }
  }

  // merge right segment
  if (segment_index < memory_segments.size() - 1) {
    if (memory_segments[segment_index + 1].empty) {
      MergeSegment(segment_index, segment_index + 1);
    }
  }
}

void DeviceMemory::MergeSegment(uint32_t segment1_index,
                                uint32_t segment2_index) {
  MemorySegment &segment1 = memory_segments[segment1_index];
  MemorySegment &segment2 = memory_segments[segment2_index];

  MemorySegment merged_segment;
  merged_segment.empty = true;
  merged_segment.offset = min(segment1.offset, segment2.offset);
  merged_segment.size = segment1.size + segment2.size;

  memory_segments[segment1_index] = merged_segment;
  memory_segments.erase(memory_segments.begin() + segment2_index);
}

uint32_t DeviceMemory::FindSegment(VkBuffer buffer) {
  for (int i = 0; i < memory_segments.size(); i++) {
    MemorySegment &segment = memory_segments[i];

    if (segment.buffer == buffer) {
      return i;
    }
  }

  throw VulkanException("no suck buffer binded to device memory");
}

void DeviceMemory::OccupieSegment(uint32_t segment_index, VkDeviceSize size,
                                  VkBuffer buffer) {
  MemorySegment initial_segment = memory_segments[segment_index];

  MemorySegment occupied_segment;
  occupied_segment.empty = false;
  occupied_segment.offset = initial_segment.offset;
  occupied_segment.size = size;
  occupied_segment.buffer = buffer;

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
  aligned_pos = tools::align_up(segment.offset, alignment);
  aligned_size = segment.size - aligned_pos + segment.offset;
}

} // namespace vk
