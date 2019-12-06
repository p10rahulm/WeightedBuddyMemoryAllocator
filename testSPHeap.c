#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "spHeap.h"
#include "spHeapTester.h"

int main() {
    printf("Hello, World!\nsizeof(int)=%lu\n", sizeof(int));
    //doing some pointer arithmetic
    int a;
    printf("\na+0:%p", &a);
    printf("\na+1:%p", &a + 1);
    printf("\na+2:%p", &a + 2);

    void *b = &a;
    printf("\nb+0:%p", b + 0);
    printf("\nb+1:%p", b + 1);
    printf("\nb+2:%p", b + 2);

    for (int i = 0; i < 257; ++i) {
        int heapSizeInMB = i;
        int logHeapSize = (int) ceil(log2((int) heapSizeInMB));
        printf("heapSizeInMB= %d, logHeapSize=%d\n", heapSizeInMB, logHeapSize);
    }
    //Checking correctedSize function
    for (int i = 0; i < 257; ++i) {
        int correct_size = correctedSize(i);
        printf("memory_request= %d, correct_size=%d\n", i, correct_size);
    }
    //Checking bucket_num function
    for (int i = 0; i < 257; ++i) {
        int correct_size = correctedSize(i);
        int bucket_number = bucket_num(correct_size);
        printf("memory_request= %d, correct_size=%d, bucket_num = %d,\n", i, correct_size, bucket_number);
    }
    //Checking get_bucket_size function
    for (int i = 0; i < 56; ++i) {
        int bucket_num = i;
        int bucket_sizeInB = get_bucket_size(i);
        printf("bucket_num= %d, bucket_sizeInB=%d\n", bucket_num, bucket_sizeInB);
    }
    //Checking Memory Initialization
    spHeap *heap1 = initializeMemory(10);
    printHeap(heap1);
    spHeap *heap2 = initializeMemory(100);
    printHeap(heap2);
    spHeap *heap3 = initializeMemory(1000);
    printHeap(heap3);
    // Below you can note how, though we requested only 100B, anything between 4B and 128B is accepted as request.
    // This is because 100 is initialized to bucket 128B.
    for (int i = -10; i < 150; ++i) {
        BucketBlock *spaceAvlBucket = checkSpaceAvailableBucket(heap2, i);
        printf("Requested %d bytes. Space is ", i);
        if (!spaceAvlBucket) {
            printf("not available in Heap2\n");
            continue;
        }
        printf("available in bucket %d of Heap 2\n", spaceAvlBucket->bucket_num);
    }
    for (int i = -10; i < 20; ++i) {
        BucketBlock *some_mem = allocateMemory(heap2, i, 1);
        if (some_mem) {
            printf("\n\nRequested Memory Size = %d, obtained Pointer = %p", i, some_mem->block->mem_address);
            printHeap(heap2);
        }
    }
    for (int i = 0; i < 40; ++i) {
        BucketBlock *some_mem = allocateMemory(heap3, i, 1);
        if (some_mem) {
            printf("\n\nRequested Memory Size = %d, obtained Pointer = %p", i, some_mem->block->mem_address);
            printHeap(heap3);
        }
    }
    //Repeated Requests of Same Size
    spHeap *heap4 = initializeMemory(1000);
    printHeap(heap4);
    int num_Allocs = 45;
    BucketBlock **bucketsAllocated = calloc(num_Allocs, sizeof(BucketBlock *));
    for (int i = 0; i < num_Allocs; ++i) {
        bucketsAllocated[i] = allocateMemory(heap4, i, 1);
        if (bucketsAllocated[i]) {
            printf("\n\nRequested Memory Size = %d, obtained Pointer = %p", i, bucketsAllocated[i]->block->mem_address);
            printHeap(heap4);
        }
    }
    //Free memory check
    for (int i = 0; i < num_Allocs; ++i) {
        if (bucketsAllocated[i]) {
            printf(""
                   "---------------------------------\n"
                   "The Block %d to be freed is as below\n", i);
            printMemBlock(bucketsAllocated[i]->block);
            printf("\n---------------------------------\n");
            freeMemory(heap4, bucketsAllocated[i]);
            printHeap(heap4);
        } else {
            printf("Sorry the bucket: %d has not been allocated\n", i);
        }
    }

    //check some inistializations
    spHeap *heap5 = initializeMemory(100);
    typedef struct somestruct {
        int a;
        float f;
    } someStruct;

    BucketBlock *somemem = allocateMemory(heap5, sizeof(someStruct), 1);
    someStruct *ss = somemem->block->mem_address;
    ss->a = 5;
    ss->f = 2.2f;
    printf("\nss->a=%d,ss->f = %.2f\n", ss->a, ss->f);

    BucketBlock *someothermem = allocateMemory(heap5, sizeof(someStruct), 1);
    someStruct *ss2 = someothermem->block->mem_address;
    ss2->a = 10;
    ss2->f = 4.4f;
    printf("\nss2->a=%d,ss2->f = %.2f\n", ss2->a, ss2->f);

    freeMemory(heap5, somemem);
    freeMemory(heap5, someothermem);
    printHeap(heap5);

    /*
     * The all important Experiment now
    */

    //First the bounds of the experiment
    int num_trials = 5;
    //size of memory in power of 2
    int minMemSize = 4;
    int maxMemSize = 28;
    //size of bin asked
    int binmin = 24;
    int binmax = 64;
    int binchange = 4;

    typedef struct experimental_structure {
        int a;
        int b;
        int c;
    } SPHeapTesterStruct;

    printf(""
           "\n------------------------------------------------------------------------"
           "\n------------------------------------------------------------------------"
           "\n\t\tStarting the experiment for spHeap"
           "\n------------------------------------------------------------------------"
           "\n------------------------------------------------------------------------");

    clock_t start, end;
    int num_experiments = 0, timeCount = 0;
    double TotIntFrag = 0, TotExtFrag = 0, TotMSPerMB = 0;


    printf("\n\tMemSize\t||\t  Bin\t||\tCnt\t||\tIntFrag\t||\tExtFrag\t\t||\tTimeTaken\n");
    for (int memPower = minMemSize; memPower <= maxMemSize; memPower += 1) {
        int binmin_actual = binmin, binmax_actual = binmax, binchange_actual = binchange;
        if (two_power(memPower) >= 1024 * 1024) {
            binmin_actual = two_power(memPower) / 1024 / 1024 * binmin;
            binmax_actual = two_power(memPower) / 1024 / 1024 * binmax;
            binchange_actual = two_power(memPower) / 1024 / 1024 * binchange;
        }
        for (int binSize = binmin_actual;
             binSize <= binmax_actual && binSize <= two_power(memPower); binSize += binchange_actual) {
            for (int trial_no = 0; trial_no < num_trials; ++trial_no) {
                int memFull = 0;
                int memSize = two_power(memPower);
                start = clock();
                int actual_bin_size = next_multiple_of8(binSize);
                int actual_mem_size = nextPowerOf2(memSize);
                int num_bins = actual_mem_size / actual_bin_size - 1;

                //printf("actual_mem_size=%d,actual_bin_size=%d,num_bins=%d\n",actual_mem_size,actual_bin_size,num_bins);

                //The structures
                SPHeapTesterStruct **adobe_locations = calloc(num_bins, sizeof(SPHeapTesterStruct *));
                BucketBlock **bucket_locations = calloc(num_bins, sizeof(BucketBlock *));
                //The memory Heap

                spHeap *spEx = initializeMemory(memSize);
                for (int i = 0; i < num_bins && !memFull; ++i) {
                    bucket_locations[i] = allocateMemory(spEx, binSize, 0);
                    if (!bucket_locations[i]) {
                        memFull = 1;
                        num_bins = i;
                        continue;
                    }
                    adobe_locations[i] = bucket_locations[i]->block->mem_address;

                }

                for (int i = 0; i < num_bins; ++i) {
                    adobe_locations[i]->a = 100 * i + 1;
                    adobe_locations[i]->b = 100 * i + 2;
                    adobe_locations[i]->c = 100 * i + 3;
                }

                float intFrag = (float) (spEx->stats->total_size_allocated - spEx->stats->total_size_requested) /
                                (float) spEx->stats->total_size_requested;
                float extFrag =
                        1.00 - (float) spEx->stats->total_size_allocated / (float) spEx->stats->total_size_of_heap;

                //for (int i = 0; i < num_bins; ++i) {
                // printf("For bin %d,a= %d,b= %d,c=%d\n",
                // i,adobe_locations[i]->a,adobe_locations[i]->b,adobe_locations[i]->c);
                // }
                for (int i = 0; i < num_bins; ++i) {
                    freeMemory(spEx, bucket_locations[i]);
                }

                freeHeap(spEx);
                free(bucket_locations);
                free(adobe_locations);
                end = clock();
                printBin(memSize);
                if(isnan(intFrag) || intFrag<0||intFrag>1){
                    intFrag=0.0;
                }
                printf("\t||\t%5d\t||\t%3d\t||\t%5.2f%%\t||\t %5.2f %%\t||\t%lims\n",
                       binSize, trial_no + 1, intFrag * 100, extFrag * 100, end - start);
                num_experiments++;
                TotIntFrag += intFrag;
                TotExtFrag += extFrag;
                if (end - start != 0) {
                    TotMSPerMB += (float) (end - start) / (float) actual_mem_size * (float) (1024 * 1024);
                    //printf("TotMSPerMB=%5.2f\n",(float)(end-start)/(float)actual_mem_size*(float)(1024*1024));
                    timeCount++;
                }

            }
        }
    }
    printf("The Experiment Statistics are as follows\n");
    printf("The Average Internal Fragmentation was %6.2f%%\n", TotIntFrag * 100 / (float) num_experiments);
    printf("The Average External Fragmentation was %6.2f%%\n", TotExtFrag * 100 / (float) num_experiments);
    printf("The Average Time Taken in ms. per MB was %6.2f\n", TotMSPerMB / (float) timeCount);


}