# SO(LAB) 9 Ottobre 2025
Il file `.iso` non mantiene storage (come disco, non scrivibile); simula la disponibilità di un disco fisso ma a ogni accesso non si trova quello svolto la volta precedente (non c'è **persistenza**). Ma è atteso come comportamento.

## La Lezione delle Forche
```bash
wget http://homes.di.unimi.it/re/SYSOPLAB_24_25/L3.tar.gz
tar -xvzf L3.tar.gz
```
Famigerata lezione delle **FORCHE**!!!
Reminder segmenti in cui, generalmente, è suddivisa la **Memoria Virtuale**:
- Text (*codice*, read-only)
- Dati inizializzati
- Stack
- Heap

## Shell
Interprete dei comandi che l'utente da al sistema operativo, ne esistono grafiche e testuali. (Linux: `bash` più diffusa). Offre costrutti di base di un linguaggio di programmazione. Funziona, generalmente, così:
- Attesa input utente
- Interpretazione del comando utente
- Esecuzione comando/i utente

Implementare una shell richiede l'uso della chiamata di sistema `fork` (duplica un processo).

### Parentela tra processi
Il processo creante è detto **parent**, creato è **child**. Ogni processo (tranne `init`) ha un parent quindi si forma un "albero" di parentela.

### Creazione di processi
Richiede alcune operazioni da parte del SO:
- Creazione nuovo ID (PID - Process Identifier)
- Allocare memoria (codice, dati)
- Allocare altre risorse (stdin, stdout, dispositivi I/O...)
- Gestione informazoni sul nuovo processo (es. priorità)
- Creazione PCB (Process Control Block) che contiene informazioni precedenti

> `init` ha PID=1, è il primo processo (creato durante boot)

Cosa succede dopo che un processo viene creato?

### Relazioni dinamiche
Il processo **parent** può:
- Attendere la terminazione del processo **child**
```bash
top
```
Il terminale è associato al nuovo processo.
- Continuare senza attendere (es. esecuzione in *background*)
```bash
sleep 500 &
```

Quando un comando viene eseguito in background al genitore, notifica il suo PID.

```bash
ps
```

## Relazione di contenuto
Due possibilità:
- Processo **child** è un duplicato del **parent** (Unix)
- Il figlio esegue un programma differente (Windows)
> Citati sono i comportamenti standard, ma entrambi sono eseguibili da ambedue i sistemi

## SYSCALL fork
Permette di creare un duplicato di un processo **parent**. Appartiene allo standard POSIX (Portable Operating System Interface) di IEEE (Insitute of Electrical and Electronics Engineering). Essa crea un nuovo processo che:
- Condivide la medesima area codice del processo **parent**
- Usa una copia dell'area dati del processo **parent**
- Ha un **PID** differente dal **parent** (utile per compiere diverse operazioni su un processo e sull'altro)

Una fork fallisce quando *non è possibile* creare un processo e tipicamente questo accade quando *non c'è memoria* per il **processo** o il **kernel**.

Dopo **ogni** fork, esiste un processo identico al processo **parent** (tranne che per il valore di ritorno della fork) in esecuzione nello stesso punto del programma.

### Valori di ritorno di fork
- <0 Errore
- =0 Processo **child**
- \>0 Processo **parent** (il valore di ritorno è il PID del **child**)

### Schema standard utilizzo fork

```C
if ( (pid = fork() ) < 0 )
perror("fork error"); // stampa la descrizione dell'errore
else if (pid == 0) {
// codice figlio
} else {
// codice genitore, (pid > 0)
}
// codice del genitore e del figlio. usare con cautela
```

`pid_t` è un signed integer MA!!! potrebbe anche essere di dimensioni differenti, quindi bisogna usare `pid_t`.

#### Processo orfano
Il processo **parent** termina la propria esecuzione prima del processo **child**. Può capitare quando, ad esempio, inseriamo una *sleep* subito prima della printf nel file `fork3.c`.

```bash
cp fork1.c fork3.c
gcc fork3.c -o fork3
./fork3
```

Un processo orfano non viene più terminato con ctrl+c o dalla chiusura della shell.

#### Processo zombie
Sono processi terminati, ma in attesa che il processo **parent** rilevi il loro stato di terminazione. Per osservare la generazione di un processo **zombie** basta porre la *sleep* prima della printf del processo **parent**.

Per il test, è opportuno runnare in background il processo e poi runnare ps per vedere il processo "defunct".

![](/assets/09102025/zombieprocess.png)

#### Che output produce?

```C
#include <unistd.h>
#include <stdio.h>
 
main() {
  pid_t f1,f2,f3;
 
  f1=fork();
  f2=fork();
  f3=fork();
 
  printf("%i%i%i ", (f1 > 0),(f2 > 0),(f3 > 0)); 
}
```
Permutazione dei seguenti valori: `000 001 100 101 010 011 110 111`
Ossia tutti i numeri binari di 3 cifre in qualche ordine (dipende dallo scheduling). Perchè succede? Possiamo visualizzarlo con un albero binario 
in cui mettiamo a destra il processo genitore (stesso ID del nodo genitore) e a sinistra il processo figlio generato dalla fork. Il valore di `f1`, `f2`  ed `f3` sono quindi 0 sul ramo di sinistra e >0 sul ramo di destra.

![](/assets/09102025/fork.png)

## SYSCALL exec
Permette di eseguire un programma diverso da quello che ha effettuato la fork, **exec** sostituisce codice e dati di un processo con quelli di un programma differente.

![](/assets/09102025/syscallexec.png)

Ha diverse varianti che si differenziano in base a:
- formato degli argomenti (lista o array argv[])
- utilizzo o meno del path della shell

```C
execl("/bin/<programma>", arg0, arg1, ..., NULL);
execlp("<programma>", arg0, arg1, ..., NULL);
// Queste prime due varianti prendono una lista di argomenti terminata da NULL 

execv("/bin/<programma>", argv);
execvp("<programma>", argv);
// Queste prendono i parametri sotto forma di un array a puntatori a stringhe, terminato da NULL

// La presenza della p nel nome della exec indica che viene utilizzato il path della shell (quindi non è necessario specificare /bin perchè è già nel path)
```

La exec ritorna solamente in caso di errore (-1). In caso di successo, il vecchio codice è completamente sostituito dal nuovo e non è più possibile tornare al programma originale.

![](/assets/09102025/syscallexecerror.png)

Se tutto va a buon fine la exec non ritorna.

## Thread POSIX
Un thread è un'unità di esecuzione all'interno di un processo, un processo può avere più thread in esecuzione che tipicamente condividono le **risorse** del processo e, in particolare, la **memoria**. Lo standard POSIX definisce un insieme di funzioni per la creazione e la sincronizzazione di thread.

```C
pthread_create (pthread_t *thead, pthread_attr_t *attr, void *(*start_routine) void *), void *arg)
```

- **thread**: un puntatore a `pthread_t`, l'analogo di `pid_t`. non è necessariamente implementato come intero
- **attr**: attributi del nuovo thread; se non si vogliono modificare gli attributi è sufficiente passare `NULL`
- **start_routine**: codice da eseguire, è un puntatore a funzione che prende un puntatore a void e restituisce un puntatore a void
- **arg**: eventuali argomenti da passare (`NULL` se non si intende passare parametri)

### pthread_exit
Per terminare l'esecuzione di un thread, si usa la funzione `pthread_exit` che termina l'esecuzione di un thread restituendo un valore di ritorno.

```C
pthread_exit(void *retval)
```

### pthread_join
Per  attende la terminazione  di  un  generico  thread  th  si  usa  la  funzione `pthread_join`. Se ha successo, ritorna 0 e un puntatore al valore ritornato dal thread.

```C
pthread_join(pthread_t th, void **thread_return)
```

Se non si vuole ricevere il valore di ritorno è sufficiente passare `NULL` come secondo parametro.

### pthread_detach
Se non si vuole attendere la terminazione di un thread allora si deve eseguire la funzione `pthread_detach` che pone th in stato detached:  nessun  altro thread potrà attendere la sua terminazione  con  `pthread_join` e quando 
terminerà le sue risorse verranno automaticamente  rilasciate (evita che diventino thread “zombie”).

```C
pthread_detach(pthread_t th)
```

Si noti che `pthread_detach` non fa si che il thread rimanga attivo quando il processo termina con exit.

```C
pthread_t pthread_self() // ritorna il proprio thread id
```

Questo ID dipende dall'implementazione ed è l'ID della libreria phread e non l'ID di sistema. Per visualizzare l'ID di sistema (quello che si osserva con il comando `ps -L`, dove `L` sta per Lightweight process, ovvero thread) si può usare una **syscall specifica** di Linux: `syscall(SYS_gettid)`.

## Simulare una SHELL
```C
#include <stdio.h>
  #include <unistd.h>
  #include <stdlib.h>

  main() {
      pid_t esito;
      int status;
      char comando[128];
      while(1) {    
          printf("myshell# ");
          scanf("%s", comando); // Lettura rudimentale: niente argomenti separati
 
          if ((esito=fork()) < 0)
              perror("fallimento fork");
          else if (esito == 0) {
              execlp(comando,comando,NULL); // Non gestisce argomenti
              perror("Errore esecuzione:");
              exit(EXIT_FAILURE);
              }
              else{ // Codice genitore
                while( wait(&status) != esito ); // Aspetta completamento
              }

        // Il processo genitore (shell) torna immediatamente a leggere un altro comando
      }
  }
```

## Segnali
Sono una forma semplice di comunicazione tra processi; tecnicamente, sono interruzioni software causate da svariati eventi:
- Generati da terminale (es. *ctrl+c* (SIGINT))
- Eccezioni dovute ad errori in esecuzione (es. divisione per 0)
- Segnali **esplicitamente** inviati da un processo all'altro
- Eventi **asincroni** che vengono notificati ai processi (es. SIGALARM)

Cosa si può fare nel momento in cui arriva un segnale?
- Ignorarlo
- Gestirlo
- Lasciare il compito al gestore di sistema

```
Signal     Value     Action   Comment
------     -----     ------   -----------------------------------------------
SIGHUP        1       Term    Hangup detected on controlling terminal
                              or death of controlling process
SIGINT        2       Term    Interrupt from keyboard     <== ctrl­C
SIGQUIT       3       Core    Quit from keyboard          <== ctrl­\
SIGILL        4       Core    Illegal Instruction
SIGABRT       6       Core    Abort signal from abort(3)
SIGFPE        8       Core    Floating point exception
SIGKILL       9       Term    Kill signal                 <== kill ­9 (da 
shell)
SIGSEGV      11       Core    Invalid memory reference
SIGPIPE      13       Term    Broken pipe: write to pipe with no readers
SIGALRM      14       Term    Timer signal from alarm(2)
SIGTERM      15       Term    Termination signal          <== kill (da shell)
SIGUSR1   30,10,16    Term    User­defined signal 1
SIGUSR2   31,12,17    Term    User­defined signal 2
SIGCHLD   20,17,18    Ign     Child stopped or terminated <== gestito da 
wait()
SIGCONT   19,18,25    Cont    Continue if stopped
SIGSTOP   17,19,23    Stop    Stop process
```

#### Esempio (C)
```C
main() {
    alarm(3);
    while(1){}; // Ciclo infinito
}
```
Output previsto:
```bash
./alarmtest
Alarm clock # 3 secondi dall'esecuzione
```
Il programma punta una sveglia dopo 3 secondi; allo scadere di questi, invia un segnale `SIGALARM` al processo; il comportamento default è di terminare il processo.

Impostiamo il gestore dei segnali mediante `SIGNAL`:
```C
#include <stdio.h>
#include <signal.h>

void alarmHandler() {
    printf("Questo segnale lo gestisco io!\n");
    alarm(3);
}

main() {
    alarm(3);
    while(1){}; // Ciclo infinito
}
```
Questo scrive ogni 3 secondi `"Questo segnale lo gestisco io!"` a schermo.
```C
#include <stdio.h>
#include <signal.h>

main() {
    void (*old)(int);
    old = signal(SIGINT, SIG_IGN);
    printf("Sono protetto!\n");
    sleep(3);

    signal(SIGINT, old);
    printf("Non sono più protetto!\n");
    sleep(3);
}
```
Se modifichiamo il gestore del segnale `SIGINT` possiamo evitare anche che il programma venga interrotto tramite ctrl+c da terminale.
Se proviamo a farlo:
```bash
$ ./noctrlc # File
Sono protetto! 
^C^C^CNon sono più protetto! # Qui non ha effetto perchè è protetto
^C # Qui avrà effetto e interromperà l'esecuzione perchè non è più protetto
$
```
**N.B.**: il valore di ritorno della signal viene usato per *reimpostare il gestore originale*. La signal quando va a buon fine ritorna il gestore precedente al segnale, che salviamo nella variabile `old`: quando vogliamo reimpostare tale gestore è sufficiente passare `old` come secondo parametro a signal.

## Comunicazione tra processi
In un SO, logicamente c'è un numero elevato di programmi in esecuzione; idealmente, questi processi dovrebbero evitare di interferire l'uno con l'altro, ma nella pratica difficilmente il comportamento di un processo è **indipendente** da quello degli altri processi in esecuzione.

### Competizione
I processi competono anzitutto per le risorse comuni. Ad esempio:
- Apertura dello stesso file
- Utilizzo della stessa stampante
- Condivisione della CPU
La competizione di risorse crea **interferenze** tra i vari processi.

### Interferenze
**Starvation**: un processo è *bloccato* indefinitamente a causa di altri processi che monopolizzano una o più risorse.

**I/O**: L'accesso a una risorsa di input/output può variare notevolmente di prestazioni a seconda di quanti processi la stanno utilizzando.

Il SO deve gestire la competizione su risorse comuni in modo da ridurre il più possibile le interferenze e da garantire correttezza.
In definitiva, l'interazione tra processi non sempre è un evento positivo; il SO deve evitare inconvenienti costantemente; tuttavia, a volte, l'interazione tra i processi non è solo utile ma indispensabile per la realizzazione di alcuni task.

### Esempi di interazione positiva
- **Condivisione**: quando si vuole condividere informazione, è necessario interagire; ci sono moltissimi file nel sistema che sono condivisi da diverse applicazioni
- **Prestazioni**: con le architetture multicore si possono usare algoritmi paralleli per aumentare le prestazioni. Se un programma è scritto in modo sequenziale non andrà a sfruttare la presenza di più core
- **Modularità**: un'applicazione complessa viene spesso divisa in attività distinte più semplici, ognuna delle quali viene eseguita da un programma distinto (processo o thread).

I comandi UNIX forniscono un altro buon esempio di interazione positiva, sfruttando in modo efficace il concetto di modularità.
```bash
$ ls -al | grep fork
-rw-rw-r--   1 user user  fork1.c
-rw-r--r--   1 user user   fork2.c
$ 
```

`ls -al` mostra il contenuto del folder; il suo output viene dato in input ad un secondo comando, `|` è il **pipe**, simbolo che indica appunto che l'output del primo comando deve essere dato in input al secondo. In questo modo si ottiene un comportamento utile che combina due comandi semplici.

## Modelli di comunicazione
Per cooperare serve comunicare; esistono due modelli fondamentali di comunicazione tra thread e processi.

### Message passing
Scambio di messaggi; i processi o thread si scambiano informazioni tramite messaggi, un po' come avviene sulla rete:

![](/assets/09102025/messagepassing.png)

I processi, qui, dispongono di due primitive:

```bash
send(m) # Invia messaggio m
receive(&m) # Riceve un messaggio, lo salva in m
```

Vengono realizzate tramite opportune syscalls dette InterProcess Communication (IPC). Mittente e destinatario possono essere indicati direttamente o indirettamente.
Risulta essere un tipo di comunicazione che si adatta bene alle situazioni in cui un processo produce un dato e un altro lo consuma. 

#### Nominazione diretta
Mittente e destinatario sono nominati esplicitamente
```bash
send(P,m) # Invia il messaggio m a P
receive(Q,&m) # Riceve un messaggio da Q e lo salva in m
```
Canale riservato per ogni coppia di processi; basta conoscere la reciproca identità. Esiste anche una variante in cui si riceve da qualsiasi processo.
```bash
receive(&Q,&m) # Riceve un messaggio m da un qualsiasi processo messaggio e mittente vengono salvati in m e Q
```

- **Vantaggi**: la nominazione diretta è molto semplice e permette di comunicare in modo diretto tra coppie di processi
- **Svantaggi**: è però necessario un accordo sui nomi dei processi; il PID, infatti, vien dato dinamicamente dal sistema e non possiamo prevederne il valore a priori. Nella pratica è difficile da implementare a meno che i processi non siano in relazione stretta di parentela genitore-figlio.

#### Nominazione indiretta
Per ovviare ai difetti della nominazione diretta si utilizza, in pratica, una nominazione indiretta tramite **porte**. Le porte sono gestite dal sistema operativo tramite chiamate opportune, che ne consentono creazione e distruzione.

```bash
send(A,m) # Invia il messaggio m sulla porta A
receive(A,&m) # Riceve il messaggio dalla porta A e lo salva in m
```

Non è necessario, così, essere a conoscenza del nome dei processi, basta quello delle porte. 

Le pipe di UNIX implementano ad esempio un meccanismo di scambio di messaggi a nominazione indiretta, il riferimento alla pipe può avvenire tramite un descrittore (pipe senza nome) oppure tramite un nome conosciuto a livello di filesystem (pipe con nome).

### Shared memory
Memoria condivisa; i processi o thread condividono dati in memoria e accedono in lettura e scrittura a tali dati condivisi:

![](/assets/09102025/sharedmemory.png)