#pragma once
#include "vk/instance.hpp"
#include <memory>

using namespace std;

class VulkanApplication {
private:
  unique_ptr<vk::Instance> instance;
public:
  VulkanApplication() = default;
  void Run();
};
