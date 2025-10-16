#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) { // figlio
        printf("figlio: PID=%d, padre=%d\n", getpid(), getppid());
        exit(0);
    } else { // padre
        printf("padre: PID=%d, figlio=%d\n", getpid(), pid);
        wait(NULL); // aspetta terminazione figlio
        exit(0);
    }
}

// gcc es1.c -o es1
// ./es1
