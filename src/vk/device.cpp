#include "device.hpp"
#include "device_memory.hpp"

namespace vk {

Device::Device(shared_ptr<PhysicalDevice> physical_device,
               DeviceCreateInfo &create_info) {
  this->physical_device = physical_device;

  // queue create info
  VkDeviceQueueCreateInfo vk_queue_create_info;
  vk_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  vk_queue_create_info.pNext = nullptr;
  vk_queue_create_info.flags = 0;
  vk_queue_create_info.queueCount = 1;
  vk_queue_create_info.queueFamilyIndex = create_info.queue_family;
  float queue_priority = 0;
  vk_queue_create_info.pQueuePriorities = &queue_priority;

  // device create info
  VkDeviceCreateInfo vk_create_info;
  vk_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  vk_create_info.pNext = nullptr;
  vk_create_info.flags = 0;

  vk_create_info.queueCreateInfoCount = 1;
  vk_create_info.pQueueCreateInfos = &vk_queue_create_info;

  vector<const char *> layers_names_pp =
      tools::string_vector_to_c_array(create_info.layers);
  vk_create_info.enabledLayerCount = layers_names_pp.size();
  vk_create_info.ppEnabledLayerNames = layers_names_pp.data();

  vector<const char *> extensions_names_pp =
      tools::string_vector_to_c_array(create_info.extensions);
  vk_create_info.enabledExtensionCount = extensions_names_pp.size();
  vk_create_info.ppEnabledExtensionNames = extensions_names_pp.data();

  vk_create_info.pEnabledFeatures = &create_info.features;

  VkResult result = vkCreateDevice(physical_device->GetHandle(),
                                   &vk_create_info, nullptr, &handle);
  if (result) {
    throw VulkanException("cant create device");
  }
}

PhysicalDevice &Device::GetPhysicalDevice() { return *physical_device; }

VkDevice Device::GetHandle() { return handle; }

uint32_t Device::ChooseQueueFamily(VkQueueFlags requirements) {
  return physical_device->ChooseQueueFamily(requirements);
}

uint32_t Device::ChooseMemoryType(VkMemoryPropertyFlags properties,
                                  VkMemoryHeapFlags heap_properties,
                                  uint32_t memory_types) {
  return physical_device->ChooseMemoryType(properties, heap_properties,
                                           memory_types);
}

} // namespace vk
