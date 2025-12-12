/**
 * src/tabuleiro.c
 * Implementação das funções de gerenciamento do mapa.
 * Responsável por alocar memória, limpar e desenhar o jogo na tela.
 * Alteração: Adicionado suporte para desenhar a Lula ('L').
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tabuleiro.h"
#include "../include/screen.h"

// --- Configurações de Borda (ASCII) ---
#define BORDA_CANTOS       "+"
#define BORDA_HORIZONTAL   "-"
#define BORDA_VERTICAL     "|"

/**
 * Aloca memória dinamicamente para o tabuleiro.
 */
Tabuleiro* criar_tabuleiro(int linhas, int colunas) {
    // 1. Aloca a estrutura principal
    Tabuleiro *tab = (Tabuleiro*)malloc(sizeof(Tabuleiro));
    if (!tab) return NULL; 

    tab->linhas = linhas;
    tab->colunas = colunas;

    // 2. Aloca o array de ponteiros para as linhas
    tab->matriz = (char**)malloc(linhas * sizeof(char*));
    if (tab->matriz == NULL) {
        free(tab); 
        return NULL;
    }
    
    // 3. Aloca cada linha individualmente e preenche com água
    for (int i = 0; i < linhas; i++) {
        tab->matriz[i] = (char*)malloc(colunas * sizeof(char));
        for (int j = 0; j < colunas; j++) {
            tab->matriz[i][j] = '.'; 
        }
    }

    return tab;
}

/**
 * Libera a memória alocada.
 */
void destruir_tabuleiro(Tabuleiro *tab) {
    if (!tab) return;
    
    for (int i = 0; i < tab->linhas; i++) {
        free(tab->matriz[i]);
    }
    
    free(tab->matriz);
    free(tab);
}

/**
 * Desenha o estado atual do jogo no terminal.
 */
void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY) {
    if (!tab) return;

    int L = tab->linhas;
    int C = tab->colunas;

    screenSetColor(WHITE, BLACK);

    // --- 1. Desenha a Borda Superior ---
    screenGotoxy(MINX, MINY);
    printf("%s", BORDA_CANTOS);
    for (int i = 0; i < C; i++) printf("%s%s", BORDA_HORIZONTAL, BORDA_HORIZONTAL); 
    printf("%s", BORDA_CANTOS);

    // --- 2. Desenha o Corpo do Mapa ---
    for (int y = 0; y < L; y++) {
        int screenY = MINY + 1 + y; 

        // Desenha parede esquerda
        screenGotoxy(MINX, screenY);
        screenSetColor(WHITE, BLACK);
        printf("%s", BORDA_VERTICAL);

        // Loop das colunas (Conteúdo)
        for (int x = 0; x < C; x++) {

            // CAMADA 1: JOGADOR SURFISTA
            if (x == jogadorX && y == jogadorY) {
                screenSetColor(WHITE, BLUE); 
                printf("%s", EMOJI_JOGADOR);
                screenSetColor(WHITE, BLACK); 
                continue; 
            }

            // CAMADA 2: TUBARÃO (S)
            if (tab->matriz[y][x] == 'S') {
                screenSetColor(WHITE, BLACK);
                printf("%s", EMOJI_TUBARAO); 
                continue;
            }

            // CAMADA 3: LULA (L) - NOVO!
            // Aqui estava o problema: sem este if, o código desenhava água no lugar do L
            if (tab->matriz[y][x] == 'L') {
                screenSetColor(MAGENTA, BLACK); // Cor diferente para destacar (se o terminal suportar) ou apenas branco
                printf("🦑"); 
                continue;
            }

            // CAMADA 4: ÁGUA (.)
            screenSetColor(CYAN, BLACK); 
            printf(". "); 
        }

        // Desenha parede direita
        screenSetColor(WHITE, BLACK);
        printf("%s", BORDA_VERTICAL);
    }

    // --- 3. Desenha a Borda Inferior ---
    screenGotoxy(MINX, MINY + L + 1);
    printf("%s", BORDA_CANTOS);
    for (int i = 0; i < C; i++) printf("%s%s", BORDA_HORIZONTAL, BORDA_HORIZONTAL);
    printf("%s", BORDA_CANTOS);

    screenUpdate();
}