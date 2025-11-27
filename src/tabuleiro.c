/**
 * src/tabuleiro.c – versão com TUBARÃO EM EMOJI (🦈) e JOGADOR SURFISTA (🏄)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tabuleiro.h"
#include "../include/screen.h"

// Borda 100% ASCII
#define BORDA_CANTOS       "+"
#define BORDA_HORIZONTAL   "-"
#define BORDA_VERTICAL     "|"

Tabuleiro* criar_tabuleiro(int linhas, int colunas) {
    Tabuleiro *tab = (Tabuleiro*)malloc(sizeof(Tabuleiro));
    if (!tab) return NULL;

    tab->linhas = linhas;
    tab->colunas = colunas;

    tab->matriz = (char**)malloc(linhas * sizeof(char*));
    if (tab->matriz == NULL) {
        free(tab);
        return NULL;
    }
    
    for (int i = 0; i < linhas; i++) {
        tab->matriz[i] = (char*)malloc(colunas * sizeof(char));
        for (int j = 0; j < colunas; j++) {
            tab->matriz[i][j] = '.';  // Água
        }
    }

    return tab;
}

void destruir_tabuleiro(Tabuleiro *tab) {
    if (!tab) return;
    for (int i = 0; i < tab->linhas; i++) free(tab->matriz[i]);
    free(tab->matriz);
    free(tab);
}

void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY) {
    if (!tab) return;

    int L = tab->linhas;
    int C = tab->colunas;

    screenSetColor(WHITE, BLACK);

    // Borda superior
    screenGotoxy(MINX, MINY);
    printf("%s", BORDA_CANTOS);
    for (int i = 0; i < C; i++) printf("%s%s", BORDA_HORIZONTAL, BORDA_HORIZONTAL); 
    printf("%s", BORDA_CANTOS);

    // Corpo
    for (int y = 0; y < L; y++) {
        int screenY = MINY + 1 + y;

        screenGotoxy(MINX, screenY);
        screenSetColor(WHITE, BLACK);
        printf("%s", BORDA_VERTICAL);

        for (int x = 0; x < C; x++) {

            // JOGADOR SURFISTA
            if (x == jogadorX && y == jogadorY) {
                screenSetColor(WHITE, BLUE);
                printf("%s", EMOJI_JOGADOR);
                screenSetColor(WHITE, BLACK); 
                continue;
            }

            // TUBARÃO (S)
            if (tab->matriz[y][x] == 'S') {
                screenSetColor(WHITE, BLACK);
                printf("%s", EMOJI_TUBARAO); 
                continue;
            }

            // ÁGUA (.)
            screenSetColor(CYAN, BLACK); 
            printf(". "); 
        }

        screenSetColor(WHITE, BLACK);
        printf("%s", BORDA_VERTICAL);
    }

    // Borda inferior
    screenGotoxy(MINX, MINY + L + 1);
    printf("%s", BORDA_CANTOS);
    for (int i = 0; i < C; i++) printf("%s%s", BORDA_HORIZONTAL, BORDA_HORIZONTAL);
    printf("%s", BORDA_CANTOS);

    screenUpdate();
}