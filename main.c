
#include <stdio.h>
#include "parser.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Użycie: %s test.py\n", argv[0]);
        return 1;
    }

    FILE *plik = fopen(argv[1], "r");
    if (plik == NULL) {
        perror("Nie można otworzyć pliku");
        return 1;
    }

    struct Zmienna zmienne[MAKS_ZMIENNYCH] = {0};
    int liczba_zmiennych = 0;
    char linia[256];

    while (fgets(linia, sizeof linia, plik) != NULL) {
        wykonaj_linie(linia, zmienne, &liczba_zmiennych);
    }

    if (ferror(plik)) {
        perror("Błąd odczytu pliku");
        fclose(plik);
        return 1;
    }

    fclose(plik);
    return 0;
}
