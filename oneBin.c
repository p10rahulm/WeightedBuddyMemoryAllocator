#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "oneBin.h"


oneBin* ob_start_kenobi(int memSize){
    oneBin* out = calloc(1, sizeof(oneBin));
    out->num_chunks=1;
    out->firstFree = malloc(memSize);
    out->total_size = memSize;
    memChunk* firstchunk = out->firstFree;
    firstchunk->nextChunk = NULL;
    firstchunk->prevChunk = NULL;
    firstchunk->size = memSize;
    return out;
}

void set_address(void* mem_location, int size, memChunk* next, memChunk* prev){
    memChunk* new_mem = mem_location;
    new_mem->size = size;
    new_mem->nextChunk = next;
    new_mem->prevChunk = prev;
}

void* ob_wan_memory(oneBin* ob_heap){
    void* address_to_return = ob_heap->firstFree;
    memChunk* freeMem = ob_heap->firstFree;
    if(freeMem->size >= 2*ONEBIN_SIZE){
        void* new_address = ob_heap->firstFree+ONEBIN_SIZE;
        ob_heap->firstFree = new_address;
        set_address(new_address,freeMem->size-ONEBIN_SIZE,freeMem->nextChunk,freeMem->prevChunk);
    }
    else if(freeMem->nextChunk==NULL && freeMem->size<2*ONEBIN_SIZE){
        printf("The Space you Allocated is full. Obi Cannot Wan");
        return NULL;
    } else if(freeMem->nextChunk && freeMem->size<2*ONEBIN_SIZE){
        memChunk* nextMem = freeMem->nextChunk;
        set_address(nextMem,nextMem->size,nextMem->nextChunk,freeMem->prevChunk);
        ob_heap->firstFree = nextMem;
    }
    return address_to_return;
}

void* ob_gib_memory(void* ){


}