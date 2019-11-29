all : memoryAllocator.out

memoryAllocator.out:  testMemory.o spHeap.o
	gcc -o memoryAllocator.out testMemory.o spHeap.o

# now for the dependancies for the above executables
testMemory.o: testMemory.c spHeap.h
	gcc -c testMemory.c

spHeap.c: spHeap.c spHeap.h
	gcc -c spHeap.c
