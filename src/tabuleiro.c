#include <stdio.h>
#include <stdlib.h>
#include "../include/tabuleiro.h"
#include "../include/screen.h"

Tabuleiro* criar_tabuleiro(int linhas, int colunas) {
    Tabuleiro *tab = (Tabuleiro*)malloc(sizeof(Tabuleiro));
    if (!tab) return NULL;
    tab->linhas = linhas;
    tab->colunas = colunas;
    tab->matriz = (char**)malloc(linhas * sizeof(char*));
    for (int i = 0; i < linhas; i++) {
        tab->matriz[i] = (char*)malloc(colunas * sizeof(char));
        for (int j = 0; j < colunas; j++) tab->matriz[i][j] = '.'; 
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
    
    // --- Borda Superior ---
    screenGotoxy(MINX, MINY);
    printf("%s", BORDA_CANTOS);
    for (int i = 0; i < C; i++) printf("%s%s", BORDA_HORIZONTAL, BORDA_HORIZONTAL); 
    printf("%s", BORDA_CANTOS);

    // --- Mapa ---
    for (int y = 0; y < L; y++) {
        screenGotoxy(MINX, MINY + 1 + y);
        screenSetColor(WHITE, BLACK);
        printf("%s", BORDA_VERTICAL); // Borda Esquerda

        for (int x = 0; x < C; x++) {
            if (x == jogadorX && y == jogadorY) {
                screenSetColor(WHITE, BLUE);
                printf("%s", EMOJI_JOGADOR);
            } else if (tab->matriz[y][x] == 'S') {
                screenSetColor(WHITE, BLACK);
                printf("%s", EMOJI_TUBARAO);
            } else {
                screenSetColor(CYAN, BLACK); 
                printf(". "); 
            }
        }
        screenSetColor(WHITE, BLACK);
        printf("%s", BORDA_VERTICAL); // Borda Direita
    }

    // --- Borda Inferior ---
    screenGotoxy(MINX, MINY + L + 1); // Garante que fecha logo abaixo da ultima linha
    printf("%s", BORDA_CANTOS);
    for (int i = 0; i < C; i++) printf("%s%s", BORDA_HORIZONTAL, BORDA_HORIZONTAL);
    printf("%s", BORDA_CANTOS);
}