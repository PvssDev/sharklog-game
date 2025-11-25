#ifndef __TABULEIRO_H__
#define __TABULEIRO_H__

#include <stdio.h>
#include <stdlib.h>

#define EMOJI_TUBARAO "🦈"
#define EMOJI_JOGADOR "🏄"

// Borda ASCII
#define BORDA_CANTOS       "+"
#define BORDA_HORIZONTAL   "-"
#define BORDA_VERTICAL     "|"

typedef struct {
    int linhas;
    int colunas;
    char **matriz;
} Tabuleiro;

// criação e destruição
Tabuleiro* criar_tabuleiro(int linhas, int colunas);
void destruir_tabuleiro(Tabuleiro *tab);

// desenho
void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY);

// movimentação
void aplicar_movimento(Tabuleiro *tab);
void mover_tubaroes_perseguicao(Tabuleiro *tab);
int checar_colisao(Tabuleiro *tab);

// variáveis globais do jogador (extern)
extern int jogadorX;
extern int jogadorY;
extern int next_moveX;
extern int next_moveY;

#endif
