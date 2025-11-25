/**
 * src/tabuleiro.c
 * CORRIGIDO: Alinhamento visual para evitar que emojis "empurrem" o cenário.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabuleiro.h"
#include "screen.h"

// Borda 100% ASCII
#define BORDA_CANTOS       "+"
#define BORDA_HORIZONTAL   "-"
#define BORDA_VERTICAL     "|"

Tabuleiro* criar_tabuleiro(int linhas, int colunas) {
    Tabuleiro *tab = (Tabuleiro*)malloc(sizeof(Tabuleiro));
    if (tab == NULL) return NULL;

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
    if (tab == NULL) return;
    for (int i = 0; i < tab->linhas; i++) {
        free(tab->matriz[i]);
    }
    free(tab->matriz);
    free(tab);
}

void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY) {
    if (tab == NULL) return;

    int L = tab->linhas;
    int C = tab->colunas;

    screenSetColor(WHITE, BLACK);

    // --- Borda superior ---
    screenGotoxy(MINX, MINY);
    printf("%s", BORDA_CANTOS);
    // Como o conteúdo agora é duplo (. + espaço), a borda precisa ser dupla também
    for (int i = 0; i < C; i++) printf("%s%s", BORDA_HORIZONTAL, BORDA_HORIZONTAL); 
    printf("%s", BORDA_CANTOS);

    // --- Corpo do Tabuleiro ---
    for (int y = 0; y < L; y++) {
        int screenY = MINY + 1 + y;

        screenGotoxy(MINX, screenY);
        screenSetColor(WHITE, BLACK);
        printf("%s", BORDA_VERTICAL);

        for (int x = 0; x < C; x++) {

            // CASO 1: JOGADOR SURFISTA
            if (x == jogadorX && y == jogadorY) {
                screenSetColor(WHITE, BLUE); // Fundo azul para destacar na água
                printf("%s", EMOJI_JOGADOR); // O Emoji já ocupa 2 espaços visuais
                screenSetColor(WHITE, BLACK); // Reseta cor
                continue;
            }

            // CASO 2: TUBARÃO
            if (tab->matriz[y][x] == 'S') {
                screenSetColor(WHITE, BLACK);
                printf("%s", EMOJI_TUBARAO); // O Emoji já ocupa 2 espaços visuais
                continue;
            }

            // CASO 3: ÁGUA
            // AQUI ESTÁ A CORREÇÃO: Adicionamos um espaço após o ponto
            screenSetColor(CYAN, BLACK); 
            printf(". "); // Agora a água ocupa 2 espaços, igual aos emojis!
        }

        screenSetColor(WHITE, BLACK);
        printf("%s", BORDA_VERTICAL);
    }

    // --- Borda inferior ---
    screenGotoxy(MINX, MINY + L + 1);
    printf("%s", BORDA_CANTOS);
    for (int i = 0; i < C; i++) printf("%s%s", BORDA_HORIZONTAL, BORDA_HORIZONTAL);
    printf("%s", BORDA_CANTOS);

    screenUpdate();
}
