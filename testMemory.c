#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "spHeap.h"

int main(){
    printf("Hello, World!\nsizeof(int)=%d\n",sizeof(int));
    int a;
    printf("\na+0:%p",&a);
    printf("\na+1:%p",&a+1);
    printf("\na+2:%p",&a+2);

    for (int i = 0; i < 1025; ++i) {
        int heapSizeInMB =  i;
        int logHeapSize = (int) ceil(log2((int) heapSizeInMB));
        printf("heapSizeInMB= %d, logHeapSize=%d\n",heapSizeInMB,logHeapSize);
    }
    for (int i = 0; i < 1025; ++i) {
        int correct_size = correctedSize(i);
        printf("memory_request= %d, correct_size=%d\n",i,correct_size);
    }

    for (int i = 0; i < 1025; ++i) {
        int correct_size = correctedSize(i);
        int bucket_number = bucket_num(correct_size);
        printf("memory_request= %d, correct_size=%d, bucket_num = %d,\n",i,correct_size,bucket_number);
    }
    for (int i = 0; i < 56; ++i) {
        int bucket_num = i;
        int bucket_sizeInB = get_bucket_size(i);
        printf("bucket_num= %d, bucket_sizeInB=%d\n",bucket_num,bucket_sizeInB);
    }
    spHeap* heap1 = initializeMemory(10);
    printHeap(heap1);
    spHeap* heap2 = initializeMemory(100);
    printHeap(heap2);
    spHeap* heap3 = initializeMemory(1000);
    printHeap(heap3);




}