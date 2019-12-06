#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "oneBinTester.h"
#include "spHeapTester.h"

int main() {
    {
        /*
         * The all important Experiment now
        */

        //First the bounds of the experiment
        int num_trials = 5;
        //size of memory in power of 2
        int minMemSize = 4;
        int maxMemSize = 28;
        //size of bin asked
        int binmin = 24;
        int binmax = 64;
        int binchange = 4;

        typedef struct experimental_structure {
            int a;
            int b;
            int c;
        } SPHeapTesterStruct;

        printf(""
               "\n------------------------------------------------------------------------"
               "\n------------------------------------------------------------------------"
               "\n\t\tStarting the experiment for spHeap"
               "\n------------------------------------------------------------------------"
               "\n------------------------------------------------------------------------");

        clock_t start, end;
        int num_experiments = 0, timeCount = 0;
        double TotIntFrag = 0, TotExtFrag = 0, TotMSPerMB = 0;


        printf("\n\tMemSize\t||\t  Bin\t||\tCnt\t||\tIntFrag\t||\tExtFrag\t\t||\tTimeTaken\n");
        for (int memPower = minMemSize; memPower <= maxMemSize; memPower += 1) {
            int binmin_actual = binmin, binmax_actual = binmax, binchange_actual = binchange;
            if (two_power(memPower) >= 1024 * 1024) {
                binmin_actual = two_power(memPower) / 1024 / 1024 * binmin;
                binmax_actual = two_power(memPower) / 1024 / 1024 * binmax;
                binchange_actual = two_power(memPower) / 1024 / 1024 * binchange;
            }
            for (int binSize = binmin_actual;
                 binSize <= binmax_actual && binSize <= two_power(memPower); binSize += binchange_actual) {
                for (int trial_no = 0; trial_no < num_trials; ++trial_no) {
                    int memFull = 0;
                    int memSize = two_power(memPower);
                    start = clock();
                    int actual_bin_size = next_multiple_of8(binSize);
                    int actual_mem_size = nextPowerOf2(memSize);
                    int num_bins = actual_mem_size / actual_bin_size - 1;

                    //printf("actual_mem_size=%d,actual_bin_size=%d,num_bins=%d\n",actual_mem_size,actual_bin_size,num_bins);

                    //The structures
                    SPHeapTesterStruct **adobe_locations = calloc(num_bins, sizeof(SPHeapTesterStruct *));
                    BucketBlock **bucket_locations = calloc(num_bins, sizeof(BucketBlock *));
                    //The memory Heap

                    spHeap *spEx = initializeMemory(memSize);
                    for (int i = 0; i < num_bins && !memFull; ++i) {
                        bucket_locations[i] = allocateMemory(spEx, binSize, 0);
                        if (!bucket_locations[i]) {
                            memFull = 1;
                            num_bins = i;
                            continue;
                        }
                        adobe_locations[i] = bucket_locations[i]->block->mem_address;

                    }

                    for (int i = 0; i < num_bins; ++i) {
                        adobe_locations[i]->a = 100 * i + 1;
                        adobe_locations[i]->b = 100 * i + 2;
                        adobe_locations[i]->c = 100 * i + 3;
                    }

                    float intFrag = (float) (spEx->stats->total_size_allocated - spEx->stats->total_size_requested) /
                                    (float) spEx->stats->total_size_requested;
                    float extFrag =
                            1.00 - (float) spEx->stats->total_size_allocated / (float) spEx->stats->total_size_of_heap;

                    //for (int i = 0; i < num_bins; ++i) {
                    // printf("For bin %d,a= %d,b= %d,c=%d\n",
                    // i,adobe_locations[i]->a,adobe_locations[i]->b,adobe_locations[i]->c);
                    // }
                    for (int i = 0; i < num_bins; ++i) {
                        freeMemory(spEx, bucket_locations[i]);
                    }

                    freeHeap(spEx);
                    free(bucket_locations);
                    free(adobe_locations);
                    end = clock();
                    printBin(memSize);
                    if (isnan(intFrag) || intFrag < 0 || intFrag > 1) {
                        intFrag = 0.0;
                    }
                    printf("\t||\t%5d\t||\t%3d\t||\t%5.2f%%\t||\t %5.2f %%\t||\t%lims\n",
                           binSize, trial_no + 1, intFrag * 100, extFrag * 100, end - start);
                    num_experiments++;
                    TotIntFrag += intFrag;
                    TotExtFrag += extFrag;
                    if (end - start != 0) {
                        TotMSPerMB += (float) (end - start) / (float) actual_mem_size * (float) (1024 * 1024);
                        //printf("TotMSPerMB=%5.2f\n",(float)(end-start)/(float)actual_mem_size*(float)(1024*1024));
                        timeCount++;
                    }

                }
            }
        }
        printf("The Experiment Statistics are as follows\n");
        printf("The Average Internal Fragmentation was %6.2f%%\n", TotIntFrag * 100 / (float) num_experiments);
        printf("The Average External Fragmentation was %6.2f%%\n", TotExtFrag * 100 / (float) num_experiments);
        printf("The Average Time Taken in ms. per MB was %6.2f\n", TotMSPerMB / (float) timeCount);


    }
    {
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
        int num_experiments = 0, timeCount = 0;
        double TotIntFrag = 0, TotExtFrag = 0, TotMSPerMB = 0;


        printf("\n\tMemSize\t||\tBin\t||\tCnt\t||\tIntFrag\t||\tExtFrag\t\t||\tTimeTaken\n");
        for (int memPower = minMemSize; memPower <= maxMemSize; memPower += 1) {
            for (int binSize = binmin; binSize <= binmax && binSize <= two_power(memPower); binSize += 4) {
                for (int trial_no = 0; trial_no < num_trials; ++trial_no) {
                    int memSize = two_power(memPower);
                    start = clock();
                    int actual_bin_size = next_multiple_of8(binSize);
                    int actual_mem_size = nextPowerOf2(memSize);
                    int num_bins = actual_mem_size / actual_bin_size - 1;
                    Adobe **adobe_locations = calloc(num_bins, sizeof(Adobe *));
                    oneBin *Xiobi = ob_start_kenobi(memSize, binSize);
                    for (int i = 0; i < num_bins; ++i) {
                        adobe_locations[i] = ob_wan_memory(Xiobi);
                    }
                    for (int i = 0; i < num_bins; ++i) {
                        adobe_locations[i]->a = 100 * i + 1;
                        adobe_locations[i]->b = 100 * i + 2;
                        adobe_locations[i]->c = 100 * i + 3;
                    }
                    float intFrag = (float) (actual_bin_size - binSize) / (float) binSize;
                    float extFrag = 1.00 - (float) Xiobi->total_allocated / (float) Xiobi->total_size;

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
                    printf("\t||\t%d\t||\t%d\t||\t%5.2f%%\t||\t %5.2f %%\t||\t%lims\n",
                           binSize, trial_no + 1, intFrag * 100, extFrag * 100, end - start);
                    num_experiments++;
                    TotIntFrag += intFrag;
                    TotExtFrag += extFrag;
                    if (end - start != 0) {
                        TotMSPerMB += (float) (end - start) / (float) actual_mem_size * (float) (1024 * 1024);
                        //printf("TotMSPerMB=%5.2f\n",(float)(end-start)/(float)actual_mem_size*(float)(1024*1024));
                        timeCount++;
                    }

                }
            }
        }
        printf("The Experiment Statistics are as follows\n");
        printf("The Average Internal Fragmentation was %6.2f%%\n", TotIntFrag * 100 / (float) num_experiments);
        printf("The Average External Fragmentation was %6.2f%%\n", TotExtFrag * 100 / (float) num_experiments);
        printf("The Average Time Taken in ms. per MB was %6.2f\n", TotMSPerMB / (float) timeCount);
    }

}