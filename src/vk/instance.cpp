#include "instance.hpp"

namespace vk {

Instance::Instance(InstanceCreateInfo &create_info) {
  CreateInstance(create_info);
  EnumPhysicalDevices();
}

void Instance::CreateInstance(InstanceCreateInfo &create_info) {
  create_info.layers.push_back("VK_LAYER_KHRONOS_validation");
  create_info.extensions.push_back("VK_EXT_debug_utils");

  VkInstanceCreateInfo vk_create_info;
  vk_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  vk_create_info.pNext = nullptr;
  vk_create_info.flags = 0;
  vk_create_info.pApplicationInfo = 0;

  vector<const char *> layers_names_pp =
      tools::string_vector_to_c_array(create_info.layers);
  vk_create_info.enabledLayerCount = layers_names_pp.size();
  vk_create_info.ppEnabledLayerNames = layers_names_pp.data();

  vector<const char *> extensions_names_pp =
      tools::string_vector_to_c_array(create_info.extensions);
  vk_create_info.enabledExtensionCount = extensions_names_pp.size();
  vk_create_info.ppEnabledExtensionNames = extensions_names_pp.data();

  VkResult result = vkCreateInstance(&vk_create_info, nullptr, &handle);
  if (result) {
    throw VulkanException("cant create vk instance");
  }
}

void Instance::EnumPhysicalDevices() {
  vector<VkPhysicalDevice> raw_devices;

  uint32_t count;
  vkEnumeratePhysicalDevices(handle, &count, nullptr);

  raw_devices.resize(count);
  physical_devices.resize(count);
  vkEnumeratePhysicalDevices(handle, &count, raw_devices.data());

  for (int i = 0; i < count; i++) {
    physical_devices[i] =
        unique_ptr<PhysicalDevice>(new PhysicalDevice(raw_devices[i]));
  }
}

} // namespace vk
