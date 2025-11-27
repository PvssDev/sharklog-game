/**
 * src/main.c
 * Versão Final - SharkLog Game (Sem Ondas)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <time.h>

#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/timer.h"
#include "../include/tabuleiro.h" 
#include "../include/jogador.h"
#include "../include/jogo.h"

// Garante visibilidade
void jogo_inicializar_tubaroes(Tabuleiro *tab, int pontuacao);
void jogo_resetar_tubaroes(Tabuleiro *tab, int pontuacao);

int main() {
    screenInit(1);
    keyboardInit();
    timerInit(1000); 
    srand((unsigned) time(NULL));

    Tabuleiro *tab = criar_tabuleiro(ALTURA_JOGO, LARGURA_JOGO);
    if (!tab) return 1;

    Jogador *surfista = criar_jogador(tab);
    if (!surfista) return 1;

    // Inicializa o jogo
    jogo_resetar_tubaroes(tab, surfista->pontuacao); 
    int game_running = 1;

    while (game_running && surfista->vidas > 0) {
        
        desenhar_HUD(surfista);
        desenhar_tabuleiro(tab, surfista->x, surfista->y);
        
        // Fase de Perguntas
        if (!jogo_fase_perguntas(tab, surfista)) {
            game_running = 0; 
            break;
        }

        jogo_inicializar_tubaroes(tab, surfista->pontuacao);

        screenClear(); 
        desenhar_HUD(surfista);
        desenhar_tabuleiro(tab, surfista->x, surfista->y);
        
        int Y_MSG = MINY + ALTURA_JOGO + 4;
        screenGotoxy(MINX, Y_MSG);
        printf(" SUA VEZ! [W, A, S, D] para mover... ");
        
        int moveu = 0;
        
        // Loop de Ação
        while (!moveu && game_running) {
            
            // 1. Input do Jogador
            if (keyhit()) {
                int ch = readch();
                if (ch == 'q' || ch == 'Q') {
                    game_running = 0;
                }
                else if (mover_jogador(surfista, tab, ch)) {
                    moveu = 1;
                }
                desenhar_tabuleiro(tab, surfista->x, surfista->y);
            }

            // 2. Movimento dos Inimigos (Tubarões)
            if (timerTimeOver()) {
                jogo_mover_tubaroes(tab, surfista);
                
                // Se um tubarão andou em cima de você
                if (verificar_colisao(surfista, tab)) {
                    moveu = 1; 
                }
                
                desenhar_tabuleiro(tab, surfista->x, surfista->y);
            }
        }
        
        if (!game_running) break;
        screenGotoxy(MINX, Y_MSG);
        printf("                                    ");

        // --- COLISÃO COM TUBARÃO (S) ---
        if (verificar_colisao(surfista, tab)) {
            
            // Remove o tubarão específico que mordeu
            if (tab->matriz[surfista->y][surfista->x] == 'S') {
                tab->matriz[surfista->y][surfista->x] = '.';
            }
            
            surfista->vidas--;
            
            screenSetColor(RED, BLACK);
            screenGotoxy(MINX, Y_MSG);
            printf("AI! VOCE FOI MORDIDO! -1 VIDA");
            
            screenUpdate();
            usleep(1000 * 1000); 
            
            screenGotoxy(MINX, Y_MSG);
            printf("                                 ");
        }
    }

    screenClear();
    screenGotoxy(MAXX/2 - 10, MAXY/2);
    if (surfista->vidas <= 0) {
        screenSetColor(RED, BLACK);
        printf("GAME OVER!");
    } else {
        screenSetColor(GREEN, BLACK);
        printf("JOGO ENCERRADO.");
    }
    screenGotoxy(MAXX/2 - 10, MAXY/2 + 2);
    printf("Pontuação Final: %d", surfista->pontuacao);

    screenUpdate();
    usleep(3000 * 1000); 

    destruir_jogador(surfista);
    destruir_tabuleiro(tab);
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}