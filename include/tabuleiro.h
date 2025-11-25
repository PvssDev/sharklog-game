#ifndef __TABULEIRO_H__
#define __TABULEIRO_H__

#include <stdio.h>
#include <stdlib.h>

// Emoji do tubarão (UTF-8)
#define EMOJI_TUBARAO "🦈"

// Emoji do jogador surfista (UTF-8)
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

// Criação e destruição do tabuleiro
Tabuleiro* criar_tabuleiro(int linhas, int colunas);
void destruir_tabuleiro(Tabuleiro *tab);

// Desenho do tabuleiro com jogador
void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY);

// Movimentação do jogador dentro do tabuleiro
void aplicar_movimento(Tabuleiro *tab, int *jogadorX, int *jogadorY, int moveX, int moveY);

// Movimentação dos tubarões em direção ao jogador
void mover_tubaroes_perseguicao(Tabuleiro *tab, int jogadorX, int jogadorY);

// Checa colisão entre jogador e tubarão
int checar_colisao(Tabuleiro *tab, int jogadorX, int jogadorY);

#endif
