#include "main.hpp"
#include "vk/instance.hpp"

int main() {
  VulkanApplication application;

  try {
    application.Run();
  } catch (Exception &e) {
    cout << (string)e << endl;
  };

  cout << "aeee" << endl;
}
