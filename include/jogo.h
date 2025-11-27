#ifndef JOGO_H
#define JOGO_H

#include "tabuleiro.h"
#include "jogador.h"

// Definições de Pontuação
#define PONTOS_NORMAL 10
#define PONTOS_DIFICIL 20

// Funções Principais
void jogo_inicializar_tubaroes(Tabuleiro *tab, int pontuacao);
void jogo_resetar_tubaroes(Tabuleiro *tab, int pontuacao);

int jogo_fase_perguntas(Tabuleiro *tab, Jogador *j);

int fazer_pergunta_gui(Tabuleiro *tab, Jogador *j, const char* p, const char* r1, const char* r2, int indice_correta);

int jogo_pergunta_tubarao(Tabuleiro *tab, Jogador *j);

void jogo_mover_tubaroes(Tabuleiro *tab, Jogador *j);

void desenhar_HUD(Jogador *j);

#endif