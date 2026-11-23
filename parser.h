#ifndef PARSER_H
#define PARSER_H

#define MAKS_ZMIENNYCH 32

struct Zmienna {
    char nazwa[32];
    int wartosc;
};

int oblicz_wyrazenie(
    const char tekst[],
    struct Zmienna zmienne[],
    int *liczba_zmiennych
);

void wykonaj_przypisanie(
    const char tekst[],
    struct Zmienna zmienne[],
    int *liczba_zmiennych
);
void wykonaj_print(
    const char tekst[],
    struct Zmienna zmienne[],
    int *liczba_zmiennych
);

void wykonaj_linie(
    const char tekst[],
    struct Zmienna zmienne[],
    int *liczba_zmiennych
);

#endif