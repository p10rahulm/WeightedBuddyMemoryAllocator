#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "oneBin.h"
#include "spHeapTester.h"

int main() {
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
           "\n\t\t\t\tStarting the experiment for one bin"
           "\n------------------------------------------------------------------------"
           "\n------------------------------------------------------------------------");

    clock_t start, end;
    int num_experiments=0;
    double TotIntFrag=0,TotExtFrag=0,TotAvgMSPerMB=0;


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
                       binSize,trial_no+1,intFrag*100,extFrag*100,end - start);
            }
        }
    }


}