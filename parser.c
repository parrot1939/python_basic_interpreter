#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"

struct Parser {
    const char *tekst;
    int indeks;
    struct Token aktualny_token;
    struct Zmienna *zmienne;
    int *liczba_zmiennych;
};

static void wczytaj_nastepny_token(struct Parser *parser) {
    parser->aktualny_token =
        pobierz_token(parser->tekst, &parser->indeks);
}

static void inicjalizuj_parser(
    struct Parser *parser,
    const char tekst[],
    struct Zmienna zmienne[],
    int *liczba_zmiennych
) {
    parser->tekst = tekst;
    parser->indeks = 0;
    parser->zmienne = zmienne;
    parser->liczba_zmiennych = liczba_zmiennych;

    wczytaj_nastepny_token(parser);
}

static int czytaj_wyrazenie_z_tokenow(struct Parser *parser);

static int czytaj_liczbe_z_tokena(struct Parser *parser) {
    if (parser->aktualny_token.type != TOKEN_NUMBER) {
        printf("Oczekiwano liczby\n");
        exit(1);
    }

    int wartosc = parser->aktualny_token.value;
    wczytaj_nastepny_token(parser);
    return wartosc;
}

static int czytaj_czynnik_z_tokenow(struct Parser *parser) {
    if (parser->aktualny_token.type == TOKEN_NUMBER) {
        return czytaj_liczbe_z_tokena(parser);
    }

    if (parser->aktualny_token.type == TOKEN_MINUS) {
        wczytaj_nastepny_token(parser);
        return -czytaj_czynnik_z_tokenow(parser);
    }

    if (parser->aktualny_token.type == TOKEN_IDENTIFIER) {
        for (int j = 0; j < *parser->liczba_zmiennych; j++) {
            if (strcmp(
                    parser->zmienne[j].nazwa,
                    parser->aktualny_token.nazwa
                ) == 0) {
                int wartosc = parser->zmienne[j].wartosc;
                wczytaj_nastepny_token(parser);
                return wartosc;
            }
        }

        printf("Nieznana zmienna: %s\n", parser->aktualny_token.nazwa);
        exit(1);
    }

    if (parser->aktualny_token.type == TOKEN_LPAREN) {
        wczytaj_nastepny_token(parser);
        int wynik = czytaj_wyrazenie_z_tokenow(parser);

        if (parser->aktualny_token.type != TOKEN_RPAREN) {
            printf("Oczekiwano ')'\n");
            exit(1);
        }

        wczytaj_nastepny_token(parser);
        return wynik;
    }

    printf("Oczekiwano liczby, zmiennej lub '('\n");
    exit(1);
}

static int czytaj_iloczyn_z_tokenow(struct Parser *parser) {
    int wynik = czytaj_czynnik_z_tokenow(parser);

    while (parser->aktualny_token.type == TOKEN_STAR) {
        wczytaj_nastepny_token(parser);
        wynik *= czytaj_czynnik_z_tokenow(parser);
    }

    return wynik;
}

static int czytaj_wyrazenie_z_tokenow(struct Parser *parser) {
    int wynik = czytaj_iloczyn_z_tokenow(parser);

    while (parser->aktualny_token.type == TOKEN_PLUS ||
           parser->aktualny_token.type == TOKEN_MINUS) {
        TokenType znak = parser->aktualny_token.type;
        wczytaj_nastepny_token(parser);

        int drugi_iloczyn = czytaj_iloczyn_z_tokenow(parser);

        if (znak == TOKEN_PLUS) {
            wynik += drugi_iloczyn;
        } else {
            wynik -= drugi_iloczyn;
        }
    }

    return wynik;
}

int oblicz_wyrazenie(
    const char tekst[],
    struct Zmienna zmienne[],
    int *liczba_zmiennych
) {
    struct Parser parser;
    inicjalizuj_parser(&parser, tekst, zmienne, liczba_zmiennych);

    int wynik = czytaj_wyrazenie_z_tokenow(&parser);

    if (parser.aktualny_token.type != TOKEN_EOF) {
        printf("Nieoczekiwany token na końcu wyrażenia\n");
        exit(1);
    }

    return wynik;

}


void wykonaj_przypisanie(
    const char tekst[],
    struct Zmienna zmienne[],
    int *liczba_zmiennych
) {
    struct Parser parser;
    inicjalizuj_parser(&parser, tekst, zmienne, liczba_zmiennych);

    if (parser.aktualny_token.type != TOKEN_IDENTIFIER) {
        printf("Oczekiwano nazwy zmiennej\n");
        exit(1);
    }

    char nazwa[32];
    strcpy(nazwa, parser.aktualny_token.nazwa);
    wczytaj_nastepny_token(&parser);

    if (parser.aktualny_token.type != TOKEN_ASSIGN) {
        printf("Oczekiwano '='\n");
        exit(1);
    }

    wczytaj_nastepny_token(&parser);
    int wartosc = czytaj_wyrazenie_z_tokenow(&parser);

    if (parser.aktualny_token.type != TOKEN_EOF) {
        printf("Nieoczekiwany token po przypisaniu\n");
        exit(1);
    }

    for (int j = 0; j < *liczba_zmiennych; j++) {
        if (strcmp(zmienne[j].nazwa, nazwa) == 0) {
            zmienne[j].wartosc = wartosc;
            return;
        }
    }

    if (*liczba_zmiennych >= MAKS_ZMIENNYCH) {
        printf("Za dużo zmiennych\n");
        exit(1);
    }

    strcpy(zmienne[*liczba_zmiennych].nazwa, nazwa);
    zmienne[*liczba_zmiennych].wartosc = wartosc;
    (*liczba_zmiennych)++;
}


void wykonaj_print(
    const char tekst[],
    struct Zmienna zmienne[],
    int *liczba_zmiennych
) {
    struct Parser parser;
    inicjalizuj_parser(&parser, tekst, zmienne, liczba_zmiennych);

    if (parser.aktualny_token.type != TOKEN_IDENTIFIER ||
        strcmp(parser.aktualny_token.nazwa, "print") != 0) {
        printf("Oczekiwano print\n");
        exit(1);
    }

    wczytaj_nastepny_token(&parser);

    if (parser.aktualny_token.type != TOKEN_LPAREN) {
        printf("Oczekiwano '('\n");
        exit(1);
    }

    wczytaj_nastepny_token(&parser);
    int wynik = czytaj_wyrazenie_z_tokenow(&parser);

    if (parser.aktualny_token.type != TOKEN_RPAREN) {
        printf("Oczekiwano ')'\n");
        exit(1);
    }

    wczytaj_nastepny_token(&parser);

    if (parser.aktualny_token.type != TOKEN_EOF) {
        printf("Nieoczekiwany token po print\n");
        exit(1);
    }

    printf("%d\n", wynik);
}

void wykonaj_linie(
    const char tekst[],
    struct Zmienna zmienne[],
    int *liczba_zmiennych
) {
    int indeks = 0;
    struct Token pierwszy = pobierz_token(tekst, &indeks);

    if (pierwszy.type == TOKEN_EOF) {
        return;
    }

    if (pierwszy.type == TOKEN_IDENTIFIER &&
        strcmp(pierwszy.nazwa, "print") == 0) {
        wykonaj_print(tekst, zmienne, liczba_zmiennych);
    } else {
        wykonaj_przypisanie(tekst, zmienne, liczba_zmiennych);
    }
}