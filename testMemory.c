#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "spHeap.h"
#include "spHeapTester.h"

int main(){
    printf("Hello, World!\nsizeof(int)=%d\n",sizeof(int));
    //doing some pointer arithmetic
    int a;
    printf("\na+0:%p",&a);
    printf("\na+1:%p",&a+1);
    printf("\na+2:%p",&a+2);

    void* b = &a;
    printf("\nb+0:%p",b+0);
    printf("\nb+1:%p",b+1);
    printf("\nb+2:%p",b+2);

    for (int i = 0; i < 257; ++i) {
        int heapSizeInMB =  i;
        int logHeapSize = (int) ceil(log2((int) heapSizeInMB));
        printf("heapSizeInMB= %d, logHeapSize=%d\n",heapSizeInMB,logHeapSize);
    }
    //Checking correctedSize function
    for (int i = 0; i < 257; ++i) {
        int correct_size = correctedSize(i);
        printf("memory_request= %d, correct_size=%d\n",i,correct_size);
    }
    //Checking bucket_num function
    for (int i = 0; i < 257; ++i) {
        int correct_size = correctedSize(i);
        int bucket_number = bucket_num(correct_size);
        printf("memory_request= %d, correct_size=%d, bucket_num = %d,\n",i,correct_size,bucket_number);
    }
    //Checking get_bucket_size function
    for (int i = 0; i < 56; ++i) {
        int bucket_num = i;
        int bucket_sizeInB = get_bucket_size(i);
        printf("bucket_num= %d, bucket_sizeInB=%d\n",bucket_num,bucket_sizeInB);
    }
    //Checking Memory Initialization
    spHeap* heap1 = initializeMemory(10);
    printHeap(heap1);
    spHeap* heap2 = initializeMemory(100);
    printHeap(heap2);
    spHeap* heap3 = initializeMemory(1000);
    printHeap(heap3);
    // Below you can note how, though we requested only 100B, anything between 4B and 128B is accepted as request.
    // This is because 100 is initialized to bucket 128B.
    for (int i = -10; i < 150; ++i) {
        BucketBlock* spaceAvlBucket = checkSpaceAvailableBucket(heap2,i);
        printf("Requested %d bytes. Space is ",i);
        if(!spaceAvlBucket){            printf("not available in Heap2\n");continue;        }
        printf("available in bucket %d of Heap 2\n",spaceAvlBucket->bucket_num);
    }
    for (int i = -10; i < 20; ++i) {
        BucketBlock* some_mem =allocateMemory(heap2,i);
        if(some_mem){
            printf("\n\nRequested Memory Size = %d, obtained Pointer = %p",i,some_mem->block->mem_address);
            printHeap(heap2);
        }
    }
    for (int i = 0; i < 40; ++i) {
        BucketBlock* some_mem =allocateMemory(heap3,i);
        if(some_mem){
            printf("\n\nRequested Memory Size = %d, obtained Pointer = %p",i,some_mem->block->mem_address);
            printHeap(heap3);
        }
    }
    //Repeated Requests of Same Size
    spHeap* heap4 = initializeMemory(1000);
    printHeap(heap4);
    int num_Allocs = 45;
    BucketBlock** bucketsAllocated = calloc(num_Allocs, sizeof(BucketBlock*));
    for (int i = 0; i < num_Allocs; ++i) {
        bucketsAllocated[i] = allocateMemory(heap4,i);
        if(bucketsAllocated[i]){
            printf("\n\nRequested Memory Size = %d, obtained Pointer = %p",i,bucketsAllocated[i]->block->mem_address);
            printHeap(heap4);
        }
    }
    //Free memory check
    for (int i = 0; i < num_Allocs; ++i) {
        if(bucketsAllocated[i]){
            printf(""
                   "---------------------------------\n"
                   "The Block %d to be freed is as below\n",i);
            printMemBlock(bucketsAllocated[i]->block);
            printf("\n---------------------------------\n");
            freeMemory(heap4, bucketsAllocated[i]);
            printHeap(heap4);
        } else {
            printf("Sorry the bucket: %d has not been allocated\n",i);
        }
    }

    //check some inistializations
    spHeap* heap5 = initializeMemory(100);
    typedef struct somestruct{
        int a;
        float f;
    }someStruct;

    BucketBlock* somemem = allocateMemory(heap5, sizeof(someStruct));
    someStruct* ss = somemem->block->mem_address;
    ss->a = 5;
    ss->f = 2.2;
    printf("\nss->a=%d,ss->f = %.2f\n",ss->a,ss->f);
    freeMemory(heap5,somemem);
    printHeap(heap5);






}