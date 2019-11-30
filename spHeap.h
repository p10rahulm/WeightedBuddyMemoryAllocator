#ifndef WEIGHTEDBUDDYMEMORYALLOCATOR_SPHEAP_H
#define WEIGHTEDBUDDYMEMORYALLOCATOR_SPHEAP_H


#define SIZEOFHEAP 8 //(in 2^n MB)
#define MIN_ALLOCATABLE_BYTES 4 //(SIZEOF(INT) = 4
#define MAX_HEAP_SIZE 536870912
#define AVAILABLE 0
#define RESERVED 1
#define COMBINE31 0
#define COMBINE22 1
#define BUDDYLO 0
#define BUDDYHI 1

typedef struct heap_stats {
    int num_allocs;
    int num_deallocs;
    int splits;
    int recombines;
    int total_size_allocated;
    int total_size_requested;
    int total_size_of_heap;
}HeapStats;

typedef struct memory_block {
    int kval; //stores the n in 2^n
    int tag;  //1 bit to indicate available (0) or reserved (1)
    int type; //Indicates whether the type is
    void *mem_address; //base address of the block. This is what will be returned
    struct memory_block *next;
    struct memory_block *prev;
} memBlock;

typedef struct memory_bucket {
    int bucketSizeinB;
    int numMemBlocks;
    memBlock *head;
    memBlock *tail;
} memBucket;

typedef struct spHeap {
    int num_buckets;
    int smallestBucketSize;
    int largestBucketSize;
    memBucket *memBuckets;
    HeapStats* stats;
} spHeap;

typedef struct bucket_block{
    int bucket_num;
    memBlock* block;
} BucketBlock;


int correctedSize(int memSizeinBytes);

int bucket_num(int memSizeRequired);

int get_bucket_size(int bucket_num);

spHeap *initializeMemory(int heapBytes);

void printHeap(spHeap *inputHeap);

BucketBlock* checkSpaceAvailableBucket(spHeap *inputHeap, int spaceRequired);

BucketBlock *allocateMemory(spHeap *inputHeap, int spaceRequired);

void freeMemory(spHeap* inputHeap, BucketBlock* bucketFreed);

void printMemBlock(memBlock *inputBlock);
#endif //WEIGHTEDBUDDYMEMORYALLOCATOR_SPHEAP_H
