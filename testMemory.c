#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "spHeap.h"

int main(){
    printf("Hello, World!\nsizeof(int)=%d\n",sizeof(int));


    for (int i = 0; i < 200; ++i) {
        double heapSizeInMB = i;
        int logHeapSize = (int) ceil(log2((int) heapSizeInMB));
        printf("logHeapSize=%d\n",logHeapSize);
    }


}