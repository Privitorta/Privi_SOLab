# Esercizi vari

> [!IMPORTANT]
> Runna `sudo -s` prima di provarli nell'ambiente di esercitazione dato.

## Esercizio 1

### Obiettivo
Costruire una pipeline che, nel ramo /usr/include, individui il file di intestazione (`*.h`) con il maggior numero di righe di commento, cioè righe che iniziano con `//` o contengono un commento `\*...*/` in linea singola. Il risultato deve essere nel formato: `n percorso_assoluto_file`.

### Soluzione
```bash
find /usr/include -type f -name '*.h' -print0 | xargs -0 grep -E '^[[:space:]]*//|/\*.*\*/' -H | cut -d':' -f1 | sort | uniq -c | sort -nr | head -1 | sed 's/^[ \t]*//'
```

> [!NOTE]
> `sed 's/^[ \t]*//'` serve a eliminare, come richiesto all'esame, eventuali spazi e tabulazioni che si formano naturalmente nel corso della concatenazione delle pipe. Sarà presente alla fine di ogni pipeline come comando di formattazione output.

## Esercizio 2

### Obiettivo
Costruire una pipeline che, nel ramo `/usr/bin`, identifichi l’eseguibile più grande (in byte) e stampi: `dimensione percorso_assoluto_file`.

### Soluzione
```bash
find /usr/bin -type f -executable -print0 | xargs -0 du -b | sort -n | tail -1 | sed 's/^[ \t]*//'
```

## Esercizio 3

### Obiettivo
Costruire una pipeline che, all’interno di /etc, identifichi il file contenente il maggior numero di indirizzi IPv4 validi formato `x.x.x.x`, con $x∈[0,255]$. Il risultato deve essere nel formato: `n percorso_assoluto_file`.

### Soluzione
```bash
grep -r -EoH '([0-9]{1,3}\.){3}[0-9]{1,3}' /etc | cut -d: -f1 | sort | uniq -c | sort -nr | head -1 | sed 's/^[ \t]*//'
```

## Esercizio 4
### Obiettivo
Trova la parola più frequente in file `.txt` nella cartella corrente.
### Soluzione
```bash
grep -hoE '[[:alpha:]]+' *.txt | sort | uniq -c | sort -nr | head -1 | sed 's/^[ \t]*//'
```

## Esercizio 5
### Obiettivo
Conta file di testo più grandi di 1KB nella tua home e ordina per dimensione decrescente. 
### Soluzione
```bash
find ~/ -type f -size +1k -exec ls -lS {} + | sed 's/^[ \t]*//'
```
### Obiettivo 2
Output previsto: `dimensione percorso_assoluto_file`.
### Soluzione
```bash
find ~/ -type f -size +1k -print0 | xargs -0 -n1 ls -l | sort -k5 -nr | sed -E 's/^.+ +([0-9]+) +.+ +.+ +(.+)$/\1 \2/' | sed 's/^[ \t]*//' # mostra total 0 su ambiente, è normale
```
## Esercizio 6
### Obiettivo
 Costruire una pipeline che nel ramo `/var/log` individui il file di log più “verboso”, ossia quello con il maggior numero totale di righe. Il risultato deve essere nel formato: `n percorso_assoluto_file`.
### Soluzione
```bash
find /var/log -type f -exec wc -l {} \; | sort -nr | head -n 1 | sed 's/^[ \t]*//'
```