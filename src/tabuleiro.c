#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tabuleiro.h"
#include "screen.h"

// variáveis globais
int jogadorX = 4;
int jogadorY = 4;
int next_moveX = 0;
int next_moveY = 0;

// Cria o tabuleiro e preenche com '.'
Tabuleiro* criar_tabuleiro(int linhas, int colunas) {
    Tabuleiro *tab = (Tabuleiro*)malloc(sizeof(Tabuleiro));
    if (!tab) return NULL;

    tab->linhas = linhas;
    tab->colunas = colunas;

    tab->matriz = (char**)malloc(linhas * sizeof(char*));
    if (!tab->matriz) { free(tab); return NULL; }

    for (int i = 0; i < linhas; i++) {
        tab->matriz[i] = (char*)malloc(colunas * sizeof(char));
        for (int j = 0; j < colunas; j++) tab->matriz[i][j] = '.';
    }

    return tab;
}

// Destrói tabuleiro
void destruir_tabuleiro(Tabuleiro *tab) {
    if (!tab) return;
    for (int i = 0; i < tab->linhas; i++) free(tab->matriz[i]);
    free(tab->matriz);
    free(tab);
}

// Desenha tabuleiro e jogador
void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY) {
    if (!tab) return;

    int L = tab->linhas;
    int C = tab->colunas;

    screenSetColor(WHITE, BLACK);

    // Borda superior
    screenGotoxy(MINX, MINY);
    printf("%s", BORDA_CANTOS);
    for (int i = 0; i < C; i++) printf("%s", BORDA_HORIZONTAL);
    printf("%s", BORDA_CANTOS);

    // Corpo
    for (int y = 0; y < L; y++) {
        screenGotoxy(MINX, MINY + 1 + y);
        printf("%s", BORDA_VERTICAL);
        for (int x = 0; x < C; x++) {
            if (x == jogadorX && y == jogadorY) {
                screenSetColor(RED, BLACK);
                printf(EMOJI_JOGADOR);
            } else if (tab->matriz[y][x] == 'S') {
                screenSetColor(WHITE, BLACK);
                printf(EMOJI_TUBARAO);
            } else {
                screenSetColor(LIGHTBLUE, BLACK);
                printf("%c", tab->matriz[y][x]);
            }
        }
        screenSetColor(WHITE, BLACK);
        printf("%s", BORDA_VERTICAL);
    }

    // Borda inferior
    screenGotoxy(MINX, MINY + L + 1);
    printf("%s", BORDA_CANTOS);
    for (int i = 0; i < C; i++) printf("%s", BORDA_HORIZONTAL);
    printf("%s", BORDA_CANTOS);

    screenUpdate();
}

// Aplica movimento do jogador
void aplicar_movimento(Tabuleiro *tab) {
    if (!tab) return;
    int nx = jogadorX + next_moveX;
    int ny = jogadorY + next_moveY;
    if (nx > 0 && nx < tab->colunas - 1) jogadorX = nx;
    if (ny > 0 && ny < tab->linhas - 1) jogadorY = ny;
    next_moveX = next_moveY = 0;
}

// Move tubarões em direção ao jogador
void mover_tubaroes_perseguicao(Tabuleiro *tab) {
    if (!tab) return;

    char **novo = malloc(tab->linhas * sizeof(char*));
    for (int i = 0; i < tab->linhas; i++) {
        novo[i] = malloc(tab->colunas * sizeof(char));
        memcpy(novo[i], tab->matriz[i], tab->colunas);
    }

    for (int y = 0; y < tab->linhas; y++) {
        for (int x = 0; x < tab->colunas; x++) {
            if (tab->matriz[y][x] == 'S') {
                int dx = jogadorX - x;
                int dy = jogadorY - y;
                int best_dx = 0, best_dy = 0;
                if (abs(dx) > abs(dy)) best_dx = dx>0?1:-1;
                else if (dy != 0) best_dy = dy>0?1:-1;

                int nx = x + best_dx;
                int ny = y + best_dy;

                if (nx > 0 && nx < tab->colunas - 1 && ny > 0 && ny < tab->linhas - 1) {
                    if (novo[ny][nx] == '.' && !(nx==jogadorX && ny==jogadorY)) {
                        novo[ny][nx] = 'S';
                        novo[y][x] = '.';
                    } else if (nx==jogadorX && ny==jogadorY) {
                        novo[ny][nx] = 'S';
                        novo[y][x] = '.';
                    } else novo[y][x] = 'S';
                } else novo[y][x] = 'S';
            }
        }
    }

    for (int i = 0; i < tab->linhas; i++) {
        memcpy(tab->matriz[i], novo[i], tab->colunas);
        free(novo[i]);
    }
    free(novo);
}

// Checa colisão
int checar_colisao(Tabuleiro *tab) {
    if (!tab) return 0;
    return tab->matriz[jogadorY][jogadorX] == 'S';
}
