cmake_minimum_required(VERSION 3.13.4)

project(vulkan_test)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)

#project
add_executable(${PROJECT_NAME} main.cpp vulkan_application.cpp vk/device_memory.cpp vk/device.cpp vk/instance.cpp vk/tools.cpp vk/physical_device.cpp)

# libs

link_directories(/usr/lib)

target_link_libraries(vulkan_test libvulkan.so)


