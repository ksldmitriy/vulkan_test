#pragma once
#include "vk/device.hpp"
#include "vk/device_memory.hpp"
#include "vk/instance.hpp"
#include <memory>

using namespace std;

class VulkanApplication {
private:
  unique_ptr<vk::Instance> instance;
  unique_ptr<vk::Device> device;

  uint32_t queue_family;

  unique_ptr<vk::DeviceMemory> memory;
  vector<unique_ptr<vk::Buffer>> buffers;

  void InitVulkan();
  void CreateBuffers();

public:
  VulkanApplication() = default;
  void Run();
};
