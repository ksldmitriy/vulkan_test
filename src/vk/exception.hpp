#pragma once
#include "../exception.hpp"

using namespace std;

class VulkanException : Exception {
private:
  string message;

public:
  VulkanException(string message) { this->message = message; }
  operator string() { return message; }
};
