#pragma once
#include "vk/device.hpp"
#include "vk/device_memory.hpp"
#include "vk/instance.hpp"
#include <memory>
#include <cstring>
#include <fstream>

using namespace std;

class ComputeVulkanApplication {
private:
  unique_ptr<vk::Instance> instance;
  unique_ptr<vk::Device> device;

  uint32_t queue_family;

  unique_ptr<vk::DeviceMemory> memory;
  vector<unique_ptr<vk::Buffer>> buffers;

  VkCommandPool command_pool;
  VkCommandBuffer command_buffer;

  VkShaderModule shader_module;
  VkPipeline pipeline;
  VkPipelineLayout pipeline_layout;
  VkDescriptorPool descriptor_pool;
  VkDescriptorSetLayout descriptor_set_layout;
  VkDescriptorSet descriptor_set;
  
  void InitVulkan();
  void CreateBuffers();
  void CreateCommandPool();
  void CreateCommandBuffer();
  void FillCommandBuffer();
  void RunQueue();
  void CreateShaderModule();
  void CreatePipeline();
public:
  ComputeVulkanApplication() = default;
  void Run();
};
