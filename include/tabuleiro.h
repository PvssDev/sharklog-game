#ifndef TABULEIRO_H
#define TABULEIRO_H

// Dimensões do Jogo (Ajustado para caber na tela com margem)
#define LARGURA_JOGO 35 
#define ALTURA_JOGO  15

// Visuais
#define EMOJI_TUBARAO "🦈"
#define EMOJI_JOGADOR "🏄"

#define BORDA_CANTOS       "+"
#define BORDA_HORIZONTAL   "-"
#define BORDA_VERTICAL     "|"

typedef struct {
    int linhas;
    int colunas;
    char **matriz;
} Tabuleiro;

Tabuleiro* criar_tabuleiro(int linhas, int colunas);
void destruir_tabuleiro(Tabuleiro *tab);
void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY);

#endif