
#ifndef WEIGHTEDBUDDYMEMORYALLOCATOR_SPHEAPTESTER_H
#define WEIGHTEDBUDDYMEMORYALLOCATOR_SPHEAPTESTER_H

#include "spHeap.h"

int correctedSize(int memSizeinBytes);

int bucket_num(int memSizeRequired);

int get_bucket_size(int bucket_num);

BucketBlock *checkSpaceAvailableBucket(spHeap *inputHeap, int spaceRequired);

void printMemBlock(memBlock *inputBlock);

#endif //WEIGHTEDBUDDYMEMORYALLOCATOR_SPHEAPTESTER_H
