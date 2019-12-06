all : memoryAllocator.out testSPHeap.out testOneBin.out

memoryAllocator.out: spHeap.o oneBin.o memComparisons.o helpers.o
	gcc -o memoryAllocator.out memComparisons.o spHeap.o oneBin.o helpers.o -lm

testSPHeap.out:  testSPHeap.o spHeap.o helpers.o
	gcc -o testSPHeap.out testSPHeap.o spHeap.o helpers.o -lm

testOneBin.out:  testOneBin.o oneBin.o helpers.o
	gcc -o testOneBin.out testOneBin.o oneBin.o helpers.o -lm

# now for the dependancies for the above executables
testSPHeap.o: testSPHeap.c spHeap.h spHeapTester.h
	gcc -c testSPHeap.c

spHeap.o: spHeap.c spHeap.h spHeapTester.h
	gcc -c spHeap.c

testOneBin.o: testOneBin.c oneBin.h oneBinTester.h
	gcc -c testOneBin.c

oneBin.o: oneBin.c oneBin.h oneBinTester.h
	gcc -c oneBin.c

memComparisons.o: memComparisons.c oneBin.h spHeapTester.h
	gcc -c memComparisons.c

helpers.o: helpers.c helpers.h
	gcc -c helpers.c
