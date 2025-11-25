/**
 * src/main.c
 * Versão final corrigida para compatibilidade com bibliotecas estáticas
 */

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h> // ADICIONADO: Para usar usleep

#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "tabuleiro.h" 
#include "jogador.h"

// Função auxiliar local
void mover_tubaroes(Tabuleiro *tab) {
    if (!tab) return;

    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            if (tab->matriz[i][j] == 'S') {
                int dir = rand() % 4;
                int ni = i, nj = j;

                switch(dir) {
                    case 0: nj--; break; // Esquerda
                    case 1: nj++; break; // Direita
                    case 2: ni--; break; // Cima
                    case 3: ni++; break; // Baixo
                }

                if (ni > 0 && ni < tab->linhas &&
                    nj > 0 && nj < tab->colunas &&
                    tab->matriz[ni][nj] == '.') {
                    
                    tab->matriz[ni][nj] = 'S';
                    tab->matriz[i][j] = '.';
                }
            }
        }
    }
}

int main() 
{
    int ch = 0;
    int game_running = 1;

    screenInit(1);
    keyboardInit();
    timerInit(1000); 

    srand((unsigned) time(NULL));

    Tabuleiro *tab = criar_tabuleiro(MAXY, MAXX);
    if (tab == NULL) return 1;

    int qtd_tubaroes = 15;
    for (int k = 0; k < qtd_tubaroes; k++) {
        int rx = rand() % (tab->colunas - 2) + 1;
        int ry = rand() % (tab->linhas - 2) + 1;
        tab->matriz[ry][rx] = 'S';
    }

    Jogador *surfista = criar_jogador(tab);
    if (surfista == NULL) return 1;

    desenhar_tabuleiro(tab, surfista->x, surfista->y);

    while (game_running)
    {
        if (keyhit()) 
        {
            ch = readch();

            // Tratamento de setas (ESC sequence)
            if (ch == 27) {
                int ch2 = readch(); 
                if (ch2 == '[') {
                    int ch3 = readch();
                    switch(ch3) {
                        case 'A': ch = 'w'; break;
                        case 'B': ch = 's'; break;
                        case 'C': ch = 'd'; break;
                        case 'D': ch = 'a'; break;
                    }
                }
            }
            
            if (ch == 10) { 
                game_running = 0;
            } else {
                mover_jogador(surfista, tab, ch);
                
                if (verificar_colisao(surfista, tab)) {
                    game_running = 0;
                }
            }
            desenhar_tabuleiro(tab, surfista->x, surfista->y);
        }

        if (timerTimeOver() == 1)
        {
            mover_tubaroes(tab);

            if (verificar_colisao(surfista, tab)) {
                game_running = 0;
            }

            desenhar_tabuleiro(tab, surfista->x, surfista->y);
        }
    }

    screenClear();
    screenGotoxy(MAXX/2 - 5, MAXY/2);
    
    if (verificar_colisao(surfista, tab)) {
        screenSetColor(RED, BLACK);
        printf("GAME OVER! O tubarao te pegou! 🦈");
    } else {
        screenSetColor(GREEN, BLACK);
        printf("Jogo Encerrado.");
    }

    screenUpdate();
    
    // CORREÇÃO: timerDelay substituído por usleep
    usleep(2000 * 1000); 

    destruir_jogador(surfista);
    destruir_tabuleiro(tab);

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
