#ifndef WEIGHTEDBUDDYMEMORYALLOCATOR_ONEBIN_H
#define WEIGHTEDBUDDYMEMORYALLOCATOR_ONEBIN_H

#define ONEBIN_SIZE 24

typedef struct ob_want_heap{
    void* firstFree;
    int total_size;
    int num_chunks;
}oneBin;

typedef struct memChunk{
    int size;
    void* nextChunk;
    void* prevChunk;
} memChunk;

void* ob_wan_memory(oneBin* ob_heap);
oneBin* ob_start_kenobi(int memSize);

#endif //WEIGHTEDBUDDYMEMORYALLOCATOR_ONEBIN_H
