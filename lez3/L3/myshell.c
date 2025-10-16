#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
main() {
   int esito;
   char comando[128];
   while(1) {    
           printf("myshell# ");
           scanf("%s", comando); //lettura rudimentale: niente argomenti separati
			      
           if ((esito=fork()) < 0)
	              perror("fallimento fork");
           else if (esito == 0) {
	              execlp(comando,comando,NULL); // NOTA: non gestisce argomenti
                      perror("Errore esecuzione:");
                      exit(EXIT_FAILURE);
           }
           // il processo genitore (shell) torna immediatamente a leggere un altro comando
   }
}
