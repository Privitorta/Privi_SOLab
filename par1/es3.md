# Problema con il disco

Uno studente una mattina va a lezione e trova che il suo utente non ha più la possibilità di scrivere su disco nemmeno un bit. Appare un messaggio di errore che lo avvisa che è over quota e che è il caso di eliminare i file in eccesso nella sua home.

L'aula di calcolo funziona in questo modo: la quota massima per la home di un utente è di 1.1 Gb. Se superate quel limite il sistema vi avvisa per 14 giorni che è necessario liberare spazio. Se il problema non viene risolto entro i 14 giorni il sistema blocca la scrittura.

Lo studente SysopLab non si preoccupa eccessivamente anche se il messaggio d'errore dice che i sistemi a finestre potrebbero non funzionare come atteso (si sa che generano valanghe di file temporanei...) allora accede alla postazione dell'aula di calcolo in modalità Linux console e inizia a:

## Obiettivo
Scrivere una pipeline composta esattamente da **3 blocchi** che, se eseguita nella vostra home directory, trova il file o la sottodirectory che occupa più spazio e stampa l'output nel seguente formato:
```
dimensione percorso
```

> [!WARNING] 
> La pipeline **deve** considerare anche eventuali directory/file **nascoste/i** nella vostra home.

> [!NOTE]
> Leggere il manuale del comando `du` e non dimenticare che `du` capisce le **espressioni regolari** nella ricerca dei percorsi.

### Soluzioni
```bash
du -sh -- * .[!.]* ..?* 2>/dev/null | sort -rh | sed -n 1p
```
```
4.0K    .su-to-rootrc
```
Alternativamente, per un output più pulito:
```bash
du -sh -- "$HOME"/* "$HOME"/.[!.]* "$HOME"/..?* 2>/dev/null | sort -rh | sed -n 1p
```
```
4.0K    /home/user/.su-to-rootrc
```

### Spiegazioni
```bash
du -sh -- * .[!.]* ..?* 2>/dev/null
```
- `du -sh` dimensione sommaria, in formato leggibile.
- `--` serve a dire che dopo non ci sono opzioni, solo nomi di file.
- `*` prende tutti i file e directory visibili.
- `.[!.]*` prende file e directory nascosti che iniziano con un singolo punto ma non `.` - o `...`.
- `..?*` prende file e directory nascosti che iniziano con due o più punti (per sicurezza).
- `2>/dev/null` nasconde errori (ad esempio su file a cui non hai accesso).

Mentre:
```bash
du -sh -- "$HOME"/* "$HOME"/.[!.]* "$HOME"/..?* 2>/dev/null
```
- `du -sh` dimensione sommaria, in formato leggibile.
- `--` serve a dire che dopo non ci sono opzioni, solo nomi di file.
- `"$HOME"/*` espande tutti i file e directory visibili dentro la home.
- `"$HOME"/.[!.]*` espande i file e directory nascosti che iniziano con un solo punto `.` seguito da un carattere diverso da `.` (per questo `[!.]`). esclude quindi `.` (la home stessa) e `..` (la directory superiore).
- `"$HOME"/..?*` espande i file e directory nascosti che iniziano con due o più punti (per esempio .git o .mozilla). serve a prendere eventuali nomi che iniziano con più di un punto, perché `.[!.]*` prende solo quelli con un punto e un carattere diverso da punto.
- `2>/dev/null` nasconde errori (ad esempio su file a cui non hai accesso).

```bash
sort -rh
```
ordina dal più grande al più piccolo.
```bash
sed -n 1p
```
stampa solo la prima riga, cioè il più grande.

## Soluzioni alternative
```bash
du /home -ah | sort -hr | head -1
```

```bash
find /home -type f -exec du -h {} \; | sort -h | tail -n 1 
```