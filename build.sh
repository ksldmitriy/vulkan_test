#!/bin/bash
echo === CMAKE ===

cmake -S ./src -B ./out

echo === BUILD ===
make -C ./out;

echo === SHADERS ===
glslc shaders/test.comp -o shaders/test.spv

echo === RUN ===
./out/vulkan_test

