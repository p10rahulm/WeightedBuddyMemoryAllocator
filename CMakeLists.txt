cmake_minimum_required(VERSION 3.15)
project(WeightedBuddyMemoryAllocator C)

set(CMAKE_C_STANDARD 99)

add_executable(testSPHeap spHeap.c spHeap.h spHeapTester.h testSPHeap.c helpers.c)
add_executable(testOneBin oneBin.c oneBin.h testOneBin.c oneBinTester.h helpers.c)
add_executable(memComparisons memComparisons.c spHeap.c spHeap.h oneBin.h oneBin.c helpers.c)
