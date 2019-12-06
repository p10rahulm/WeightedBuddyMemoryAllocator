#include <stdio.h>
#include "helpers.h"
#include "math.h"

void printBin(int memSize) {
    if (memSize <= 1024) {
        printf("%8dB", memSize);
        return;
    }
    if (memSize <= 1024 * 1024) {
        printf("%8.2fKB", (float) memSize / 1024);
        return;
    }
    printf("%8.2fMB", (float) memSize / 1024 / 1024);
    return;
}

int two_power(int n) {
    if (n < 0 || n > 31) {
        printf("Please input valid No.\n");
        return 0;
    }
    return 1u << n;
}

unsigned int nextPowerOf2(unsigned int n) {
    unsigned count = 0;
// First n in the below condition
// is for the case where n is 0
    if (n && !(n & (n - 1)))
        return n;

    while (n != 0) {
        n >>= 1u;
        count += 1;
    }
    return 1u << count;
}

int next_multiple_of8(int n) {
    if (n % 8 == 0)
        return n;
    return ((n / 8) + 1) * 8;
}

int isPowerOfTwo(int n) {
    if (n == 0)
        return 0;
    return (ceil(log2(n)) == floor(log2(n)));
}
