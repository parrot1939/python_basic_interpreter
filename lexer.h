#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_NUMBER,   // liczba
    TOKEN_PLUS,     // +
    TOKEN_MINUS,    // -
    TOKEN_STAR,     // *
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
    TOKEN_EOF,  // koniec wejścia
    TOKEN_IDENTIFIER, //zmienna
    TOKEN_UNKNOWN,  // nieznany znak
    TOKEN_ASSIGN
} TokenType;



struct Token{
    TokenType type;
    int value;
    char nazwa[32];
    
};

struct Token pobierz_token(const char tekst[], int *i);

#endif