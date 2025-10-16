#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    for (int i = 1; i <= 10; ++i) {
        printf("Hello World (%d)\n", i);
        fflush(stdout);  // assicura che l'output venga stampato immediatamente
        sleep(1);
    }
    exit(0);
}

// gcc es2.c -o es2
// ./es2
