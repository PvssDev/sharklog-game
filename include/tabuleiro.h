#ifndef __TABULEIRO_H__
#define __TABULEIRO_H__

typedef struct {
    int linhas;
    int colunas;
    char **matriz;
} Tabuleiro;

Tabuleiro* criar_tabuleiro(int linhas, int colunas);
void destruir_tabuleiro(Tabuleiro *tab);
void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY);

#endif
