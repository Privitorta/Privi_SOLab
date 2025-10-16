#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

die(char * s, int e) {                        // stampa gli errori ed esce (non si puo' usare perror)
    printf("%s [%i]\n",s,e);
    exit(1);
}

void * codice_thread(void * a) {     // prende un puntatore e ritorna un puntatore (a void)
    pthread_t tid;
    int ptid;
    
    tid=pthread_self();                    // library tid
    ptid = syscall(SYS_gettid);       // tid assegnato dal SO

    printf("Sono il thread %lu (%i) del processo %i\n",tid,ptid,getpid());
    sleep(1);
    pthread_exit(NULL);
}

main() {
    pthread_t tid[2];
    int i,err;
    // crea i thread
    //     // - gli attributi sono quelli di default (il secondo parametro e' NULL)
    //         // - codice_thread e' il nome della funzione da eseguire
    //             // - non vegnono passati parametri (quarto parametro e' NULL)
    for (i=0;i<2;i++)
      if (err=pthread_create(&tid[i],NULL,codice_thread,NULL))
         die("errore create",err);
                           
         // attende i thread. Non si legge il valore di ritorno (secondo parametro NULL)
         for (i=0;i<2;i++)
           if (err=pthread_join(tid[i],NULL))
              die("errore join",err);
                                                     
           printf("I thread hanno terminato l'esecuzione correttamente\n");
}
