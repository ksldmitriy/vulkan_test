cmake_minimum_required(VERSION 3.13.4)

project(vulkan_test)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)


#project

file(GLOB vulkan_test_SRC *.cpp vk/*.cpp)

add_executable(${PROJECT_NAME} ${vulkan_test_SRC}) 

# libs

link_directories(/usr/lib)

target_link_libraries(vulkan_test libvulkan.so)


