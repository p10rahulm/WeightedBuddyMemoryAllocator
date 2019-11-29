#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "spHeap.h"

/*
 * SIZE LIST USED AS BELOW:
 *
-----------------------------------------------------------
c	bs	op	    power	B		    KB		    MB
-----------------------------------------------------------
1	2	power	28		268435456	262144		256
3	2	power	26		201326592	196608		192
1	2	power	27		134217728	131072		128
3	2	power	25		100663296	98304		96
1	2	power	26		67108864	65536		64
3	2	power	24		50331648	49152		48
1	2	power	25		33554432	32768		32
3	2	power	23		25165824	24576		24
1	2	power	24		16777216	16384		16
3	2	power	22		12582912	12288		12
1	2	power	23		8388608		8192		8
3	2	power	21		6291456		6144		6
1	2	power	22		4194304		4096		4
3	2	power	20		3145728		3072		3
1	2	power	21		2097152		2048		2
3	2	power	19		1572864		1536		1.5
1	2	power	20		1048576		1024		1
3	2	power	18		786432		768		    0.75
1	2	power	19		524288		512		    0.5
3	2	power	17		393216		384		    0.375
1	2	power	18		262144		256		    0.25
3	2	power	16		196608		192		    0.1875
1	2	power	17		131072		128		    0.125
3	2	power	15		98304		96		    0.09375
1	2	power	16		65536		64		    0.0625
3	2	power	14		49152		48		    0.046875
1	2	power	15		32768		32		    0.03125
3	2	power	13		24576		24		    0.0234375
1	2	power	14		16384		16		    0.015625
3	2	power	12		12288		12		    0.01171875
1	2	power	13		8192		8		    0.0078125
3	2	power	11		6144		6		    0.005859375
1	2	power	12		4096		4		    0.00390625
3	2	power	10		3072		3		    0.002929688
1	2	power	11		2048		2		    0.001953125
3	2	power	9		1536		1.5		    0.001464844
1	2	power	10		1024		1		    0.000976563
3	2	power	8		768		    0.75		0.000732422
1	2	power	9		512		    0.5		    0.000488281
3	2	power	7		384		    0.375		0.000366211
1	2	power	8		256		    0.25		0.000244141
3	2	power	6		192		    0.1875		0.000183105
1	2	power	7		128		    0.125		0.00012207
3	2	power	5		96		    0.09375		9.15527E-05
1	2	power	6		64		    0.0625		6.10352E-05
3	2	power	4		48		    0.046875	4.57764E-05
1	2	power	5		32		    0.03125		3.05176E-05
3	2	power	3		24		    0.0234375	2.28882E-05
1	2	power	4		16		    0.015625	1.52588E-05
3	2	power	2		12		    0.01171875	1.14441E-05
1	2	power	3		8		    0.0078125	7.62939E-06
3	2	power	1		6		    0.005859375	5.72205E-06
1	2	power	2		4		    0.00390625	3.8147E-06
-----------------------------------------------------------



 There are 53 lines above. So our ASL can have 53 elements.
 In general, if we allocate 2^n MB, we will have
 number of list elements = 37+2n elements
 in ASL list of size 4B and above.
 (4B element, plus two each for each power of 2^n Bytes, one for 1x2^n and one for 3x2^(n-2))

*/



typedef struct memory_block {
    int kval; //stores the n in 2^n
    int tag;  //1 bit to indicate available (0) or reserved (1)
    int type; //Indicates whether the type is
    void *mem_address; //base address of the block. This is what will be returned
    struct memory_block *next;
} memBlock;

typedef struct memory_bucket {
    int bucketSizeinB;
    int numMemBlock;
    memBlock *head;
} memBucket;

typedef struct spHeap {
    int num_buckets;
    int smallestBucketSize;
    int largestBucketSize;
    memBucket* memBuckets;
} spHeap;

int bucket_num(int memSizeRequired,spHeap* Heap){

}

int correctedSize(int memSizeinBytes,spHeap* inputHeap){
    //first we decide how much to allocate
    if(memSizeinBytes>inputHeap->largestBucketSize){
        printf("The memory size you have input is too large to fit into inputHeap");
        return -1;
    }

    int logSize = (int) ceil(log2((int) memSizeinBytes));
    int twoPowerLogSize = (int) pow(2,logSize);
    int output_size = twoPowerLogSize;
    if(twoPowerLogSize*3/4>=memSizeinBytes){
        output_size = twoPowerLogSize*3/4;
    } else {
        output_size = twoPowerLogSize;
    }
    return output_size;
}

spHeap *initialize_memory(int heapSizeInMB) {
    //error checks
    int heapSizeActual;
    heapSizeActual = heapSizeInMB;
    if (heapSizeInMB < 1) { heapSizeActual = 1; }
    if (heapSizeInMB > 512) { heapSizeActual = 512; }

    int logHeapSize = (int) ceil(log2((int) heapSizeInMB));
    int num_memory_buckets = 2 * logHeapSize + 37;
    spHeap *out = calloc(1, sizeof(spHeap));

    out->smallestBucketSize = 4;
    out->largestBucketSize = 4;
    out->num_buckets = num_memory_buckets;
    out->memBuckets = calloc(num_memory_buckets, sizeof(memBucket));
    for (int i = 0; i < num_memory_buckets; ++i) {
        out->memBuckets[i].numMemBlock=0;
        out->memBuckets[i].head=NULL;
    }


}

int closest_memory_block_size(int sizeInBytes) {

}