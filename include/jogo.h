#ifndef JOGO_H
#define JOGO_H

#include "tabuleiro.h"
#include "jogador.h"

// --- AQUI ESTÃO AS DEFINIÇÕES QUE FALTAVAM ---
#define PONTOS_NORMAL 10
#define PONTOS_DIFICIL 15

// Funções de Inicialização/Atualização
void jogo_inicializar_tubaroes(Tabuleiro *tab);
void jogo_resetar_tubaroes(Tabuleiro *tab);
void jogo_mover_tubaroes(Tabuleiro *tab, Jogador *j);
void desenhar_HUD(Jogador *j);

// Funções de Lógica e Eventos
int jogo_fase_perguntas(Jogador *j);
int jogo_pergunta_tubarao(Jogador *j);
int fazer_pergunta_gui(const char* p, const char* r1, const char* r2, int indice_correta);

#endif