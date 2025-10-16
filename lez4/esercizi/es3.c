// parallelo
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    for (int i = 1; i <= 10; ++i) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); exit(1); }
        if (pid == 0) { // figlio
            printf("Hello World (%d) from child %d\n", i, getpid());
            fflush(stdout);
            exit(0);
        }
        // padre continua il loop senza wait => tutti i figli partono "in parallelo"
    }
    // padre aspetta tutti i figli
    for (int i = 0; i < 10; ++i) wait(NULL);
    exit(0);
}

// oppure for i in $(seq 1 10); do ./hello_sleep & done
// lancia 10 copie in background da shell