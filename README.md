# Przykłady C POSIX
POSIX jest standardem interfejsu systemów operacyjnych Unix.

## Przykłady
- Komunikacja wątków przez potok;
- Komunikacja wątków przez potok nazwany;
- Komunikacja wątków przez pamięć współdzieloną;
- Obsługa sygnałów;

Dokładny opis działania znajduje się w komentarzach programów

## Kompilacja
Każdy z programów kompilujemy osobno poleceniem:
```
gcc -Wall -pedantic -o <nazwa_progrmau> <nazwa_pliku_zrodlowego>
```

Lub wszystkie jednocześnie:
```
. skompiluj_wszystkie.sh
```