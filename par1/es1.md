# Nome e Cognome più comuni
Hai a disposizione un file `nomi.txt` che contiene riga per riga dei nomi e cognomi di persone.

Esempio:
```
Mario Rossi
Lucia Bianchi
Anna Verdi
Paolo Rossi
Giulia Neri
Marco Bianchi
Sara Verdi
Luca Rossi
```

## Obiettivo
Scrivi un comando Bash che stampi su una sola riga:
```
<nome_più_comune> <cognome_più_comune>
```

Esempio di output atteso:
```
Mario Rossi
```

### Soluzione
```bash
echo "$(cat nomi.txt | cut -d' ' -f1 | sort | uniq -c | sort -nr | head -1 | xargs | cut -d' ' -f2) $(cat nomi.txt | cut -d' ' -f2 | sort | uniq -c | sort -nr | head -1 | xargs| cut -d' ' -f2)"
```
Passo per passo (Nome):
```bash
privitorta@archlinux:~$ cat nomi.txt
Gianluca Piede
Emma Popò
Andrea Corti
Andrea Lunghi
Vittorio Moretti
Simone Lunghi
Simone Colombo
Simone Moretti
Emma Lunghi
Pietro Barbone
Emanuele Degiuli
privitorta@archlinux:~$ cat nomi.txt | cut -d' ' -f1
Gianluca
Emma
Andrea
Andrea
Vittorio
Simone
Simone
Simone
Emma
Pietro
Emanuele
privitorta@archlinux:~$ cat nomi.txt | cut -d' ' -f1 | sort
Andrea
Andrea
Emanuele
Emma
Emma
Gianluca
Pietro
Simone
Simone
Simone
Vittorio
privitorta@archlinux:~$ cat nomi.txt | cut -d' ' -f1 | sort | uniq -c
      2 Andrea
      1 Emanuele
      2 Emma
      1 Gianluca
      1 Pietro
      3 Simone
      1 Vittorio
privitorta@archlinux:~$ cat nomi.txt | cut -d' ' -f1 | sort | uniq -c | sort -nr
      3 Simone
      2 Emma
      2 Andrea
      1 Vittorio
      1 Pietro
      1 Gianluca
      1 Emanuele
privitorta@archlinux:~$ cat nomi.txt | cut -d' ' -f1 | sort | uniq -c | sort -nr | head -1
      3 Simone
privitorta@archlinux:~$ cat nomi.txt | cut -d' ' -f1 | sort | uniq -c | sort -nr | head -1 | xargs
3 Simone
privitorta@archlinux:~$ cat nomi.txt | cut -d' ' -f1 | sort | uniq -c | sort -nr | head -1 | xargs | cut -d' ' -f2
Simone
```
Cognome:
```bash
privitorta@archlinux:~$ cat nomi.txt
Gianluca Piede
Emma Popò
Andrea Corti
Andrea Lunghi
Vittorio Moretti
Simone Lunghi
Simone Colombo
Simone Moretti
Emma Lunghi
Pietro Barbone
Emanuele Degiuli
privitorta@archlinux:~$ cat nomi.txt | cut -d' ' -f2
Piede
Popò
Corti
Lunghi
Moretti
Lunghi
Colombo
Moretti
Lunghi
Barbone
Degiuli
privitorta@archlinux:~$ cat nomi.txt | cut -d' ' -f2 | sort
Barbone
Colombo
Corti
Degiuli
Lunghi
Lunghi
Lunghi
Moretti
Moretti
Piede
Popò
privitorta@archlinux:~$ cat nomi.txt | cut -d' ' -f2 | sort | uniq -c
      1 Barbone
      1 Colombo
      1 Corti
      1 Degiuli
      3 Lunghi
      2 Moretti
      1 Piede
      1 Popò
privitorta@archlinux:~$ cat nomi.txt | cut -d' ' -f2 | sort | uniq -c | sort -nr
      3 Lunghi
      2 Moretti
      1 Popò
      1 Piede
      1 Degiuli
      1 Corti
      1 Colombo
      1 Barbone
privitorta@archlinux:~$ cat nomi.txt | cut -d' ' -f2 | sort | uniq -c | sort -nr | head -1
      3 Lunghi
privitorta@archlinux:~$ cat nomi.txt | cut -d' ' -f2 | sort | uniq -c | sort -nr | head -1 | xargs
3 Lunghi
privitorta@archlinux:~$ cat nomi.txt | cut -d' ' -f2 | sort | uniq -c | sort -nr | head -1 | xargs | cut -d' ' -f2
Lunghi
```
Quindi:
```bash
privitorta@archlinux:~$ echo "$(cat nomi.txt | cut -d' ' -f1 | sort | uniq -c | sort -nr | head -1 | xargs | cut -d' ' -f2) $(cat nomi.txt | cut -d' ' -f2 | sort | uniq -c | sort -nr | head -1 | xargs| cut -d' ' -f2)"
Simone Lunghi
```

La richiesta che viene aggiunta è di scegliere una lettera iniziale a scelta per il nome e il cognome.

## Obiettivo 2
La richiesta che viene aggiunta è di scegliere una lettera iniziale a scelta per il nome e il cognome.

### Soluzione

```bash
echo "$(grep '^E' nomi.txt | cut -d' ' -f1 | sort | uniq -c | sort -nr | head -1 | xargs | cut -d' ' -f2) $(cut -d' ' -f2 nomi.txt | grep '^M' | sort | uniq -c | sort -nr | head -1 | xargs | cut -d' ' -f2)"
```

## Obiettivo 3 (difficile)
Prendere solo nomi e cognomi lunghi almeno `n` char con `n` a scelta.

### Soluzione
Dobbiamo usare le regex con grep; fingiamo di volere solo cognomi e nomi di 5 caratteri, dovremo usare `grep -E '^.{5,}$'`. 
- `^` vincola l’inizio
- `.` indica un carattere qualsiasi
- `{5,}` dice ripeti almeno 5 volte
- `$` vincola la fine della riga (serve per dire che l’intera riga ha quella lunghezza)

Quindi:
```bash
echo "$(cat nomi.txt | cut -d' ' -f1 | grep -E '^.{5,}$' | sort | uniq -c | sort -nr | head -1 | xargs | cut -d' ' -f2) $(cat nomi.txt | cut -d' ' -f2 | grep -E '^.{5,}$' | sort | uniq -c | sort -nr | head -1 | xargs | cut -d' ' -f2)"
```