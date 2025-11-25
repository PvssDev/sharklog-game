#include <stdlib.h> // abs
#include "../include/logica.h"

int posicao_valida(int x, int y, int maxLinhas, int maxColunas) {
    if (x >= 0 && x < maxColunas && y >= 0 && y < maxLinhas) {
        return 1;
    }
    return 0;
}

int calcular_distancia(int x1, int y1, int x2, int y2) {
    // Distância de Manhattan (passos na grade)
    return abs(x1 - x2) + abs(y1 - y2);
}