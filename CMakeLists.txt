cmake_minimum_required(VERSION 3.15)
project(WeightedBuddyMemoryAllocator C)

set(CMAKE_C_STANDARD 99)

add_executable(WeightedBuddyMemoryAllocator spHeap.c spHeap.h testMemory.c)
