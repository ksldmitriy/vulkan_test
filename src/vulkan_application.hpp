#pragma once
#include "vk/instance.hpp"
#include "vk/device.hpp"
#include <memory>

using namespace std;

class VulkanApplication {
private:
  unique_ptr<vk::Instance> instance;
  unique_ptr<vk::Device> device;
public:
  VulkanApplication() = default;
  void Run();
};
