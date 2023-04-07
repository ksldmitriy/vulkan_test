#!/bin/bash
echo === CMAKE ===

cmake -S ./src -B ./out

echo === BUILD ===
make -C ./out;

echo === SHADERS ===
glslc shaders/test.comp -o shaders/test.spv
glslc shaders/test.vert -o shaders/test_v.spv
glslc shaders/test.frag -o shaders/test_f.spv

echo === RUN ===
./out/vulkan_test

