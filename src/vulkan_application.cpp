#include "vulkan_application.hpp"

void VulkanApplication::Run(){
  vk::InstanceCreateInfo instance_create_info;
  instance = unique_ptr<vk::Instance>(new vk::Instance(instance_create_info));
  
}
