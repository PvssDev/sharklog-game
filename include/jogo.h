#ifndef JOGO_H
#define JOGO_H

#include "tabuleiro.h"
#include "jogador.h"

#define PONTOS_NORMAL 10
#define PONTOS_DIFICIL 15

// Inicializa ou reseta os tubarões
void jogo_inicializar_tubaroes(Tabuleiro *tab);

// Fase de perguntas (retorna 1 = continua, 0 = sair)
int jogo_fase_perguntas(Jogador *j);

// Pergunta difícil (retorna 1 = acertou/sobreviveu, 0 = errou/dano)
int jogo_pergunta_tubarao(Jogador *j);

// Move os tubarões (IA)
void jogo_mover_tubaroes(Tabuleiro *tab, Jogador *j);

// Desenha o HUD
void desenhar_HUD(Jogador *j);

#endif