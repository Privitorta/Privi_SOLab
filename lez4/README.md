# SO(LAB) 16 Ottobre 2025

[https://www.grymoire.com/Unix/index.html](https://www.grymoire.com/Unix/index.html)

Le **pipe di Unix** implementano un meccanismo di scambio di messaggi a <u>nominazione indiretta</u>; il riferimento alla pipe può avvenire tramite un descrittore (pipe senza nome) oppure tramite un nome conosciuto a livello di filesystem (pipe con nome).
Esempio:
```bash
ls -al | grep fork
```
> Il processo `ls -al` produce un output che viene dato in input a `grep fork` che lo consuma generando un ulteriore output

## Pipe
Una **pipe** (*tubo*) funge da <u>canale di comunicazione tra 2 processi distinti</u>. Nella shell, i due comandi ("processi") uniti vengono lanciati in modo da potersi parlare; in comune hanno il "parent", grazie alla shell si crea una memoria condivisa.
Il simbolo di pipe **accoppia** automaticamente.

Si possono **inviare** dati da un lato della pipe, per poi **riceverli** dal lato opposto. Tecnicamente, è una porta di comunicazione (*nominazione indiretta*). In UNIX, esistono due forme di pipe:
- <u>**Senza nome**</u>: utilizzabili solo da processi con <u>antenati comuni</u> in quanto risorse che vengono **ereditate dai genitori**
- <u>**Con nome**</u>: hanno un **nome nel filesystem** e costituiscono delle porte che <u>tutti i processi</u> possono utilizzare

### Pipe senza nome
Usate per combinare comandi UNIX direttamente dalla shell tramite il simbolo pipe "|". Per creare una pipe, si usa la systemcall `pipe(int filedes[2])` che restituisce in `filedes` due descrittori di file:
```bash
filedes[0] # per la lettura
filedes[1] # per la scrittura
```
Le pipe sono **half-duplex**(monodirezionali): esistono due distinti descrittori per R/W. Una pipe si usa come un normale file, per il resto.
```C
// testpipe.c

#include <stdio.h>
#include <string.h>
main() {
    int fd[2];

    pipe(fd); /* crea la pipe */
    if (fork() == 0) {
        char *phrase = "prova a inviare questo!";

        close(fd[0]); /* chiude in lettura */
        write(fd[1],phrase,strlen(phrase)+1); /* invia anche 0x00 */
        close(fd[1]); /* chiude in scrittura */
    } else {
        char message[100];
        memset(message,0,100);
        int bytesread;

        close(fd[1]); /* chiude in scrittura */
        bytesread = read(fd[0],message,100);
        printf("ho letto dalla pipe %d bytes: '%s' \n",bytesread,message);
        close(fd[0]); /* chiude in lettura */
    }
}
```

**Compilazione**: `gcc -o testpipe testpipe.c`<br>
**Esecuzione**:`./testpipe`<br>
**Output**: ho letto dalla pipe 24 bytes: 'prova a inviare questo!'

### Pipe e comandi UNIX
Le pipe permettono quindi di eseguire comandi UNIX nella shell in serie, fornendo l'output prodotto da un programma in input al comando successivo. Esaminiamo un comando più complesso nella shell:

```bash
cat /usr/include/string.h | xargs -n1 | tr A-Z a-z | sed -e 's/\.//g' -e 's/\,//g' -e
's/ //g' | sort | uniq -c | sort -nr
```
- `cat /usr/include/string.h | xargs -n1`: elenca il file una parola per riga
- `| tr A-Z a-z |`: cambia tutte le lettere maiuscole in minuscole
- `sed -e 's/\.//g' -e 's/\,//g' -e 's/ //g'`: filtra i punti e le virgole, cambia gli spazi tra parole in linefeed
- `| sort | uniq -c | sort -nr`: conteggio occorrenze e ordinamento in base al numero

## Lanciare comandi con Shell
Per iniziare l'esecuzione di un programma, basta scrivere il nome del file. 
```bash
/bin/ls
```
Il programma viene trattato come una **funzione** che prende dei parametri e ritorna un intero.
```bash
/bin/ls
/bin/ls /usr
/bin/ls piripacchio
```
Si può:
```bash
# evitare che il padre aspetti la terminazione del figlio
/bin/ls /usr & # & -> esecuzione in background
/bin/ls /usr ; /bin/ls /usr # due programmi in sequenza
/bin/ls /usr & /bin/ls /usr # due programmi in parallelo
```

## UNIX Power Tools
> [!IMPORTANT]
> In un sistema UNIX ogni risorsa è in realtà un file (anche le cartelle lo sono); un processo UNIX vede tutto il mondo esterno (I/O) come un insieme di descrittori (da qui discende omogeneità tra file e dispositivi UNIX).

> [!NOTE] 
> I file descriptor sono piccoli interi non negativi che identificano file aperti. stdin, stdout o stderr e sono associati ai file descriptor 0, 1, 2.

La <u>shell</u> è un linguaggio di programmazione completo (interpretato dalla shell stessa /bin/bash):

- **Variabili** (create al primo assegnamento, uso con $, export in un'altra shell)
    - `x="ciao"`; `y=2`; `/bin/echo "$x $y $x"`
    - Nel momento in cui la creo logicamente inizia ad esistere
    - L'accesso al contenuto di variabile avviene con l'operatore designato `$`
- **Istruzioni condizionali**(valore di ritorno `0` indica `true`)
```bash
if /bin/ls/piripacchio ; then /bin/echo ciao; else /bin/echo buonasera; fi
```
- **Iterazione su insiemi**
    - `for i in a b c d e ; do /bin/echo $i ; done`
- **Cicli**
```bash
usr/bin/touch piripacchio
while /bin/ls piripacchio; do
/usr/bin/sleep 2
/usr/bin/echo ciao
done & (/usr/bin/sleep 10 ; /bin/rm piripacchio)
```

## Linux
Il cuore del sistema operativo (che è il "primo strato dopo il ferro") è il **kernel**. Esso si occupa di gestire tutte le risorse hardware presenti nel calcolatore, allocandole in modo tale da ottimizzarne l'utilizzo, e tutto il software (eseguendo determinati programmi) in caso di necessità. Il kernel è il responsabile principale per:
- Gestione memoria
- Gestione hardware
- Gestione software
- Gestione filesystem

### Memoria
Il kernel gestisce la memoria fisica (non solo RAM, anche di massa) della macchina ma è anche in grado di creare e gestire memoria virtuale (che non esiste davvero). Per farlo, usa uno spazio sul disco detto <u>spazio di swap</u>. Il kernel sposta dinamicamente locazioni di memoria dallo spazio di swap alla memoria fisica, in questo modo **simula** l'esistenza di una quantità di memoria superiore a quella fisicamente disponibile.

Le locazioni di memoria virtuale sono raggruppate in blocchi detti <u>pagine</u>. Può collocare le pagine di memoria sia nello swap che nella memoria fisica. Il kernel mantiene una tabella che indica quali pagine di memoria sono in swap e quali in memoria fisica.

Le informazioni sullo stato della memoria virtuale si possono recuperare nel file `/proc/meminfo`, dove vengono registrate.

Usare la `cat` è utile per indagare sullo stato della memoria (fisica o virtuale) del sistema, semplicemente visualizzando il contenuto di `/proc/meminfo`.
```bash
cat /proc/meminfo
```

### File
Un file è un insieme di byte conservati nella memoria di massa. Hanno associato un **nome** e altri attributi. Nei sistemi UNIX-like, sono organizzati gerarchicamente in directory che non sono altro che file contenenti un **elenco**. 

> [!NOTE]
> `$ cat nomefile` stampa a schermo il contenuto di `nomefile`.
> Se invece di voler scrivere l'output di un comando a schermo (stdout) lo voglio scrivere su un file, posso usare la redirezione dell'output: `cat nomefile > nomefile`.

I processi interagiscono con l'I/O secondo il paradigma *open-read-write-close* (operazioni di **prologo** e di **epilogo**).
- Flessibilità (possibilità di **pipe** e **redirezione**)
- System Call per operare a basso livello sui file:
    - `create`, `open`, `close`, `read`, `write`, `lseek`

In generale, 
- Lettura da `fd 0` →  legge da standard input
- Scrittura su `fd 1` →  scrive su standard output
- Scrittura su `fd 2` →  scrive su standard error

Questi tre file descriptor sono aperti automaticamente dal sistema (shell) per 
ogni processo e collegati all’I/O per progettare **filtri**, cioè usare <u>redirezione</u> e <u>piping</u>. I filtri leggono direttamente dal `file descriptor` **0** , scrivono direttamente sul `file descriptor` **1**.

#### Lettura e Scrittura
```C
nread = read(fd, buf, n);
nwrite = write(fd, buf, n);

int nread, nwrite, n, fd;
char *buf;
```
**Lettura** e **scrittura** di un file avvengono a partire dalla **posizione** corrente del file e avanzano il puntatore (*I/O pointer*) all'interno del file. Restituiscono il **numero dei byte** su cui hanno lavorato o **-1** in caso di errore.

Ogni utente ha una propria visione dei file aperti; nel caso di più utenti che aprono lo stesso file, ogni processo utente ha un proprio I/O pointer **separato**. Se un utente legge o scrive, modifica solamente il **proprio** pointer e non l'I/O pointer altrui.

## SYSCALL lseek
La system call `lseek` permette l'accesso random ad un file cambiando il numero del prossimo byte da leggere/scrivere.

```C
#include <sys/types.h>
#include <unistd.h>

off_t lseek(int filedes, off_t offset, int start_flag);
```
Dove il parametro `filedes` è un descrittore di file, `offset` determina la posizione nuova del puntatore in lettura/scrittura, il parametro `start_flag` specifica da dove deve essere calcolato l'offset e può assumere uno dei seguenti valori simbolici:
- `SEEK SET (0)`: offset misurato dall'<u>**inizio** del file</u>
- `SEEK CUR (1)`: offset misurato dalla <u>**posizione corrente** del puntatore</u>
- `SEEK END (2)`: offset misurato dalla <u>**fine** del file</u>
E `lseek` ritorna la **nuova posizione del puntatore**.

# Shell: Basi
Alcune note:
```bash
touch nomefile      # crea un file vuoto
test -f nomefile    # verifica l'esistenza di un file (true vale 0)
[-f nomefile]       # verifica l'esistenza di un file (se esiste ritorna 0)
$?                  # stampa il valore di ritorno dell'ultimo comando eseguito nella shell
man nomecomando     # stampa il manuale contenente le informazioni (ad es. parametri) su un determinato comando
```
```bash
[ -f /etc/passwd ] && echo "File exists" || echo "File does not exist"
[ -d /var/log ] && echo "Directory exists" || echo "Directory does not exist"
```
E gli esercizi:
```bash
# ES 1
user@:~$ [-f piripacchio]
user@:~$ echo $?
1
user@:~$ touch piripacchio
user@:~$ [-f piripacchio]
user@:~$ echo $?
0
```
```bash
# ES 2
user@:~$ test -f piripacchio
user@:~$ echo $?
0
user@:~$ rm piripacchio
user@:~$ test -f piripacchio
user@:~$ echo $?
```
```bash
user@:~$ type ls
ls is hashed (/bin/ls)
user@:~$ type [
[ is a shell builtin
user@:~$ type test
test is a shell builtin
```

## Variabili d'ambiente
La shell bash utilizza le variabili di ambiente per tenere traccia di informazioni riguardanti la sessione corrente e l'ambiente di lavoro; queste permettono anche all'utente di immagazzinare in **memoria** informazioni accessibili da qualsiasi programma che viene eseguito all'<u>interno della shell</u>. Alcune, se associate in modo particolare ad ogni specifico utente, possono essere impostate <u>durante l'avvio del sistema</u>. Questo è un modo efficace di salvare informazioni inerenti a utenti, sistema o alla shell stessa in modo che siano facilmente *accessibili*. 

Le variabili d'ambiente che vengono usate a livello di sistema hanno nomi formati da lettere maiuscole (per convenzione).

### Globali
Le variabili d'ambiente definite a livello di sistema (**globali**), quindi quelle visibili all'interno della shell e di qualsiasi altro processo (anche altre shell) eseguito all'interno della shell originale, possono essere visualizzate usando il comando `printenv`.
```bash
user@:~$ printenv
``` 
### Locali
Le variabili d'ambiente valide solamente all'interno di una determinata sessione shell sono dette variabili **locali**, non esiste un comando dedicato per la visualizzazione delle sole variabili d'ambiente locali. Però, è possibile usare il comando `set` senza argomenti:
```bash
# ES 3
user@:~$ vloc1="a"          # var locale 1
user@:~$ set > prova1.txt   # set + redirezione
user@:~$ bash               # ho 2 shell ora
user@:~$ vloc2="b"          # var locale 2
user@:~$ set > prova2.txt   # set + redirezione
user@:~$ exit
```
Esamina con *vim* il contenuto di `prova1.txt` e `prova2.txt`, dove sono `vloc1` e `vloc2`? Quali conclusioni possiamo trarre?
```bash
user@:~$ diff prova1.txt prova2.txt
# vede riga per riga se sono uguali e se trova differenze mostra le righe diverse
```
Una volta che ho creato una variabile, per rimuoverla ho il comando `unset`:
```bash
# ES 4
user@:~$ var1=”test”    # creazione variabile locale
user@:~$ echo $var1     # visualizza
test                    # var1
user@:~$ unset var1     # distruzione variabile
user@:~$ echo $var1
                        # vuoto
user@:~$ var1=”globaltest”
user@:~$ export var1    # esportazione variabile (diventa globale)
user@:~$ printenv
user@:~$ unset var1     # distruzione variabile
```

## Caratteri speciali

### Commento
`#` è il carattere per commentare:
```bash
echo "Segue un bel commentazzo." # commentazzo
```
Commenti, apici, sostituzioni, conversioni...
```bash
echo "Il presente # non inizia un commento"
echo 'Il presente # non inizia un commento.'
echo Il presente \# non inizia un commento.
echo Il presente # inizia un commento.

echo ${PATH#*.}  # è una sostituzione di parametro, non un commento
echo $(( 2#101011 )) # è una conversione di base, non un commento
```
Per evitare la reinterpretazione di `#` come inizio commento, basta mettere `\` in precedenza.
```bash
user@:~$ echo bruh # aint no way
bruh
user@:~$ echo bruh \# aint no way
bruh aint no way
```

### Separatore
`;` è il carattere per separare i comandi:
```bash
echo 'stringa1' ; echo 'stringa2'
```

### Delimitatore opzione case
`;;` viene usato per questo scopo:
```bash
case "$variabile" in 
abc) echo "$variabile = abc" ;;
xyz) echo "$variabile" = xyz ;;
esac
```

### Altri
`.` il punto è un builtin bash, equivale a **source**. Se voglio caricare un file contenente ad esempio variabili posso scrivere `.nomefile`, si usa negli script. Il file da caricare dev'essere necessariamente presente nella PWD (directory di lavoro corrente).

> [!WARNING]
> In Linux/UNIX, il punto `.` nel nome dei file ha un significato importante; se il nome del file inizia con esso, significa che si tratta di un file **nascosto**. Si può visualizzare le informazioni su esso mediante `ls -la` (dove `a` è un parametro che dice a `ls` di mostrare tutti i file (`all`) anche quelli nascosti)

> [!IMPORTANT]
> Il punto `.` si usa anche nelle ricerche di carattere; in quel caso, il punto indica **un solo carattere**

`"` quoting parziale, doppio apice; preserva il contenuto, stringa, evitando che caratteri speciali al suo interno vengano interpretati come tali. Anche l'apice singolo `'` ha lo stesso effetto, ma è più *restrittivo* del doppio.

`,` la virgola concatena una serie di operazioni aritmetiche che vengono valutate tutte, ma viene restituita <u>solo</u> l'ultima. 

`\` escape, effettua il quoting (preserva) caratteri singoli: viene usato per il quoting di `"` o `'` in modo che vengano interpretati letteralmente

` è l'apice inverso, sostituzione comando, e serve a restituire il risultato del comando posto tra apici inversi in modo da renderlo disponibile per l'assegnamento a una variabile.

`:` comando NULL, i due punti è l'equivalente nell'istruzione NOP, non far niente. Da considerarsi un sinonimo del comando di shell **true**. Combinato alla redirezione `>` può azzerare il contenuto; ha lo stesso effetto di un `cat/dev/null > nomefile`.
```bash
: > nomefile.xxx # ora nomefile.xxx è vuoto
```

`!` inversione o negazione, inverte il significato di un test logico o di un comando (es. `!=` è "diverso")

`$` sostituzione di variabile, `$nomevar` accede al contenuto della variabile `nomevar`.

`$$` PID dello script in esecuzione

`*` jolly espansione MULTIcarattere

`?` jolly SINGOLO carattere, `cas?` dà match con cas**a** e cas**o**

`()` gruppo di comandi (es. `a=ciao ; echo $a`). Usate anche per inizializzare array: `a = (var1 var2)`

> [!CAUTION]
> Le parentesi `()` danno origine ad una subshell, quindi le variabili locali alla parent shell non sono viisbili alla child subshell

`{}` espansione multipla
```bash
cat {file1, file2, file3} > nomenuovofile
cp nomefile.{txt,bkp} # equivale a cp nomefile.txt nomefile.bkp
```
Viene usato dal comando **xargs** per creare liste di argomenti.

## Redirezioni
Ad ogni processo sono sempre associati tre file già aperti:
- Standard Input (termiinale, tastiera)
- Standard Output (terminale, video)
- Standard Error (terminale, video, usato per segnalare errore/i)
Possono essere rediretti:
- `/usr/bin/sort > lista`
    - lo `stdin` è il file `lista`
- `/bin/ls > lista`
    - lo `stdout` è il file `lista`
- `/bin/ls piripacchio 2 > lista`
    - lo `stderr` è il file `lista`
- `(echo ciao & date ; ls piripacchio) 2 > errori 1 > output`

Quindi la **pipe** si conferma come un canale bufferizzato, analogo ad un file, in cui un processo scrive e un altro legge; Con la pipe è possibile collegare lo `stdout` di un programma con lo `stdin` di un altro. Con la shell è possibile collegare 2 processi tramite una pipe anonima.

Lo `stdout` del primo diventa lo `stdin` del secondo.
```bash
/bin/ls | sort
ls -lR / | sort | more
```
è funzionalmente equivalernte a 
```bash
ls -lR > tmp 1 ; sort < tmp1 > tmp2 ; more < tmp2 ; rm tmp*
```
Molti programmi copiano lo `stdin` su `stdout` dopo averlo elaborato, sono detti **filtri**.

Per usare l'output di un programma sulla riga di comando di un altro programma, occorre usare la command substitution:
```bash
/bin/ls -l $(/usr/bin/which sort)
```

Es. fusione canali:
```bash
2>&1
```

## Esercizi

#### 1. Scrivere una **pipeline** di comandi che identifichi le informazioni sul processo `dropbear (ps, grep)`
Apparentemente, il problema potrebbe essere risolto dal solo uso di `ps` e `grep` ma quante righe si ottengono con questa pipeline?
```bash
ps aux | grep 'dropbear'
```
Dovrei ottenere 2 righe. Per verificare cosa contiene la seconda e perchè la otteniamo:
```bash
ps aux | grep 'dropbear' | wc -l
```
E se vogliamo solo la prima:
```bash
ps aux | grep 'dropbear' | head -n1
```

*da finire lezione 4 - slide 47 in poi*