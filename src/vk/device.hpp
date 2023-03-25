#pragma once
#include "exception.hpp"
#include "physical_device.hpp"
#include "tools.hpp"

using namespace std;

namespace vk {

class DeviceMemory;

struct DeviceCreateInfo {
  vector<string> extensions;
  vector<string> layers;
  uint32_t queue_family;
  VkPhysicalDeviceFeatures features;
};

class Device {
private:
  VkDevice handle;
  shared_ptr<PhysicalDevice> physical_device;
  VkQueue queue;
  uint32_t queue_family;

public:
  Device(shared_ptr<PhysicalDevice> physical_device,
         DeviceCreateInfo &create_info);
  Device(Device &) = delete;
  Device &operator=(Device &) = delete;

  PhysicalDevice &GetPhysicalDevice();
  VkDevice GetHandle();
  uint32_t ChooseQueueFamily(VkQueueFlags requirements);
  uint32_t ChooseMemoryType(VkMemoryPropertyFlags properties,
                            VkMemoryHeapFlags heap_properties,
                            uint32_t memory_types);
  VkQueue GetQueue();
  uint32_t GetQueueFamily();
};

} // namespace vk
