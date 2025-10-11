#include<stdio.h>
#include <unistd.h>

main() {
    printf("provo a eseguire ls\n");

    execl("/bin/ls","ls","-l",NULL); 
    // oppure : execlp("ls","ls","-l",NULL); 

    printf("non scrivo questo! \n");
    // questa printf non viene eseguita, se la exec va a buon fine
}
