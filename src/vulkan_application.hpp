#pragma once
#include "vk/device.hpp"
#include "vk/device_memory.hpp"
#include "vk/instance.hpp"
#include <memory>
#include <cstring>

using namespace std;

class VulkanApplication {
private:
  unique_ptr<vk::Instance> instance;
  unique_ptr<vk::Device> device;

  uint32_t queue_family;

  unique_ptr<vk::DeviceMemory> memory;
  vector<unique_ptr<vk::Buffer>> buffers;

  VkCommandPool command_pool;
  VkCommandBuffer command_buffer;

  void InitVulkan();
  void CreateBuffers();
  void CreateCommandPool();
  void CreateCommandBuffer();
  void FillCommandBuffer();
  void RunQueue();

public:
  VulkanApplication() = default;
  void Run();
};
