#include<stdio.h>
#include<signal.h>
 
void alarmHandler()
{
	printf("questo segnale lo gestisco io!\n");
	alarm(3); // resetta il timer a 3 secondi
}
 
main() { 
	signal(SIGALRM, alarmHandler);
	alarm(3);
	while(1){}
}
