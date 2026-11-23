#include "lexer.h"



static int czy_litera(char znak) {
    return (znak >= 'a' && znak <= 'z') ||
           (znak >= 'A' && znak <= 'Z');
}

static int czy_cyfra(char znak) {
    return znak >= '0' && znak <= '9';
}

struct Token pobierz_token(const char tekst[], int *i) {
    // 1. NAJPIERW pomijamy wszystkie białe znaki
    while (tekst[*i] == ' ' || tekst[*i] == '\t' || tekst[*i] == '\n') {
        (*i)++;
    }

    // 2. Sprawdzamy koniec wejścia
    if (tekst[*i] == '\0') {
        struct Token t = {TOKEN_EOF, 0};
        return t;
    }

    // 3. Teraz sprawdzamy liczby
    if (tekst[*i] >= '0' && tekst[*i] <= '9') {
        int value = 0;
        while (tekst[*i] >= '0' && tekst[*i] <= '9') {
            value = value * 10 + (tekst[*i] - '0');
            (*i)++;
        }
        struct Token t = {TOKEN_NUMBER, value};
        return t;
    }

    // 4. Operatory i nawiasy
    if (tekst[*i] == '+') {
        (*i)++;
        struct Token t = {TOKEN_PLUS, 0};
        return t;
    }
    if (tekst[*i] == '-') {
        (*i)++;
        struct Token t = {TOKEN_MINUS, 0};
        return t;
    }
    if (tekst[*i] == '*') {
        (*i)++;
        struct Token t = {TOKEN_STAR, 0};
        return t;
    }
    if (tekst[*i] == '(') {
        (*i)++;
        struct Token t = {TOKEN_LPAREN, 0};
        return t;
    }
    if (tekst[*i] == ')') {
        (*i)++;
        struct Token t = {TOKEN_RPAREN, 0};
        return t;
    }
    if (czy_litera(tekst[*i]) || tekst[*i] == '_') {
    struct Token t = {0};
    t.type = TOKEN_IDENTIFIER;

    int dlugosc = 0;
    int za_dluga = 0;

    while (czy_litera(tekst[*i]) ||
           czy_cyfra(tekst[*i]) ||
           tekst[*i] == '_') {
        if (dlugosc < (int)sizeof t.nazwa - 1) {
            t.nazwa[dlugosc] = tekst[*i];
            dlugosc++;
        } else {
            za_dluga = 1;
        }

        (*i)++;
    }

    t.nazwa[dlugosc] = '\0';

    if (za_dluga) {
        t.type = TOKEN_UNKNOWN;
    }

    return t;
}
    if (tekst[*i] == '=') {
        (*i)++;
        struct Token t = {0};
        t.type = TOKEN_ASSIGN;
        return t;
}

    (*i)++;
    struct Token t = {TOKEN_UNKNOWN, 0};
    return t;
}