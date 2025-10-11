#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

main() {
while(1){
           if(fork() < 0){
                perror(“errore fork”);
           }
       }
}

/* 
Compilazione:   gcc -o ffailure ffailure.c
Esecuzione:     linuxprompt$ ulimit -u 300
                linuxprompt$ ./ffailure
*/
