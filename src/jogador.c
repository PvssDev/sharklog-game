#include <stdlib.h>
#include <ctype.h> // Para toupper
#include "jogador.h"
#include "tabuleiro.h"

Jogador* criar_jogador(Tabuleiro* tab) {
    if (!tab) return NULL;

    Jogador* j = (Jogador*) malloc(sizeof(Jogador));
    if (j != NULL) {
        // Tenta posicionar no centro do tabuleiro
        j->x = tab->colunas / 2;
        j->y = tab->linhas / 2;

        // Se nascer em cima de um tubarão, move para o canto (0,0) por segurança
        if (tab->matriz[j->y][j->x] == 'S') {
            j->x = 0;
            j->y = 0;
        }
    }
    return j;
}

void destruir_jogador(Jogador* j) {
    if (j != NULL) {
        free(j);
    }
}

int mover_jogador(Jogador* j, Tabuleiro* tab, int tecla) {
    if (!j || !tab) return 0;

    int novoX = j->x;
    int novoY = j->y;

    // Normaliza para maiúsculo para aceitar 'w' e 'W'
    char comando = toupper(tecla);

    switch (comando) {
        case 'W': // Cima
            novoY--;
            break;
        case 'S': // Baixo
            novoY++;
            break;
        case 'A': // Esquerda
            novoX--;
            break;
        case 'D': // Direita
            novoX++;
            break;
        default:
            return 0; // Tecla inválida para movimento
    }

    // Verifica se a nova posição está dentro dos limites do tabuleiro
    // Lembra que tab->linhas e tab->colunas são o tamanho total
    if (novoX >= 0 && novoX < tab->colunas &&
        novoY >= 0 && novoY < tab->linhas) {
        
        j->x = novoX;
        j->y = novoY;
        return 1; // Moveu com sucesso
    }

    return 0; // Movimento bloqueado (parede)
}

int verificar_colisao(Jogador* j, Tabuleiro* tab) {
    if (!j || !tab) return 0;

    // Verifica na matriz do tabuleiro se a posição atual tem um Tubarão
    if (tab->matriz[j->y][j->x] == 'S') {
        return 1; // Colisão detectada!
    }

    return 0;
}
