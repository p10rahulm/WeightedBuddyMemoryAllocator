all : memoryAllocator.out testSPHeap.out testOneBin.out

memoryAllocator.out: spHeap.o oneBin.o memComparisons.o
	gcc -o memoryAllocator.out memComparisons.o spHeap.o oneBin.o

testSPHeap.out:  testSPHeap.o spHeap.o
	gcc -o testSPHeap.out testSPHeap.o spHeap.o

testOneBin.out:  testOneBin.o oneBin.o
	gcc -o testOneBin.out testOneBin.o oneBin.o

# now for the dependancies for the above executables
testSPHeap.o: testSPHeap.c spHeap.h spHeapTester.h
	gcc -c testSPHeap.c

spHeap.o: spHeap.c spHeap.h
	gcc -c spHeap.c

testOneBin.o: testOneBin.c oneBin.h
	gcc -c testOneBin.c

oneBin.o: oneBin.c oneBin.h
	gcc -c oneBin.c

memComparisons.o: memComparisons.c oneBin.h spHeapTester.h
	gcc -c memComparisons.c