#include "templates.hpp"

namespace vk {

VkBufferMemoryBarrier buffer_barrier_template = {
    .sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
    .pNext = nullptr,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .offset = 0,
    .size = VK_WHOLE_SIZE};

}
