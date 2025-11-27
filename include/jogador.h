#ifndef JOGADOR_H
#define JOGADOR_H

#include "tabuleiro.h"

typedef struct {
    int x; 
    int y; 
    // --- Adicionados para o SharkLog Game ---
    int vidas;
    int pontuacao;
} Jogador;

Jogador* criar_jogador(Tabuleiro* tab);
void destruir_jogador(Jogador* j);

// Retorna 1 se moveu, 0 se bloqueado
int mover_jogador(Jogador* j, Tabuleiro* tab, int tecla);

// Retorna 1 se colidiu com tubarão
int verificar_colisao(Jogador* j, Tabuleiro* tab);

#endif