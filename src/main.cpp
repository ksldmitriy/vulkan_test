#include "main.hpp"
#include "vk/instance.hpp"

int main() {
  ComputeVulkanApplication application;

  try {
    application.Run();
  } catch (Exception &e) {
    cout << (string)e << endl;
  };

  cout << "aeee" << endl;
}
