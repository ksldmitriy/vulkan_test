#pragma once
#include "vk/device.hpp"
#include "vk/device_memory.hpp"
#include "vk/instance.hpp"
#include "vk/templates.hpp"
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstring>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace std;

struct Vertex {
  glm::fvec3 pos;
  glm::fvec3 color;
};

struct UniformData {
  glm::fmat4 transformation_matrix;
};

class GraphicsVulkanApplication {
private:
  unique_ptr<vk::Instance> instance;
  unique_ptr<vk::Device> device;

  GLFWwindow *window;
  VkSurfaceKHR window_surface;

  uint32_t queue_family;

  VkCommandPool command_pool;
  VkShaderModule vertex_shader_module, fragment_shader_module;
  VkSemaphore image_ready_semaphore, render_finished_semaphore;

  unique_ptr<vk::DeviceMemory> vertex_buffer_memory;
  unique_ptr<vk::DeviceMemory> uniform_buffer_memory;

  unique_ptr<vk::Buffer> vertex_buffer, index_buffer, uniform_buffer;
  VkVertexInputBindingDescription vertex_binding_description;
  VkVertexInputAttributeDescription vertex_attribute_desctiptions[2];

  VkSwapchainKHR swapchain = VK_NULL_HANDLE;
  VkSurfaceFormatKHR surface_format;
  VkExtent2D swapchain_extent;
  vector<VkImage> swapchain_images;
  vector<VkImageView> swapchain_image_views;
  vector<VkFramebuffer> swapchain_framebuffers;

  VkVertexInputBindingDescription vertex_binding_descriptor;
  VkDescriptorPool descriptor_pool;

  VkDescriptorSetLayout descriptor_set_layout;
  VkPipelineLayout pipeline_layout;

  VkPipeline pipeline;

  VkRenderPass render_pass;

  void CreateWindow();
  void Prepare();
  void RunRenderLoop();
  void InitVulkan();
  void CreateWindowSurface();
  void CreateCommandPool();
  void CreateShaderModule(string path, VkShaderModule &shader_module);
  void CreateSemaphores();
  void CreateVertexBuffer();
  void CreateUniformBuffer();
  void UpdateUniformBuffer();
  void CreateRenderPass();
  void CreateImagesView();
  void CreateFramebuffer();
  void CreateGraphicsPipeline();
  void CreateDescriptorPool();
  void CreateDescriptorSet();
  void CreateCommandBuffer();
  void CreateSwapchain();
  VkSurfaceFormatKHR
  ChooseSurfaceFormat(vector<VkSurfaceFormatKHR> &supported_formats);
  VkPresentModeKHR
  ChoosePresentMode(vector<VkPresentModeKHR> &supported_present_modes);
  VkSurfaceTransformFlagBitsKHR
  ChooseSurfaceTransform(VkSurfaceCapabilitiesKHR surface_capabilities);
  VkExtent2D
  ChooseSwapchainExtend(VkSurfaceCapabilitiesKHR surface_capabilities);

public:
  void Run();
};
