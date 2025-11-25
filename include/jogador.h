#ifndef JOGADOR_H
#define JOGADOR_H

#include "tabuleiro.h"

// Estrutura do Jogador
typedef struct {
    int x; // Posição X na matriz (coluna)
    int y; // Posição Y na matriz (linha)
} Jogador;

/**
 * Cria o jogador em uma posição inicial segura.
 */
Jogador* criar_jogador(Tabuleiro* tab);

/**
 * Libera a memória do jogador.
 */
void destruir_jogador(Jogador* j);

/**
 * Processa a entrada do teclado e move o jogador.
 * Teclas suportadas: w, a, s, d (ou W, A, S, D).
 * Retorna 1 se moveu, 0 se não moveu.
 */
int mover_jogador(Jogador* j, Tabuleiro* tab, int tecla);

/**
 * Verifica se o jogador colidiu com um tubarão ('S').
 * Retorna 1 se colidiu (game over), 0 caso contrário.
 */
int verificar_colisao(Jogador* j, Tabuleiro* tab);

#endif
