#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "tools.hpp"
#include "exception.hpp"

using namespace std;

namespace vk {

struct InstanceCreateInfo {
  vector<string> layers;
  vector<string> extensions;
};

class Instance {
private:
  VkInstance handle;

public:
  Instance(InstanceCreateInfo &create_info);
};

} // namespace vk
