/**
 * tabuleiro.h
 * Declara as funções e estruturas de dados relacionadas ao mapa do jogo.
 */

#ifndef __TABULEIRO_H__
#define __TABULEIRO_H__

// Define a estrutura do tabuleiro (mapa)
typedef struct {
    int linhas;
    int colunas;
    char **matriz;
} Tabuleiro;


// -----------------------------------------------------------------
// FUNÇÕES DE INTERFACE (Usadas por outros módulos, como jogo.c)
// -----------------------------------------------------------------

/**
 * @brief Aloca e inicializa uma nova estrutura de tabuleiro.
 * @param linhas O número de linhas do tabuleiro.
 * @param colunas O número de colunas do tabuleiro.
 * @return Um ponteiro para a nova estrutura Tabuleiro.
 */
Tabuleiro* criar_tabuleiro(int linhas, int colunas);

/**
 * @brief Libera a memória alocada para o tabuleiro.
 * @param tab O ponteiro para a estrutura Tabuleiro.
 */
void destruir_tabuleiro(Tabuleiro *tab);

/**
 * @brief Desenha o estado atual do tabuleiro (incluindo o jogador) na tela.
 * @param tab O ponteiro para a estrutura Tabuleiro.
 * @param jogadorX A posição X do jogador.
 * @param jogadorY A posição Y do jogador.
 */
void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY);


#endif /* __TABULEIRO_H__ */
