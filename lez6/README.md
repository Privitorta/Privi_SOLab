# SO(LAB) 30 Ottobre 2025

## Istruzioni PAR1
Per accedere al server d'esame: UTILIZZARE IL FOGLIO CREDENZIALI PRESENTE SULLA POSTAZIONE. 
- **prima riga**:	comando da inserire in bash per fare login
- **seconda riga**:	password da utilizzare durante il login

Nel server d'esame potrete utilizzare comandi aggiuntivi per ottenere informazioni utili alla soluzione di domanda 1 e per scrivere e testare la soluzione da sottomettere per domanda 2. 

**ATTENZIONE**: se lasciate il server inattivo dopo un po' va in freeze ... In tal
caso chiudete il terminale, aprite un altro terminale e rieffettuate il login.

Esempio di pipeline utile per ottenere conferma di codici syscall:
```bash
printf SYS_read | gcc -include sys/syscall.h -m32 -E - | grep -v '#' | grep -v -e '^$'
```

### Domanda 1
Dati i file `.asm`, rispondere alle seguenti domande riguardanti `test.asm` (non modificare gli altri file), commentando le righe corrispondenti ed iniziando il commento con `;`

- Cosa stiamo caricando in eax a riga 12 di `test.asm`?<br>
- Commentate le righe 15 e 16 di `test.asm`<br>
- Commentate le righe da 25 a 28 (incluse) di `test.asm`

```asm
------------------------------------------
functions.asm ----------------------------
------------------------------------------

;-----------------------------------------
; int slen(String message)
; String length calculation function
slen:
    push    ebx
    mov     ebx, eax
 
nextchar:
    cmp     byte [eax], 0
    jz      finished
    inc     eax
    jmp     nextchar
 
finished:
    sub     eax, ebx
    pop     ebx
    ret
 
;-----------------------------------------
; void sprint(String message)
; String printing function
sprint:
    push    edx
    push    ecx
    push    ebx
    push    eax
    call    slen
 
    mov     edx, eax
    pop     eax
 
    mov     ecx, eax
    mov     ebx, 1
    mov     eax, 4
    int     80h
 
    pop     ebx
    pop     ecx
    pop     edx
    ret
 
 
;-----------------------------------------
; void exit()
; Exit program and restore resources
quit:
    mov     ebx, 0
    mov     eax, 1
    int     80h
    ret


------------------------------------------
test.asm ---------------------------------
------------------------------------------
 
1  %include        'functions.asm'
2  
3  SECTION .data
4  aMsg        db      'This is message a', 0h    
5  bMsg        db      'This is message b', 0h    
6  
7  SECTION .text
8  global  _start
9  
10 _start:
11
12 mov     eax, 2       ; carico in eax la syscall della fork (id 2)          
13 int     80h
14
15 cmp     eax, 0       ; confronto il contenuto di eax con 0, se è 0 => ZF = 1     
16 jz      a            ; se ho come risultato del confronto 0, quindi ZF = 1, salto all'etichetta a
17
18 b:                
19 mov     eax, bMsg      
20 call    sprint        
21
22 call    quit            
23  
24 a:                
25 mov     eax, aMsg    ; carico nel registro eax l'indirizzo di memoria della label aMsg, che è una stringa 
26 call    sprint       ; chiamata alla funzione "string" contenuta in functions.asm, funzione che stampa una stringa
27  
28  call    quit        ; chiamata alla funzione "quit" contenuta in functions.asm, funzione che esce dal programma e ripristina i registri
29  
-----------------------------------------
```

### Domanda 2
Utilizzare `strace` (linux syscall `tracer`) per listare tutte le chiamate di sistema effettuate durante l'esecuzione del comando `df -h`, il cui nome inizi con la lettera `s` ed il cui nome abbia, in terza posizione, una vocale. <u>Ordinarle per **frequenza** di **chiamata** ed estrarre  la chiamata di sistema effettuata piu' frequentemente</u>. **Stampare** in **output** `n syscallname` dove `n` e' il numero di occorrenze della chiamata  a syscallname. Risolvete l'esercizio utilizzando una *pipeline*. <u>**Non** utilizzate i seguenti parametri per strace</u>: `-c` , `-C`.
```bash
strace df -h 2>&1 | grep '(' | cut -d '(' -f 1 | grep -E '^[SINGOLALETTERASCELTA].[aiuoe].*' | sort | uniq -c | sort -nr | head -n 1 | sed 's/^ *//g'
```
Output:
```
24 stat	    (se, come lettera scelta utilizzate s)
1 write	    (se, come lettera scelta utilizzate w)
3 openat	(se, come lettera scelta utilizzate o)
```

#### Spiegazione blocchi pipeline
```bash
strace df -h 2>&1
```
`strace` comando_e_parametri `2>&1`, esecuzione di `strace`. MOLTA attenzione a `2>&1` è FUSIONE di STDERR+STDOUT

```bash
grep '('		
```
filtra (mantieni) le sole righe che contengono il carattere `(`

```bash
cut -d '(' -f 1
```		
assicurata la presenza di `(` lo si usa come delimitatore per estrarre NOME della syscall

```bash
grep -E '^[SINGOLALETTERASCELTA].[aiuoe].*'
```	
`grep -E` (esteso): usa classi (gruppi) di caratteri tra `[]`. 
e' singolo carattere `*` vuol dire un numero qualsiasi di qualsiasi carattere (anche sequenza caratteri diversi tra loro)

```bash
sort | uniq -c | sort -nr	
```
blocchi di conteggio (sempre 3, al centro `unique -c`)

```bash
head -n 1		
```
estrazione prima riga (conteggio piu' elevato dato `-r` in blocco sort precedente)

```bash
sed 's/^ *//g'
```		
rimozione caratteri spaziatori in parte iniziale output

## Esempio
Scrivi una *pipeline* di soli 2 blocchi in grado di creare `x` file, ognuno di essi deve avere il nome che soddisfa il formato `SysoplabShell_x.txt`
```bash
seq 10 | xargs -I {} touch SysoplabShell2_{}.txt
```

Analizzare lo storico dei comandi in modo da identificare il comando contenente la pipeline più lunga (senza `awk`).
```bash
history | sed -e's/^\s*//' | cut -d ' ' -f2
```
Alternativa del grande Ivan che però non si adatta molto bene
```bash
history | sed -e's/^\s*//' -e 's/ //' | cut -d ' ' -f2
```