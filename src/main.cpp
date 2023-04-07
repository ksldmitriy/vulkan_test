#include "main.hpp"

int main() {
  GraphicsVulkanApplication application;

  try {
    application.Run();
  } catch (Exception &e) {
    cout << (string)e << endl;
  };

  cout << "aeee" << endl;
}
