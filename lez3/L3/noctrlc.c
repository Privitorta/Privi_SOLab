#include<signal.h>
#include<stdio.h>
main() {
	void (*old)(int);
 
	old = signal(SIGINT,SIG_IGN);
	printf("Sono protetto!\n");
	sleep(3);
 
	signal(SIGINT,old);
	printf("Non sono più protetto!\n");
	sleep(3);
}

