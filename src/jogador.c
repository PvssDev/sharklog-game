#include <stdlib.h>
#include <ctype.h>
#include "../include/jogador.h"
#include "../include/tabuleiro.h"
#include "../include/logica.h" // Usa a função posicao_valida

Jogador* criar_jogador(Tabuleiro* tab) {
    if (!tab) return NULL;
    Jogador* j = (Jogador*) malloc(sizeof(Jogador));
    if (j != NULL) {
        j->x = 1; // Posição segura (canto superior esquerdo)
        j->y = 1;
        j->vidas = 2;       
        j->pontuacao = 0;
    }
    return j;
}

void destruir_jogador(Jogador* j) {
    if (j) free(j);
}

int mover_jogador(Jogador* j, Tabuleiro* tab, int tecla) {
    if (!j || !tab) return 0;
    int novoX = j->x;
    int novoY = j->y;
    char comando = toupper(tecla);

    switch (comando) {
        case 'W': novoY--; break;
        case 'S': novoY++; break;
        case 'A': novoX--; break;
        case 'D': novoX++; break;
        default: return 0;
    }

    // Usa logica.c para validar
    if (posicao_valida(novoX, novoY, tab->linhas, tab->colunas)) {
        j->x = novoX;
        j->y = novoY;
        return 1; 
    }
    return 0;
}

int verificar_colisao(Jogador* j, Tabuleiro* tab) {
    if (!j || !tab) return 0;
    return (tab->matriz[j->y][j->x] == 'S');
}