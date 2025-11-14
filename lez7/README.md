# SO(LAB) 30 Ottobre 2025

## Unix software factory
**UNIX** = sistema per *programmatori* (unica tipologia di utente all'inizio degli anni '70), progettato insieme a C (linguaggio).

La filosofia di UNIX si adatta perfettamente al paradigma di sviluppo **edit-compile-debug**, con tool all'avanguardia nell'elaborazione di file di testo (per lo più organizzati per *righe*) e per la scrittura dei programmi di elaborazione stessi (lex, yacc...)
> per filosofia di UNIX si intende piccoli programmi che fanno molto bene una cosa su file

### Edit/Compile
**Editor**: `ed`, `vi`, `emacs` manipolano in modo arbitrario i byte di un file, generalmente interpretandoli come caratteri stampabili (testo)

**Compilatore**: `cc` (`gcc`)
- `cc` sorgente (`.c`) $\rightarrow$ assembly (`.s`)
- `as` assembly $\rightarrow$ oggetto (`.o`)
- `ar` archivia diversi oggetti in una libreria (`.a`)
- `ld` oggetti e librerie $\rightarrow$ eseguibile (`a.out`) (il formato storico è COFF, oggi ELF)

Anche la compilazione vera e propria è fatta da due passi (pre-processore `cpp` e compilazione `cc1`).

## Make
Permette di specificare **dipendenze** fra processi di generazione.
Se cambia un prerequisito, il processo di generazione deve essere ripetuto.

```makefile
helloworld.o:   helloworld.c
            cc -c -o helloworld helloworld.c

helloworld:     helloworld.o
            cc -o $@ $<

.PHONY:     clean
clean:
        rm helloworld.o helloworld
```

Come funziona il file **Makefile**?
```makefile
Hello:  Hello.c
        clang -Wall -o Hello Hello.c
```
La prima linea è il **Dependency Tree**: `Hello:  Hello.c`
- **target** e **sorgenti**
    - **target**: modulo da costruire (es. `Hello`)
    - **sorgenti**: prerequisiti (es. `Hello.c`)

La seconda linea è il **Make rule**: `clang -Wall -o Hello Hello.c`
- comando da eseguire
  - `clang -Wall -o Hello Hello.c`
- necessita il carattere **tab** (`\t`) prima, non spazi, per indentare

Per **target multipli**:
```makefile
Program:    module1.o module2.o

module1.o:  module1.c
        clang -c Wall -o module1.o module1.c

module2.o:  module2.c module2.h
        clang -c Wall -o module2.o module2.c
```
Default target: primo target `Program'
- linka due file oggetto (`module1.o` e `module2.o`) in un programma (`Program`)
 
Secondo target: `module1.o`
- rule che compila file oggetto `module1.o` da `module1.c`
- `clang -c` compila single module (non eseguibile)

Terzo target: `module2.o`
- compila `module2.o` da sorgente `module2.c`
- dipende anche da `module2.h` (header file)

Makefile per compilare più di un programma:
```makefile
all:    Program1 Program2

Program1:   module1.o
        clang -o Program module1.o

Program2:   module2.o module3.o
        clang -o Program module2.o module3.o

module1.o:  module1.c
        clang -c Wall -o module1.o module1.c

module2.o:  module2.c module2.h
        clang -c Wall -o module2.o module2.c

module3.o:  module3.c module3.h
        clang -c Wall -o module3.o module3.c
```
Il target `all` compila tutti i programmi (`Program1` e `Program2`).

### Regole generiche
Evitano la necessità di scrivere molte volte nel temrinale comandi di utilizzo “generale” ed utilizzati molto frequentemente. 

**Incremento consistenza**: è il modo più sicuro di cambiare il modo in cui invochiamo abitualmente compilatore/i.

Utilizzo di liste di **suffissi** per *“trasformare”* (compilare) un tipo di file in un altro:
- `.c.o` per compilare un file `.c` in un file `.o`.

```makefile
.c.o:
        clang -c -Wall -o $*.o $*.c

Program:  module1.o module2.o
        clang -o Program module1.o module2.o

module2.o:  module2.c module2.h
```
`.c.o` compila `.c` a `.o`, `$*` viene sostituito dal filename senza estensioni.

Non serve `module1.o` rule, il compilatore sa già come compilare da `.c` a `.o`, ma a `module2.o` serve rule e dipende da `.h`.

### Variabili Make
Permettono makefile più flessibili, del tipo, se il compiler non fosse chiamato `clang`? Le variabili permettono di assegnare un valore, ad esempio:
```makefile
CC=gcc
```
Le variabili possono essere usate con `$(variabile)`, tipo:
```makefile
$(CC) -c -Wall -o $*.o $*.c
```
cambia `$(CC)` con `gcc`.

### Linguaggi diversi da C
Il make di default conosce solo C, quindi se voglio compilare file diversi da quel linguaggio devo usare i **suffissi**; possono essere specificati così:
```makefile
.SUFFIXES:  .o .m
```
In questo modo un `.o` file può essere anche compilato da un `.m` file (Objective-C).

## Debugger
**Breakpoint**: un punto del programma in cui l'esecuzione deve essere *bloccata*. Tipicamente usato per esaminare lo stato del programma in esecuzione in quell'istante.

**Stepping**: eseguire il programma *passo passo*; la granularità del passo può arrivare fino all'istruzione macchina.

Lo **stato del programma** può essere analizzato come:
- **forma simbolica**
  - secondo i simboli definiti nel linguaggio di alto livello e conservati come *simboli di debugging*
- **memoria virtuale**: stream di byte suddiviso in segmenti.
  - **Text**: contiene le istruzioni (spesso readonly).
  - **Initialized Data Segment**: variabili globali indicizzate.
  - **Uninitialized Data Segment (bss)**: variabili globali non inizializzate
  - **Stack**: collezione di stack frame per le chiamate di procedura; cresce verso il basso.
  - **Heap**: strutture dati create dinamicamente, create verso l'alto tramite syscall `brk` (API `malloc`).

### Uso dei debugger
- `break ...` (un simbolo o un indirizzo `*0x...`)
- `run ...` (eventualmente con `argv`)
- `print ... (x)·`
- `next (nexti)`
- `step (stepi)`
- `backtrace`

## Simboli
La **symbol table** serve al *linker* per associare nomi simbolici e indirizzi prodotti dal compilatore. Contenuta in tutti gli oggetti, generalmente viene lasciata anche negli eseguibili (può essere scartata con `strip`). Una versione più ricca viene detta **"simboli di debug"** (vari formati, es. **DWARF**); le tabelle dei simboli possono essere consultate con `nm`.

Per costruire sistemi operativi talvolta serve **alterare il flusso tradizionale**.
1. `gcc -O -nostdinc -I. -c bootmain.c`
2. `gcc -nodistinc -I. -c bootasm.S`
3. `ld -m elf_i386 -N -e start -Ttext 0x7C00 -o bootblock.o bootasm.o bootmain.o`
4. `objdump -S bootblock.o > bootblock.asm`
5. `objcopy -S -O binary -j .text bootblock.o bootblock`

1. `$ nm kernel | grep _start`
2. `8010b50c D _binary_entryother_start`
3. `8010b4e0 D _binary_intcode_start`
4. `0010000c T _start`

## Comandi GDB
- **Compilazione**: `gcc [other flags] -g <source files> -o <output file>`
- **Avviare GDB session**: `(gdb)`
  - se non si specifica il file si può caricarlo dall'interno di GDB (*comando: file*): `(gdb) file prog1.x`
- **Ottenere informazioni su un comando**: `(gdb) help [command]`
- **Eseguire**: `(gdb) run`

Se tutto procede correttamente, fino al completamento, non si hanno messaggi d'errore. Altrimenti, vengono fornite informazioni sul problema rilevato. Ad esempio:
```
Program received signal SIGSEGV, Segmentation fault.
0x0000000000400524 in sum_array_region (arr=0x7fffc902a270, r1=2, c1=5, r2=4, c2=6) at sum-array-region2.c:12
```

- **Settare breakpoint**: `(gdb) break file1.c:6`
  - posso impostare quanti breakpoint voglio, ogni volta che uno di essi viene aggiornato, l'esecuzione di GDB si ferma
  - si può anche interrompere l'esecuzione in corrispondenza di una funzione, supponiamo di voler fermare all'occorrenza di una certa `int my_func(int a, char*b)`, per impostare un breakpoint in corrispondenza di questa dobbiamo scrivere `(gdb) break my_func`
  - dopo il breakpoint si procede nell'esecuzione runnando `run`, l'esecuzione verrà interrotta di nuovo in corrispondenza dell'eventuale prossimo breakpoint
- **Continua dopo breakpoint**: si può "ignorare" il breakpoint settato con `(gdb) continue`, "passando attraverso"
- **Eseguire istruzioni step by step**: `(gdb) step` è il comando che fornisce controllo estremamente granulare sull'esecuzione di un programma, mostrando le istruzioni eseguite passo passo, a singoli step
