# Esercizi

1. Scrivere, compilare (`gcc nomefile.o -o nomefile`) ed eseguire un programma che forca un nuovo processo
2. Scrivere un programma che stampa a schermo "`Hello World (numero)`" per 10 volte alla distanza di 1 secondo l'una dall'altra (`sleep(int)`). Terminare con una chiamata a `exit(0)`
3. Usare il programma precedente per sperimentare l'esecuzione in sequenza e parallelo
4. Controllare valore di ritorno con `/bin/echo $?`
5. Tradurre il programma in assembly con `gcc -S nomefile.c`
6. Modificare l'assembly in modo che il programma esca con valore di ritorno 3 e controllare con `echo $?` dopo aver compilato con `gcc nomefile -o nomefile.s`
7. Modificare l'assembly in modo che usi `scanf` per ottenere il numero di saluti