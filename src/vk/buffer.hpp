#pragma once
#include "exception.hpp"
#include <vulkan/vulkan.h>

using namespace std;

namespace vk {

struct BufferCreateInfo {
  VkDeviceSize size;
  VkBufferUsageFlags usage;
  uint32_t queue;
};

class Buffer {
private:
  VkBuffer handle;

  Buffer(VkDevice device, BufferCreateInfo& create_info);

public:

  friend class DeviceMemory;
};

} // namespace vk
