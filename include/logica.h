#ifndef LOGICA_H
#define LOGICA_H

#include "tabuleiro.h"

// Verifica se uma coordenada (x,y) está dentro dos limites do tabuleiro
int posicao_valida(int x, int y, int maxLinhas, int maxColunas);

// Calcula distância simples (Manhattan) entre dois pontos (útil para IA)
int calcular_distancia(int x1, int y1, int x2, int y2);

#endif