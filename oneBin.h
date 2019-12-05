#ifndef WEIGHTEDBUDDYMEMORYALLOCATOR_ONEBIN_H
#define WEIGHTEDBUDDYMEMORYALLOCATOR_ONEBIN_H

#define ONEBIN_SIZE 24

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
oneBin* ob_start_kenobi(int memSize);
void ob_gib_memory(oneBin* ob, void* obis_memory);
void lightSaber(oneBin* ob);
#endif //WEIGHTEDBUDDYMEMORYALLOCATOR_ONEBIN_H
