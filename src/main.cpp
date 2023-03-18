#include "main.hpp"
#include "vk/instance.hpp"

int main() {
  try {
    vk::InstanceCreateInfo instance_create_info;
    vk::Instance instance(instance_create_info);
  } catch (Exception &e) {
    cout << (string)e << endl;
  }

  cout << "aeee" << endl;
}
