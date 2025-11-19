/**
 * src/tabuleiro.c
 * Versão ASCII (Segura)
 * Usa caracteres padrão (+, -, |) para garantir que o tabuleiro apareça
 * independentemente da configuração de fonte ou locale do terminal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabuleiro.h"
#include "screen.h"

// Caracteres ASCII Simples (Garante compatibilidade)
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
        // Preenche o fundo com pontos para visibilidade
        for (int j = 0; j < colunas; j++) {
            tab->matriz[i][j] = '.'; 
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

    // 1. Configurar cor da borda (Branco no Preto)
    screenSetColor(WHITE, BLACK);

    // ----------- BORDA SUPERIOR -----------
    screenGotoxy(MINX, MINY);
    printf("%s", BORDA_CANTOS);

    for (int i = 0; i < C; i++)
        printf("%s", BORDA_HORIZONTAL);

    printf("%s", BORDA_CANTOS);

    // ----------- CORPO DO TABULEIRO -----------
    for (int y = 0; y < L; y++) {
        // Calcula posição Y na tela (MINY + 1 para pular a borda superior + y)
        int screenY = MINY + 1 + y;
        
        // Borda Esquerda
        screenGotoxy(MINX, screenY);
        screenSetColor(WHITE, BLACK);
        printf("%s", BORDA_VERTICAL); 

        // Conteúdo da Linha
        for (int x = 0; x < C; x++) {
            if (x == jogadorX && y == jogadorY) {
                // Desenha o Jogador (P)
                screenSetColor(RED, BLACK);
                printf("P"); 
            } else {
                // Desenha o Fundo
                screenSetColor(LIGHTBLUE, BLACK);
                printf("%c", tab->matriz[y][x]);
            }
        }

        // Borda Direita
        screenSetColor(WHITE, BLACK);
        printf("%s", BORDA_VERTICAL); 
    }

    // ----------- BORDA INFERIOR -----------
    screenGotoxy(MINX, MINY + L + 1);
    printf("%s", BORDA_CANTOS);

    for (int i = 0; i < C; i++)
        printf("%s", BORDA_HORIZONTAL);

    printf("%s", BORDA_CANTOS);

    // Força o envio dos dados para a tela
    screenUpdate(); 
}
