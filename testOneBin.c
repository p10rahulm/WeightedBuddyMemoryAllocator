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
    lightSaber(ob);
    struct somestruct* some1 = ob_wan_memory(ob);
    printf("address allocated = %p. size of somestruct = %d\n",some1, (int)sizeof(struct somestruct));
    some1->a = 20;
    some1->b = 25;
    some1->c = 30;
    printf("some1 address = %p\n",some1);
    lightSaber(ob);
    struct somestruct* some2 = ob_wan_memory(ob);
    printf("some2 address = %p\n",some2);
    lightSaber(ob);
    some2->a = 5;
    some2->b = 10;
    some2->c = 15;
    printf("The numbers saved by you are"
           "\tfirst a = %d,\tfirst b = %d,\tfirst c = %d,"
           "\tsecond a = %d,\tsecond b = %d\tsecond c = %d\n",
            some1->a,some1->b,some1->c,some2->a,some2->b,some2->c);

    printf("Now testing freeing\n\n\n");
    printf("\n\nsome1, Freed Memory: %p",some1);
    ob_gib_memory(ob,some1);
    lightSaber(ob);
    printf("some2, Freed Memory: %p",some2);
    ob_gib_memory(ob,some2);
    lightSaber(ob);

    printf("num_space, Freed Memory: %p",num_space);
    ob_gib_memory(ob,num_space);
    lightSaber(ob);


    //Another series of allocates and deallocates (want to allocate a hole)
    struct somestruct* some3 = ob_wan_memory(ob);
    printf("some3 address = %p\n",some3);
    lightSaber(ob);
    struct somestruct* some4 = ob_wan_memory(ob);
    printf("some4 address = %p\n",some4);
    lightSaber(ob);
    struct somestruct* some5 = ob_wan_memory(ob);
    printf("some5 address = %p\n",some5);
    lightSaber(ob);
    ob_gib_memory(ob,some4);
    printf("gave up some memory = %p\n",some4);
    lightSaber(ob);
    struct somestruct* some6 = ob_wan_memory(ob);
    printf("some6 address = %p\n",some6);
    lightSaber(ob);
    //All Working




}