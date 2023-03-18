#!/bin/bash
echo === CMAKE ===

cmake -S ./src -B ./out

echo === BUILD ===
make -C ./out;

echo === RUN ===
./out/vulkan_test

