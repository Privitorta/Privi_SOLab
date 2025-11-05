# SO(LAB) 23 Ottobre 2025

## Unix power tools
`find` permette la selezione di file con determinate caratteristiche.
```bash
find percorso predicato
```
Spesso è usato assieme a `xargs` ...
```bash
find percorso predicato | xargs comando
```
... che è funzionalmente equivalente a ...
```bash
comando $(find percorso predicato)
```
... ma evita i problemi di lunghezza della riga di comando, perchè `xargs` si preoccupa di "spezzarla" opportunamente.

Spesso, si vuol fare un'operazione <u>per ogni file trovato con `find`</u>. L'espressione naturale sarebbe:
```bash
for i in $(find percorso predicato); do
    comando $i
done
```
Ma questa forma presenta **due problemi**:
- può eccedere la misura della linea di comando;
- non funziona correttamente se i nomi dei file contengono *spazi*. Un'alternativa è:
```bash
find percorso predicato -print0 | xargs -o -n 1
```
Con `-print0` i file trovati sono separati dal carattere 0 anzichè da spazi e `xargs` riesce ad adattarsi alla forma.

## Esercizio 1
**Trovare il file più grosso in un certo ramo**
1. Copiare alcuni file (es. il cui nome segue un certo pattern) di un ramo in un altro mantenendo la gerarchia delle directory
2. Calcolare lo spazio occupato dai file di proprietà di un certo utente
3. Scrivere un comando che conta quanti file ci sono in un determinato ramo del filesystem
```bash
sudo -s
find /var -type f -print0 # troviamo tutti i file in /var
xargs -0 du # con xargs li passiamo come argomenti a du
sort -n # ordiniamo la lista ottenuta in ordine crescente
tail -1 # estraggo ultimo file (il più grande)
cut -f2 # in ogni riga 2 campi, size rel.path. estrarre il secondo
xargs -I{} du -sh {} # uso in modo combinato xargs e du per l'output finale
```
La soluzione finale della pipeline:
```bash
find /var -type f -print0 | xargs -0 du | sort -n | tail -1 | cut -f2 | xargs -I{} du -sh {}
```
- `find /var -type f -print0` troviamo tutti i file in /var
- `xargs -0 du` con `xargs` li passiamo come argomenti a du
- `sort -n` ordiniamo la lista ottenuta in ordine crescente (numeric)
- `tail -1` estraggo ultimo file (il più grande)
- `cut -f2` prendo il secondo campo (seconda colonna) e lo tolgo
- `xargs -I{} du -sh {}` prende in input tutto ciò che arriva e prepara un'altra stringa

## Esercizio 2
**Copiare alcuni file (es. *il cui nome segue un certo pattern*) di un ramo in un altro mantenendo la <u>gerarchia delle directory**</u>
Supponendo che nel folder corrente esista un folder `localinclude`:
```bash
find /usr/include/ -name 's*.h' | xargs -I{} cp --parents {} ./localinclude
```
Il ramo scelto è `/usr/include`, se volessi la cartella corrente sarebbe `.` ma se io voglio usare una cartella corrente posso usare la copia in una directory che non esiste e verrà creata `./localinclude`.
- `find /usr/include/ -name 's*.h'` trova tutti i file `s*.h` (file di intestazione `.h`) in `/usr/include`
- `xargs -I{} cp --parents {} ./localinclude` mediante `xargs` passiamoli come argomenti a `cp`
Per preservare permessi, timestamp ecc. aggiungere `-p` (`man cp`).

## Esercizio 3
**Calcolare lo spazio occupato dai file di proprietà di un certo utente**
```bash
sudo -s
find /home -user user -exec du -k {} \; # troviamo tutti i file di un utente in un dato ramo e usiamo il parametro exec di find per eseguire du
awk '{ s = s+$1 }END {print " Total used: ",s}' # uso awk per calcolare la somma dello spazio occupato
```
- `find /home -user user -exec du -k {} \;` esegue mandandola come uno stream continuo alla shell e quando vede `;` ha finito, bash "capisce" ed esegue per evitare che esploda la lunghezza della riga di comando
- `awk '{ s = s+$1 }END {print " Total used: ",s}'` quel `$1` è la prima colonna
> [!IMPORTANT]
> Possiamo agire secondo la stessa logica ma selezionando per nome del gruppo: `-group groupname`
```bash
find /home -user user -exec du -k {} \; | awk '{ s = s+$1 }END {print " Total used: ",s}'
```

## Esercizio 4
**Scrivere un comando che conta quanti file ci sono in un determinato ramo del filesystem**
```bash
sudo -s
find /home -type f | wc -l
```
Con `find /home -type f` troviamo tutti i file in un dato ramo e con `wc -l` passiamo tutto a wc con opzione di conteggio righe.

## Archivi
Un archivio *archive* è un file di file, cioè un file che contiene i byte di diversi altri file e i relativi *metadati*. `ar` è l'archiviatore classico, generlmente usato per le librerie (`ar t/usr/lib/i86/libc.a`).
> [!NOTE]
> Prova a idente solo dopo aver deerminato la posizione del file usate `ar`. Per vedere tutti i possibili parametri di `ar`, consulta manuale `man ar`.

C'è anche `tar`, Tape archive, standard *POSIX* (`tar cvf archivio.tar lista_files`); gli archivi possono essere compressi con `compress` o più comunemente con `gzip` o `bzip2`, i file `.zip` sono archivi complessi.
> [!WARNING]
> `tar` non comprime i file; li archivia, si appoggia poi a programmi esterni per comprimere. Puoi decidere se comprimere e in quel caso con quale programma esterno farlo, usando i parametri di `tar` in modo appropriato.

### TAR: archiviazione
`tar` è usato per archiviare rami del filesystem. Ad esempio:
**Creare(`c`) un archivio**:
```bash
tar -cvf <archive_name>.tar
```
**Esaminare(`t`) contenuto di un tarball (file `*.tar`) e stampa lista contenuti**:
```bash
tar -tvf <archive_name>.tar
```
**Estrarre(`x`) file da archivio (extract)**:
```bash
tar -xvf <archive_name>.tar
```
L'uso congiunto di `find` e `tar` (mediante `find ... -exec ...`) il parametro `r` di `tar` aggiunge (uno alla volta) i file trovati da `find` all'archivio senza ricrearlo:
```bash
find /usr/include/ -name 's*.h' -exec tar -rvf file.tar {} \;
```

### TAR: archiviazione + compressione
**Creazione(`c`) di un archivio e compressione con `gzip`**
```bash
tar -cvzf <archive_name>.tar.gz <files>
```
**Estrarre(`x`) i file dall'archivio e decomprimere con `gunzip(z)`**
```bash
tar -xvzf <archive_name>.tar.gz
```
Se, al posto di gzip/gunzip vogliamo utilizzare come programmi di compressione/decompressione `bzip2` / `bunzip2` al posto di `z` dovremo utilizzare `j`. In questo caso l'estensione del file compresso è `*.tar.bz2`. 

## Utility standard
| Prog (sez. man) | Descrizione                                       |
| --------------- | ------------------------------------------------- |
| `uniq(1)`       | report or omit repeated lines                     |
| `cut(1)`        | remove sections from each line of files           |
| `tr(1)`         | translate or delete characters                    |
| `dd(1)`         | convert and copy a file                           |
| `stat(1)`       | display file or file system status                |
| `tee(1)`        | read from stdin and write to stdout               |
| `basename(1)`   | strip directory and suffix from filenames         |
| `dirname(1)`    | strip non-directory suffix from filename          |
| `sed(1)`        | stream editor for filtering and transforming text |
| `seq(1)`        | print a sequence of numbers                       |

> [!CAUTION]
Mai usare `uniq` senza `sort` davanti.