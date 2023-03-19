#include "vulkan_application.hpp"

void VulkanApplication::Run() {
  // create instance
  vk::InstanceCreateInfo instance_create_info;
  instance = unique_ptr<vk::Instance>(new vk::Instance(instance_create_info));

  // create device
  vk::PhysicalDevice &physical_device = *instance->physical_devices[0];

  vk::DeviceCreateInfo device_create_info;
  device_create_info.features = {};
  device_create_info.queue_family = physical_device.ChooseQueueFamily(0);

  device = unique_ptr<vk::Device>(
      new vk::Device(physical_device, device_create_info));
}
