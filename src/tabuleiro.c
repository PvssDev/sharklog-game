#include <stdio.h>
#include <stdlib.h>
#include "tabuleiro.h"
#include "screen.h"

#define BORDA_TOP_LEFT     "╔"
#define BORDA_TOP_RIGHT    "╗"
#define BORDA_BOTTOM_LEFT  "╚"
#define BORDA_BOTTOM_RIGHT "╝"
#define BORDA_HORIZONTAL   "═"
#define BORDA_VERTICAL     "║"


Tabuleiro* criar_tabuleiro(int linhas, int colunas) {
    Tabuleiro *tab = (Tabuleiro*)malloc(sizeof(Tabuleiro));
    tab->linhas = linhas;
    tab->colunas = colunas;

    tab->matriz = (char**)malloc(linhas * sizeof(char*));
    for (int i = 0; i < linhas; i++) {
        tab->matriz[i] = (char*)malloc(colunas * sizeof(char));

        for (int j = 0; j < colunas; j++) {
            tab->matriz[i][j] = ':'; // fundo padrão
        }
    }

    return tab;
}


void destruir_tabuleiro(Tabuleiro *tab) {
    for (int i = 0; i < tab->linhas; i++) {
        free(tab->matriz[i]);
    }
    free(tab->matriz);
    free(tab);
}


void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY) {

    int L = tab->linhas;
    int C = tab->colunas;

    // ----------- BORDA SUPERIOR -----------
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MINX, MINY);
    printf(BORDA_TOP_LEFT);

    for (int i = 0; i < C; i++)
        printf(BORDA_HORIZONTAL);

    printf(BORDA_TOP_RIGHT);


    // ----------- CORPO DO TABULEIRO -----------
    for (int y = 0; y < L; y++) {

        screenGotoxy(MINX, MINY + 1 + y);
        printf(BORDA_VERTICAL); // parede esquerda

        for (int x = 0; x < C; x++) {

            // Jogador
            if (x == jogadorX && y == jogadorY) {
                screenSetColor(RED, BLACK);
                printf("P");
                screenSetColor(LIGHTBLUE, BLACK);
                continue;
            }

            // Conteúdo normal
            screenSetColor(LIGHTBLUE, BLACK);
            printf("%c", tab->matriz[y][x]);
        }

        screenSetColor(WHITE, BLACK);
        printf(BORDA_VERTICAL); // parede direita
    }


    // ----------- BORDA INFERIOR -----------
    screenGotoxy(MINX, MINY + L + 1);
    printf(BORDA_BOTTOM_LEFT);

    for (int i = 0; i < C; i++)
        printf(BORDA_HORIZONTAL);

    printf(BORDA_BOTTOM_RIGHT);

    screenSetColor(WHITE, BLACK);
}
