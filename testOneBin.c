#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "oneBin.h"
#include "spHeapTester.h"

int main() {
    printf("Hello, World!");
    oneBin *ob = ob_start_kenobi(256);
    int *num_space = ob_wan_memory(ob);
    for (int i = 0; i <50; i+=5) {
        *num_space = i;
        printf("Digit now saved = %d\n", *num_space);
    }

    struct somestruct{
        int a;
        int b;
        int c;
    };
    struct somestruct* some1 = ob_wan_memory(ob);
    printf("address = %p. size of somestruct = %d\n",some1, (int)sizeof(struct somestruct));
    some1->a = 20;
    some1->b = 25;
    some1->c = 30;
    printf("address = %p\n",some1);
    struct somestruct* some2 = ob_wan_memory(ob);
    printf("address = %p\n",some2);
    some2->a = 5;
    some2->b = 10;
    some2->c = 15;
    printf("The numbers saved by you are"
           "\tfirst a = %d,\tfirst b = %d,\tfirst c = %d,"
           "\tsecond a = %d,\tsecond b = %d\tsecond c = %d\n",
            some1->a,some1->b,some1->c,some2->a,some2->b,some2->c);

}