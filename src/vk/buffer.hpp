#pragma once
#include "exception.hpp"
#include <vulkan/vulkan.h>

using namespace std;

namespace vk {

class DeviceMemory;

struct BufferCreateInfo {
  VkDeviceSize size;
  VkBufferUsageFlags usage;
  uint32_t queue;
};

class Buffer {
private:
  VkDevice device;
  VkBuffer handle;
  DeviceMemory *memory;
  bool is_binded;

  VkDeviceSize size;
  VkMemoryRequirements requirements;

public:
  Buffer(VkDevice device, BufferCreateInfo &create_info);
  Buffer(Buffer &) = delete;
  Buffer &operator=(Buffer &) = delete;
  ~Buffer();

  void *Map();
  void Flush();
  void Unmap();
  VkBuffer GetHandle();
  void Destroy();
  VkMemoryRequirements GetMemoryRequirements();
  uint32_t GetMemoryTypes();
  VkDeviceSize GetSize();

  friend DeviceMemory;
};

} // namespace vk
