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
  VkDeviceSize size;
  VkMemoryRequirements requirements;

public:
  Buffer(VkDevice device, BufferCreateInfo &create_info);

  VkBuffer GetHandle();
  VkMemoryRequirements GetMemoryRequirements();
  uint32_t GetMemoryTypes();
  VkDeviceSize GetSize();
};

} // namespace vk
