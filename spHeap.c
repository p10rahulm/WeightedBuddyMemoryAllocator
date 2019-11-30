#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "spHeap.h"

/*
 * SIZE LIST USED AS BELOW:
 *
-----------------------------------------------------------
BN  c	bs	op	    power	B		    KB		    MB
-----------------------------------------------------------
0	1	2	power	2		4			0.00390625		3.8147E-06
1	3	2	power	1		6			0.005859375		5.72205E-06
2	1	2	power	3		8			0.0078125		7.62939E-06
3	3	2	power	2		12			0.01171875		1.14441E-05
4	1	2	power	4		16			0.015625		1.52588E-05
5	3	2	power	3		24			0.0234375		2.28882E-05
6	1	2	power	5		32			0.03125			3.05176E-05
7	3	2	power	4		48			0.046875		4.57764E-05
8	1	2	power	6		64			0.0625			6.10352E-05
9	3	2	power	5		96			0.09375			9.15527E-05
10	1	2	power	7		128			0.125			0.00012207
11	3	2	power	6		192			0.1875			0.000183105
12	1	2	power	8		256			0.25			0.000244141
13	3	2	power	7		384			0.375			0.000366211
14	1	2	power	9		512			0.5				0.000488281
15	3	2	power	8		768			0.75			0.000732422
16	1	2	power	10		1024		1				0.000976563
17	3	2	power	9		1536		1.5				0.001464844
18	1	2	power	11		2048		2				0.001953125
19	3	2	power	10		3072		3				0.002929688
20	1	2	power	12		4096		4				0.00390625
21	3	2	power	11		6144		6				0.005859375
22	1	2	power	13		8192		8				0.0078125
23	3	2	power	12		12288		12				0.01171875
24	1	2	power	14		16384		16				0.015625
25	3	2	power	13		24576		24				0.0234375
26	1	2	power	15		32768		32				0.03125
27	3	2	power	14		49152		48				0.046875
28	1	2	power	16		65536		64				0.0625
29	3	2	power	15		98304		96				0.09375
30	1	2	power	17		131072		128				0.125
31	3	2	power	16		196608		192				0.1875
32	1	2	power	18		262144		256				0.25
33	3	2	power	17		393216		384				0.375
34	1	2	power	19		524288		512				0.5
35	3	2	power	18		786432		768				0.75
36	1	2	power	20		1048576		1024			1
37	3	2	power	19		1572864		1536			1.5
38	1	2	power	21		2097152		2048			2
39	3	2	power	20		3145728		3072			3
40	1	2	power	22		4194304		4096			4
41	3	2	power	21		6291456		6144			6
42	1	2	power	23		8388608		8192			8
43	3	2	power	22		12582912	12288			12
44	1	2	power	24		16777216	16384			16
45	3	2	power	23		25165824	24576			24
46	1	2	power	25		33554432	32768			32
47	3	2	power	24		50331648	49152			48
48	1	2	power	26		67108864	65536			64
49	3	2	power	25		100663296	98304			96
50	1	2	power	27		134217728	131072			128
51	3	2	power	26		201326592	196608			192
52	1	2	power	28		268435456	262144			256
-----------------------------------------------------------



 There are 53 lines above. So our ASL can have 53 elements numbered 0 to 52.
 In general, if we allocate 2^n MB, we will have
 number of list elements = 37+2n elements
 in ASL list of size 4B and above.
 (4B element, plus two each for each power of 2^n Bytes, one for 1x2^n and one for 3x2^(n-2))

 Note: Buckets of size MIN_ALLOCATABLE_BYTES and MIN_ALLOCATABLE_BYTES*3/2 cannot be split
*/

//Helper Functions Signatures
int isPowerOfTwo(int n);

int bucket_num(int memSizeRequired);

int get_bucket_size(int bucket_num);

int correctedSize(int memSizeinBytes);

spHeap *createEmptySPHeap();

spHeap *createMinSPHeap();

spHeap *initialize_memory_structure(int heapBytes);

void printMemBlock(memBlock *inputBlock);

void printMemBucket(memBucket *inputBucket);

BucketBlock *checkSpaceAvailableBucket(spHeap *inputHeap, int spaceRequired);

BucketBlock *checkSpaceAvailableInBucket(spHeap *inputHeap, int bucket_num);

void addBlockToTail(spHeap *inputHeap, int bucket_num, memBlock *memory_block);

void removeCurrentBlockFromSpaceList(spHeap *inputHeap, int bucket_num, memBlock *currentBlock);

BucketBlock *split(spHeap *inputHeap, BucketBlock *bucketHavingSpace, int spaceRequired);

HeapStats *initialize_heap_stats(int heapSizeInBytes);

void update_allocation_stats(spHeap *inputHeap, int space_requested, int space_allocated);

void printStats(spHeap *inputHeap);

//End Helper Functions Signatures

memBlock *createMemBlock(void *address, int kval, int tag, int type, memBlock *prev, memBlock *next) {
    memBlock *out = calloc(1, sizeof(memBlock));
    out->kval = kval;
    out->tag = tag;
    out->type = type;
    out->mem_address = address;
    out->prev = prev;
    out->next = next;
    return out;
}

spHeap *initializeMemory(int heapBytes) {
    spHeap *heap = initialize_memory_structure(heapBytes);
    int last_bucket_num = heap->num_buckets - 1;
    heap->memBuckets[last_bucket_num].numMemBlocks = 1;
    heap->memBuckets[last_bucket_num].head = createMemBlock(NULL, last_bucket_num, AVAILABLE, 0, NULL, NULL);
    heap->memBuckets[last_bucket_num].tail = heap->memBuckets[last_bucket_num].head;

    // * Above has been slightly modified from Shen's original paper for convenience.
    // * c*2^k can be represented in 2 ways. Therefore Paper Ambiguous. We store the actual bucket number

    // * We also store the address rather than computing it each time. This is for speed over memory efficiency

    int bucket_size_required = heap->memBuckets[last_bucket_num].bucketSizeinB;
    //we use malloc rather than calloc, as we are returning large storage size.
    // Cleaning all of them into 0's may take a long time.
    heap->memBuckets[last_bucket_num].head->mem_address = malloc(bucket_size_required);
    return heap;
}

void printHeap(spHeap *inputHeap) {
    printf(""
           "\n---------------------------------------------------------------------------------------------"
           "\nDETAILED STUDY OF HEAP THROUGH THE SP HEAP ALLOCATION"
           "\n---------------------------------------------------------------------------------------------\n");
    printf("Number of Buckets in Heap = %d\n", inputHeap->num_buckets);
    printf("Smallest Bucket Size in Heap = %d\n", inputHeap->smallestBucketSize);
    printf("Largest Bucket Size in Heap = %d\n", inputHeap->largestBucketSize);
    printf("\nNow Printing the Buckets");

    for (int i = 0; i < inputHeap->num_buckets; ++i) {
        printf(""
               "\n------------------------------------------------------"
               "\nBucket No: %d\t", i);
        printMemBucket(&(inputHeap->memBuckets[i]));

    }
    printf("\n------------------------------------------------------\n");
    printStats(inputHeap);

}

BucketBlock *allocateMemory(spHeap *inputHeap, int spaceRequired) {
    if (spaceRequired > inputHeap->largestBucketSize) {
        printf("The space you requested: %d is too big\n", spaceRequired);
        return NULL;
    }
    if (spaceRequired < inputHeap->smallestBucketSize) {
        printf("The space you requested: %d is too small\n", spaceRequired);
        return NULL;
    }
    BucketBlock *bucketHavingSpace = checkSpaceAvailableBucket(inputHeap, spaceRequired);
    if (!bucketHavingSpace) {
        printf("The space you requested: %d is not available. Sorry!\n", spaceRequired);
        return NULL;
    }
    BucketBlock *exactBucket = split(inputHeap, bucketHavingSpace, spaceRequired);
    if (exactBucket->bucket_num == bucket_num(spaceRequired) || exactBucket->bucket_num < 3) {
        inputHeap->stats->num_allocs += 1;
        update_allocation_stats(inputHeap, spaceRequired, inputHeap->memBuckets[exactBucket->bucket_num].bucketSizeinB);
        exactBucket->block->tag = RESERVED;
        return exactBucket;
    }
    printf("Something went wrong.\n");
    return NULL;
}


BucketBlock* findRecombinBuddy(spHeap* inputHeap,void* buddyAddr,int bucket_num ){
    BucketBlock* out = calloc(1, sizeof(BucketBlock));
    out->bucket_num = bucket_num;
    memBlock* rover = inputHeap->memBuckets[bucket_num].head;
    while(rover){
        if(rover->mem_address==buddyAddr){
            out->block = rover;
            return out;
        }
        rover = rover->next;
    }
    return NULL;
}

BucketBlock* combine_buddies31(spHeap* inputHeap,BucketBlock* bucketLow,BucketBlock* bucketHi){
    int lowSize = inputHeap->memBuckets[bucketLow->bucket_num].bucketSizeinB;

    if(bucketLow->block->mem_address+lowSize != bucketHi->block->mem_address){
        printf(""
               "Are you sure these two are buddies? Please check again.\n"
               "Bucket Low Address: %p, Bucket Hi Address: %p, Bucket Lo Size: %d",
               bucketLow->block->mem_address,bucketHi->block->mem_address,lowSize);
        return NULL;
    }
    BucketBlock* out = calloc(1, sizeof(BucketBlock));
    out->bucket_num = bucketLow->bucket_num-1;
    memBlock*  newBlock =createMemBlock(bucketLow->block->mem_address,bucketLow->block->kval+1,AVAILABLE,3,NULL,NULL);
    out->block =newBlock;
    addBlockToTail(inputHeap, bucketLow->bucket_num+1, newBlock);
    removeCurrentBlockFromSpaceList(inputHeap,bucketHi->bucket_num,bucketHi->block);
    removeCurrentBlockFromSpaceList(inputHeap,bucketLow->bucket_num,bucketLow->block);
}

BucketBlock* combine_buddies22(spHeap* inputHeap,BucketBlock* bucketLow,BucketBlock* bucketHi){
    int lowSize = inputHeap->memBuckets[bucketLow->bucket_num].bucketSizeinB;

    if(bucketLow->block->mem_address+lowSize != bucketHi->block->mem_address){
        printf(""
               "Are you sure these two are buddies? Please check again.\n"
               "Bucket Low Address: %p, Bucket Hi Address: %p, Bucket Lo Size: %d",
               bucketLow->block->mem_address,bucketHi->block->mem_address,lowSize);
        return NULL;
    }
    BucketBlock* out = calloc(1, sizeof(BucketBlock));
    out->bucket_num = bucketLow->bucket_num-1;
    memBlock*  newBlock =createMemBlock(bucketLow->block->mem_address,bucketLow->block->kval+1,AVAILABLE,3,NULL,NULL);
    out->block =newBlock;
    addBlockToTail(inputHeap, bucketLow->bucket_num+1, newBlock);
    removeCurrentBlockFromSpaceList(inputHeap,bucketHi->bucket_num,bucketHi->block);
    removeCurrentBlockFromSpaceList(inputHeap,bucketLow->bucket_num,bucketLow->block);
}


void freeMemory(spHeap* inputHeap, BucketBlock* bucketFreed){
    bucketFreed->block->tag=AVAILABLE;
    int bucket_num = bucketFreed->bucket_num;
    memBlock* memFreed = bucketFreed->block;
    int block_size = inputHeap->memBuckets[bucket_num].bucketSizeinB;
    if(bucket_num%2==0 && memFreed->type==3){
        void* memAddr = memFreed->mem_address;
        void* buddyAddr = memAddr - block_size*3;
        int buddy_bucket_num = bucket_num+3;
        BucketBlock* buddy = findRecombinBuddy(inputHeap,buddyAddr,buddy_bucket_num);
        if(buddy && buddy->block->tag==AVAILABLE){
                BucketBlock*new_bucket_to_free = combine_buddies31(inputHeap,buddy,bucketFreed);
                freeMemory(inputHeap, new_bucket_to_free);
        }
        return;
    }
    if(bucket_num%2==1 && memFreed->type==3){
        void* memAddr = memFreed->mem_address;
        void* buddyAddr = memAddr + block_size;
        int buddy_bucket_num = bucket_num-3;
        BucketBlock* buddy = findRecombinBuddy(inputHeap,buddyAddr,buddy_bucket_num);
        if(buddy && buddy->block->tag==AVAILABLE){
            BucketBlock*new_bucket_to_free = combine_buddies31(inputHeap,bucketFreed,buddy);
            freeMemory(inputHeap, new_bucket_to_free);
        }
        return;
    }
    if(bucket_num%2==0 && memFreed->type==1){
        void* memAddr = memFreed->mem_address;
        void* buddyAddr = memAddr - block_size*2;
        int buddy_bucket_num = bucket_num+2;
        BucketBlock* buddy = findRecombinBuddy(inputHeap,buddyAddr,buddy_bucket_num);
        if(buddy && buddy->block->tag==AVAILABLE){
            BucketBlock*new_bucket_to_free = combine_buddies22(inputHeap,buddy,bucketFreed);
            freeMemory(inputHeap, new_bucket_to_free);
        }
        return;
    }
    if(bucket_num%2==0 && memFreed->type==2){
        void* memAddr = memFreed->mem_address;
        void* buddyAddr = memAddr + block_size;
        int buddy_bucket_num = bucket_num-2;
        BucketBlock* buddy = findRecombinBuddy(inputHeap,buddyAddr,buddy_bucket_num);
        if(buddy && buddy->block->tag==AVAILABLE){
            BucketBlock*new_bucket_to_free = combine_buddies22(inputHeap,bucketFreed,buddy);
            freeMemory(inputHeap, new_bucket_to_free);
        }
        return;
    }
    return;

}

//Helper Functions Below
int isPowerOfTwo(int n) {
    if (n == 0)
        return 0;
    return (ceil(log2(n)) == floor(log2(n)));
}

int bucket_num(int memSizeRequired) {
    int corrected_memsize = correctedSize(memSizeRequired);
    if (corrected_memsize < MIN_ALLOCATABLE_BYTES) {
        printf("The size required is too small\n");
        return -1;
    }
    if (corrected_memsize > MAX_HEAP_SIZE) {
        printf("The size required is too large\n");
        return -1;
    }
    int logSize = (int) floor(log2((int) corrected_memsize));
    int logMin = (int) log2((int) MIN_ALLOCATABLE_BYTES);
    if (isPowerOfTwo(corrected_memsize)) {
        return (logSize - logMin) * 2;
    } else {
        return (logSize - logMin) * 2 + 1;
    }
}

int get_bucket_size(int bucket_num) {
    int power_of_two = bucket_num / 2;
    if (bucket_num % 2 == 0) {
        int two_power_bucketByTwo = (int) pow(2, power_of_two);
        return 4 * two_power_bucketByTwo;
    } else {
        int two_power_bucketByTwo = (int) pow(2, power_of_two);
        return 6 * two_power_bucketByTwo;
    }
}

int correctedSize(int memSizeinBytes) {
    //first we decide how much to allocate
    if (memSizeinBytes > MAX_HEAP_SIZE) {
        printf("The memory size you have input is too large to fit into inputHeap");
        return -1;
    }

    int logSize = (int) ceil(log2((int) memSizeinBytes));
    int twoPowerLogSize = (int) pow(2, logSize);
    int output_size = twoPowerLogSize;
    if (twoPowerLogSize * 3 / 4 >= memSizeinBytes) {
        output_size = twoPowerLogSize * 3 / 4;
    } else {
        output_size = twoPowerLogSize;
    }
    return output_size;
}

spHeap *createEmptySPHeap() {
    spHeap *out = calloc(1, sizeof(spHeap));
    out->smallestBucketSize = 0;
    out->largestBucketSize = 0;
    out->num_buckets = 0;
    out->memBuckets = NULL;
    out->stats = NULL;
    return out;
}

spHeap *createMinSPHeap() {
    spHeap *out = calloc(1, sizeof(spHeap));
    out->smallestBucketSize = MIN_ALLOCATABLE_BYTES;
    out->largestBucketSize = MIN_ALLOCATABLE_BYTES;
    out->num_buckets = 1;
    out->stats = initialize_heap_stats(MIN_ALLOCATABLE_BYTES);
    out->memBuckets = calloc(1, sizeof(memBucket));
    out->memBuckets[0].bucketSizeinB = MIN_ALLOCATABLE_BYTES;
    out->memBuckets[0].numMemBlocks = 1;
    out->memBuckets[0].head = createMemBlock(0, 0, 0, 0, NULL, NULL);
    out->memBuckets[0].tail = out->memBuckets[0].head;
    out->memBuckets[0].head->mem_address = calloc(1, MIN_ALLOCATABLE_BYTES);
    return out;
}

BucketBlock *checkSpaceAvailableInBucket(spHeap *inputHeap, int bucket_num) {
    if (bucket_num < 0 || bucket_num >= inputHeap->num_buckets) {
        printf("Please check the bucket Number input\n");
        return NULL;
    }
    memBlock *memBlockRover = inputHeap->memBuckets[bucket_num].head;
    while (memBlockRover) {
        if (memBlockRover->tag == AVAILABLE) {
            BucketBlock *out = calloc(1, sizeof(BucketBlock));
            out->bucket_num = bucket_num;
            out->block = memBlockRover;
            return out;
        }
        memBlockRover = memBlockRover->next;
    }
    return NULL;
}

BucketBlock *checkSpaceAvailableBucket(spHeap *inputHeap, int spaceRequired) {
    if (spaceRequired < 0 || spaceRequired > MAX_HEAP_SIZE) {
        printf("Please check the space required:%d that you have input\n", spaceRequired);
        return NULL;
    }
    if (spaceRequired < inputHeap->smallestBucketSize) {
        printf("The space requested:%d is too low. Please request atleast %d bytes\n", spaceRequired,
               inputHeap->smallestBucketSize);
        return NULL;
    }
    if (spaceRequired > inputHeap->largestBucketSize) {
        printf("The space requested:%d is too high. Please reinitialize a larger Heap\n", spaceRequired);
        return NULL;
    }
    int bucketNum = bucket_num(spaceRequired);
    BucketBlock *spaceAvlBucket = NULL;
    for (int i = bucketNum; i < inputHeap->num_buckets && !spaceAvlBucket; ++i) {
        spaceAvlBucket = checkSpaceAvailableInBucket(inputHeap, i);
    }
    return spaceAvlBucket;
}

BucketBlock *split(spHeap *inputHeap, BucketBlock *bucketHavingSpace, int spaceRequired) {
    if (bucketHavingSpace->bucket_num == bucket_num(spaceRequired)) { return bucketHavingSpace; }
    if (bucketHavingSpace->bucket_num < 3) { return bucketHavingSpace; }
    inputHeap->stats->splits += 1;

    memBlock *current_block = bucketHavingSpace->block;
    int bucket_num = bucketHavingSpace->bucket_num;
    int current_bucket_size = inputHeap->memBuckets[bucket_num].bucketSizeinB;


    if (bucket_num % 2 == 0) {
        memBlock *triple_two_power_n_minus2 = createMemBlock(current_block->mem_address, current_block->kval - 1,
                                                             AVAILABLE, 3, NULL, NULL);
        addBlockToTail(inputHeap, current_block->kval - 1, triple_two_power_n_minus2);
        void *new_pointer = current_block->mem_address;
        //int address_shift = current_bucket_size >> 2 + current_bucket_size >> 1;
        int address_shift = current_bucket_size * 3 / 4;
        new_pointer = new_pointer + address_shift;
        memBlock *single_two_power_n_minus2 = createMemBlock(new_pointer, current_block->kval - 4, AVAILABLE, 3, NULL,
                                                             NULL);
        addBlockToTail(inputHeap, current_block->kval - 4, single_two_power_n_minus2);


        if (spaceRequired <= inputHeap->memBuckets[current_block->kval - 4].bucketSizeinB) {
            bucketHavingSpace->bucket_num = current_block->kval - 4;
            bucketHavingSpace->block = single_two_power_n_minus2;
        } else {
            bucketHavingSpace->bucket_num = current_block->kval - 1;
            bucketHavingSpace->block = triple_two_power_n_minus2;
        }
        removeCurrentBlockFromSpaceList(inputHeap, bucket_num, current_block);
        return split(inputHeap, bucketHavingSpace, spaceRequired);

    } else {
        memBlock *two_power_n_minus1 = createMemBlock(current_block->mem_address, current_block->kval - 1, AVAILABLE, 2,
                                                      NULL, NULL);
        addBlockToTail(inputHeap, current_block->kval - 1, two_power_n_minus1);

        void *new_pointer = current_block->mem_address;
        int address_shift = (current_bucket_size << 1) / 3;
        new_pointer = new_pointer + address_shift;
        memBlock *two_power_n_minus2 = createMemBlock(new_pointer, current_block->kval - 3, AVAILABLE, 1, NULL, NULL);
        addBlockToTail(inputHeap, current_block->kval - 3, two_power_n_minus2);


        if (spaceRequired <= inputHeap->memBuckets[current_block->kval - 3].bucketSizeinB) {
            bucketHavingSpace->bucket_num = current_block->kval - 3;
            bucketHavingSpace->block = two_power_n_minus2;
        } else {
            bucketHavingSpace->bucket_num = current_block->kval - 1;
            bucketHavingSpace->block = two_power_n_minus1;
        }
        removeCurrentBlockFromSpaceList(inputHeap, bucket_num, current_block);
        return split(inputHeap, bucketHavingSpace, spaceRequired);
    }


}

void removeCurrentBlockFromSpaceList(spHeap *inputHeap, int bucket_num, memBlock *currentBlock) {
    inputHeap->memBuckets[bucket_num].numMemBlocks -= 1;
    if (inputHeap->memBuckets[bucket_num].numMemBlocks == 0) {
        inputHeap->memBuckets[bucket_num].head = NULL;
        inputHeap->memBuckets[bucket_num].tail = NULL;
    }
    if (currentBlock->prev) { currentBlock->prev->next = currentBlock->next; }
    if (currentBlock->next) { currentBlock->next->prev = currentBlock->prev; }
    free(currentBlock);
}

void addBlockToTail(spHeap *inputHeap, int bucket_num, memBlock *memory_block) {
    memory_block->prev = inputHeap->memBuckets[bucket_num].tail;
    memory_block->next = NULL;
    if (inputHeap->memBuckets[bucket_num].tail) {
        inputHeap->memBuckets[bucket_num].tail->next = memory_block;
    } else {
        inputHeap->memBuckets[bucket_num].head = memory_block;
    }

    inputHeap->memBuckets[bucket_num].tail = memory_block;
    inputHeap->memBuckets[bucket_num].numMemBlocks += 1;
}

spHeap *initialize_memory_structure(int heapBytes) {
    if (heapBytes < MIN_ALLOCATABLE_BYTES) {
        printf("Error. Number of bytes requested is too low\n");
        return createEmptySPHeap();
    }
    if (heapBytes > MAX_HEAP_SIZE) {
        printf("Error. Number of bytes requested is too large\n");
        return createEmptySPHeap();
    }
    //error checks
    int heapSizeActual = correctedSize(heapBytes);
    int num_memory_buckets = bucket_num(heapSizeActual) + 1;

    spHeap *out = calloc(1, sizeof(spHeap));
    out->smallestBucketSize = 4;
    out->largestBucketSize = heapSizeActual;
    out->num_buckets = num_memory_buckets;
    out->stats = initialize_heap_stats(heapSizeActual);

    out->memBuckets = calloc(num_memory_buckets, sizeof(memBucket));
    for (int i = 0; i < num_memory_buckets; ++i) {
        out->memBuckets[i].numMemBlocks = 0;
        out->memBuckets[i].bucketSizeinB = get_bucket_size(i);
        out->memBuckets[i].head = NULL;
        out->memBuckets[i].tail = NULL;
    }
    return out;
}

void printMemBucket(memBucket *inputBucket) {
    printf("Bucket Size in Bytes = %d\n", inputBucket->bucketSizeinB);
    printf("Number of Memory Blocks in this bucket = %d", inputBucket->numMemBlocks);
    if (inputBucket->numMemBlocks > 0) {
        memBlock *memBlockRover = inputBucket->head;
        while (memBlockRover) {
            printf("\n-->\t");
            printMemBlock(memBlockRover);
            memBlockRover = memBlockRover->next;

        }
    }
}

void printMemBlock(memBlock *inputBlock) {
    printf("[");
    printf("addr = %p", inputBlock->mem_address);
    printf(", (");
    printf("kval = %d", inputBlock->kval);
    printf(",");
    printf("tag = %d", inputBlock->tag);
    printf(",");
    printf("type = %d", inputBlock->type);

    printf(")]");

}

HeapStats *initialize_heap_stats(int heapSizeInBytes) {
    HeapStats *out = calloc(1, sizeof(HeapStats));
    out->num_allocs = 0;
    out->num_deallocs = 0;
    out->recombines = 0;
    out->splits = 0;
    out->total_size_allocated = 0;
    out->total_size_requested = 0;
    out->total_size_of_heap = heapSizeInBytes;
    return out;
}

void update_allocation_stats(spHeap *inputHeap, int space_requested, int space_allocated) {
    inputHeap->stats->num_allocs += 1;
    inputHeap->stats->total_size_requested += space_requested;
    inputHeap->stats->total_size_allocated += space_allocated;
}

void printStats(spHeap *inputHeap) {
    float internal_fragmentation = 0;
    if (inputHeap->stats->total_size_requested > 0) {
        internal_fragmentation = (float) (inputHeap->stats->total_size_allocated - inputHeap->stats->total_size_requested) /
                                 (inputHeap->stats->total_size_requested);
    }
    float percentageFull = 0;
    if (inputHeap->stats->total_size_of_heap > 0) {
        percentageFull = (float) inputHeap->stats->total_size_allocated / inputHeap->stats->total_size_of_heap;
    }

    printf(""
           "\n------------------------------------------"
           "\nSTATISTICS FOR SPHEAP"
           "\nNumber of Allocation Requests: \t\t%d"
           "\nNumber of De-allocation Requests: \t%d"
           "\nNumber of Splits: \t\t\t\t\t%d"
           "\nNumber of Recombines: \t\t\t\t%d"
           "\nTotal Size of Heap: \t\t\t\t%d"
           "\nTotal Size Requested: \t\t\t\t%d"
           "\nTotal Size Allocated: \t\t\t\t%d"
           "\nInternal Fragmentation: \t\t\t%.2f%%"
           "\nPercentage Full: \t\t\t\t\t%.2f%%"
           "\n------------------------------------------\n",
           inputHeap->stats->num_allocs, inputHeap->stats->num_deallocs, inputHeap->stats->splits,
           inputHeap->stats->recombines,
           inputHeap->stats->total_size_of_heap, inputHeap->stats->total_size_requested,
           inputHeap->stats->total_size_allocated,
           internal_fragmentation * 100, percentageFull * 100);
}