# Testowanie czasów dostępu do pamięci DDR - Projekt OiAK
Program powstał w ramach projektu przedmiotu Organizacja i Architektura Komputerów.

Jest on częścią realizacji tematu 15 - "Badanie struktury pamięci DDR na podstawie czasów dostępu"

## Kompilacja

Program można skompilować kompilatorem g++ bez żadnych dodatkowych opcji.


## Użycie
Program pozwala na wybranie jednego z dwóch trybów:

- pomiar dwóch odczytów z RAM

- pomiar jednego odczytu i wykonanie pierwszego przed rozpoczęciem pomiaru

Następnie wczytywane są w postaci indeksu wiersza i kolumny tablicy dwuwymiarowej trzy adresy:

- adres pierwszego elementu pary
- początek zakresu testowanych drugich elementów pary
- koniec zakresu testowanych drugich elementów pary

Ostatecznie należy podać krok z jakim wykonane będą pomiary i ilość testów w serii pomiarów dla jednej pary.

Wyniki zostają wygenerowane w postaci dwóch plików tekstowych: "results /data/" oraz "results ŚREDNIE /data/".
