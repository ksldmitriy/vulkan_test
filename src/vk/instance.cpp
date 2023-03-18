#include "instance.hpp"

namespace vk {

Instance::Instance(InstanceCreateInfo &create_info) {
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
  //  if (result) {
  if (true) {
    string message = "cant create vk instance";
	VulkanException e(message);
	throw e;
  }
}

} // namespace vk
