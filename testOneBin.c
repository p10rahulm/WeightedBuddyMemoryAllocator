#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "oneBin.h"
#include "spHeapTester.h"

int main() {
    printf("Hello, World!");
    oneBin *ob = ob_start_kenobi(256, 24);
    int *num_space = ob_wan_memory(ob);
    for (int i = 0; i < 50; i += 5) {
        *num_space = i;
        printf("Digit now saved = %d\n", *num_space);
    }

    struct somestruct {
        int a;
        int b;
        int c;
    };
    lightSaber(ob);
    struct somestruct *some1 = ob_wan_memory(ob);
    printf("address allocated = %p. size of somestruct = %d\n", some1, (int) sizeof(struct somestruct));
    some1->a = 20;
    some1->b = 25;
    some1->c = 30;
    printf("some1 address = %p\n", some1);
    lightSaber(ob);
    struct somestruct *some2 = ob_wan_memory(ob);
    printf("some2 address = %p\n", some2);
    lightSaber(ob);
    some2->a = 5;
    some2->b = 10;
    some2->c = 15;
    printf("The numbers saved by you are"
           "\tfirst a = %d,\tfirst b = %d,\tfirst c = %d,"
           "\tsecond a = %d,\tsecond b = %d\tsecond c = %d\n",
           some1->a, some1->b, some1->c, some2->a, some2->b, some2->c);

    printf("Now testing freeing\n\n\n");
    printf("\n\nsome1, Freed Memory: %p", some1);
    ob_free_la_mem(ob, some1);
    lightSaber(ob);
    printf("some2, Freed Memory: %p", some2);
    ob_free_la_mem(ob, some2);
    lightSaber(ob);

    printf("num_space, Freed Memory: %p", num_space);
    ob_free_la_mem(ob, num_space);
    lightSaber(ob);


    //Another series of allocates and deallocates (want to allocate a hole)
    struct somestruct *some3 = ob_wan_memory(ob);
    printf("some3 address = %p\n", some3);
    lightSaber(ob);
    struct somestruct *some4 = ob_wan_memory(ob);
    printf("some4 address = %p\n", some4);
    lightSaber(ob);
    struct somestruct *some5 = ob_wan_memory(ob);
    printf("some5 address = %p\n", some5);
    lightSaber(ob);
    ob_free_la_mem(ob, some4);
    printf("gave up some memory = %p\n", some4);
    lightSaber(ob);
    struct somestruct *some6 = ob_wan_memory(ob);
    printf("some6 address = %p\n", some6);
    lightSaber(ob);
    freeB(ob);
    //All Working


    /*
     * Doing the loopy loop to output everything!
     */
    //First the bounds of the experiment
    int num_trials = 5;
    //size of memory in power of 2
    int minMemSize = 5;
    int maxMemSize = 28;
    //size of bin asked
    int binmin = 28;
    int binmax = 64;

    typedef struct experimental_structure {
        int a;
        int b;
        int c;
    } Adobe;

    printf(""
           "\n------------------------------------------------------------------------"
           "\n------------------------------------------------------------------------"
           "\n\t\t\t\tStarting the experiment"
           "\n------------------------------------------------------------------------"
           "\n------------------------------------------------------------------------");

    clock_t start, end;
    double cpu_time_used;
    printf("\n\tMemSize\t||\tBin\t||\tCnt\t||\tIntFrag\t||\tExtFrag\t||\tTimeTaken\n");
    for (int memPower = minMemSize; memPower <= maxMemSize; memPower += 1) {
        for (int binSize = binmin; binSize <= binmax && binSize <= two_power(memPower); binSize += 4) {
            for (int trial_no = 0; trial_no < num_trials; ++trial_no) {
                int memSize = two_power(memPower);
                start = clock();
                int actual_bin_size = next_multiple_of8(binSize);
                int actual_mem_size = nextPowerOf2(memSize);
                int num_bins = actual_mem_size / actual_bin_size - 1;
                Adobe **adobe_locations = calloc(num_bins, sizeof(Adobe **));
                oneBin *Xiobi = ob_start_kenobi(memSize, binSize);
                for (int i = 0; i < num_bins; ++i) {
                    adobe_locations[i] = ob_wan_memory(Xiobi);
                }
                for (int i = 0; i < num_bins; ++i) {
                    adobe_locations[i]->a = 100 * i + 1;
                    adobe_locations[i]->b = 100 * i + 2;
                    adobe_locations[i]->c = 100 * i + 3;
                }
                float intFrag = (float)(actual_bin_size-binSize)/(float)binSize;
                float extFrag = 1.00-(float)Xiobi->total_allocated/(float)Xiobi->total_size;

                //for (int i = 0; i < num_bins; ++i) {
                // printf("For bin %d,a= %d,b= %d,c=%d\n",
                // i,adobe_locations[i]->a,adobe_locations[i]->b,adobe_locations[i]->c);
                // }
                for (int i = 0; i < num_bins; ++i) {
                    ob_free_la_mem(Xiobi, adobe_locations[i]);
                }
                //lightSaber(Xiobi);


                freeB(Xiobi);
                free(adobe_locations);
                end = clock();
                printBin(memSize);
                printf("\t||\t%d\t||\t%d\t||\t%2.2f%%\t||\t %2.2f %%\t||\t%lims\n",
                        binSize,trial_no+1,100*intFrag,100*extFrag,end - start);
            }
        }
    }


}