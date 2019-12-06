#ifndef WEIGHTEDBUDDYMEMORYALLOCATOR_ONEBIN_H
#define WEIGHTEDBUDDYMEMORYALLOCATOR_ONEBIN_H

#define MINBIN_SIZE 24

typedef struct ob_want_heap{
    int total_size;
    int total_allocated;
    int onebin_size;
    int num_chunks;
    void* base_address;
    void* firstFree;
}oneBin;

typedef struct memChunk{
    int size;
    void* nextChunk;
    void* prevChunk;
} memChunk;

void* ob_wan_memory(oneBin* ob_heap);
oneBin* ob_start_kenobi(int memSize,int oneBinSize);
void ob_free_la_mem(oneBin* ob, void* obis_memory);
void lightSaber(oneBin* ob);
int next_multiple_of8(int n);
unsigned int nextPowerOf2(unsigned int n);
void freeB(oneBin*ob);

#endif //WEIGHTEDBUDDYMEMORYALLOCATOR_ONEBIN_H
