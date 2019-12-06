#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "oneBin.h"


oneBin *ob_start_kenobi(int memSize, int oneBinSize) {
    int memSizeActual = nextPowerOf2((unsigned int) abs(memSize));
    if (memSize < MINBIN_SIZE) {
        memSizeActual = MINBIN_SIZE;
    } else if (memSizeActual > 536870912) {
        memSizeActual = 536870912;
    }

    int onebin_actual = next_multiple_of8(oneBinSize);
    if (oneBinSize < MINBIN_SIZE) { onebin_actual = MINBIN_SIZE; }
    if (oneBinSize > memSizeActual) { onebin_actual = memSizeActual; }

    oneBin *out = calloc(1, sizeof(oneBin));
    out->num_chunks = 1;
    out->firstFree = malloc(memSizeActual);
    out->total_size = memSizeActual;
    out->total_allocated = 0;
    out->base_address = out->firstFree;
    out->onebin_size = onebin_actual;

    memChunk *firstChunk = out->firstFree;
    firstChunk->nextChunk = NULL;
    firstChunk->prevChunk = NULL;
    firstChunk->size = memSizeActual;
    return out;
}

void set_address(void *mem_location, int size, memChunk *next, memChunk *prev) {
    memChunk *new_mem = mem_location;
    new_mem->size = size;
    new_mem->nextChunk = next;
    new_mem->prevChunk = prev;
}

void *ob_wan_memory(oneBin *ob_heap) {
    void *address_to_return = ob_heap->firstFree;
    memChunk *freeMem = ob_heap->firstFree;
    if (freeMem->size >= 2 * ob_heap->onebin_size) {
        void *new_address = ob_heap->firstFree + ob_heap->onebin_size;
        ob_heap->firstFree = new_address;
        set_address(new_address, freeMem->size - ob_heap->onebin_size, freeMem->nextChunk, freeMem->prevChunk);
        ob_heap->total_allocated += ob_heap->onebin_size;
    } else if (freeMem->nextChunk == NULL && freeMem->size < 2 * ob_heap->onebin_size) {
        printf("The Space you Allocated is full. Obi Cannot Wan");
        return NULL;
    } else if (freeMem->nextChunk && freeMem->size < 2 * ob_heap->onebin_size) {
        memChunk *nextMem = freeMem->nextChunk;
        set_address(nextMem, nextMem->size, nextMem->nextChunk, freeMem->prevChunk);
        ob_heap->firstFree = nextMem;
        ob_heap->total_allocated += ob_heap->onebin_size;
        ob_heap->num_chunks -= 1;
    }
    return address_to_return;
}

void merge_mem(oneBin *ob, void *obis_memory, void *next_free, void *prev_free) {
    memChunk *this_mem = obis_memory;
    memChunk *next_mem = next_free;
    memChunk *prev_mem = prev_free;

    void *this_address = this_mem;
    void *prev_address = prev_mem;
    if (this_address + this_mem->size == next_free && prev_address && (prev_address + prev_mem->size == this_address)) {
        prev_mem->size += this_mem->size + next_mem->size;
        prev_mem->nextChunk = next_mem->nextChunk;
        ob->num_chunks -= 2;
    } else if (this_address + this_mem->size == next_free) {
        this_mem->size += next_mem->size;
        this_mem->nextChunk = next_mem->nextChunk;
        ob->num_chunks -= 1;
    } else if (prev_address && (prev_address + prev_mem->size == this_address)) {
        prev_mem->size += this_mem->size;
        prev_mem->nextChunk = this_mem->nextChunk;
        ob->num_chunks -= 1;
    }
}

void ob_free_la_mem(oneBin *ob, void *obis_memory) {
    if (obis_memory < ob->base_address || obis_memory > ob->base_address + ob->total_size) {
        printf("Please check the memory location entered!\n");
        return;
    }
    if (obis_memory < ob->firstFree) {
        ob->total_allocated -= ob->onebin_size;
        memChunk *firstChunk = obis_memory;
        set_address(obis_memory, ob->onebin_size, ob->firstFree, NULL);
        memChunk *secondChunk = ob->firstFree;
        secondChunk->prevChunk = obis_memory;
        merge_mem(ob, obis_memory, ob->firstFree, NULL);
        ob->firstFree = obis_memory;
        ob->num_chunks += 1;
        return;
    } else {
        memChunk *rover = ob->firstFree;
        while (rover && ((void *) rover) < obis_memory) { rover = rover->nextChunk; }
        if (!rover) {
            printf("That memory was never allocated! Error Error!\n");
            return;
        }
        memChunk *prevChunk = rover->prevChunk;
        prevChunk->nextChunk = obis_memory;
        rover->prevChunk = obis_memory;
        memChunk *thisChunk = obis_memory;
        thisChunk->prevChunk = prevChunk;
        thisChunk->nextChunk = rover;
        thisChunk->size = ob->onebin_size;
        merge_mem(ob, thisChunk, rover, prevChunk);
        ob->num_chunks += 1;
        return;

    }
}

void lightSaber(oneBin *ob) {

    printf("\n----------------------------------------------------------------------------------------------"
           "\nPrinting OB's Heap"
           "\nob->total_size=%d\tob->onebin_size=%d\t\tob->total_allocated=%d\tob->num_chunks=%d"
           "\nob->base_address=%p\tob->firstFree=%p",
           ob->total_size, ob->onebin_size, ob->total_allocated, ob->num_chunks, ob->base_address, ob->firstFree);
    memChunk *rover = ob->firstFree;
    printf("\n");
    while (rover) {
        printf("-->rover->size=%d,Current address=%p,next Address=%p,prev Address=%p\n",
               rover->size, rover, rover->nextChunk, rover->prevChunk);
        rover = rover->nextChunk;
    }
    printf("----------------------------------------------------------------------------------------------\n");
}

void freeB(oneBin *ob) {
    free(ob->base_address);
    free(ob);
}