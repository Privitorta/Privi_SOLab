#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

main() {
	pid_t pid;
	printf("Prima della fork. pid = %d, pid del genitore = %d\n",getpid(), getppid());
	if ( (pid = fork()) < 0 )
		perror("fork error"); // stampa la descrizione dell'errore
	else if (pid == 0) {          // figlio
		printf("[Figlio] pid = %d, pid del genitore = %d\n",getpid(), getppid());
	} else {                           // genitore
		printf("[Genitore] pid = %d, pid del mio genitore = %d\n",getpid(), getppid());
		printf("[Genitore] Mio figlio ha pid = %d\n",pid);
		sleep(1); // attende 1 secondo
	}
	// entrambi i processi
	printf("PID %d termina.\n", getpid());
}
