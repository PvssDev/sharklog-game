/**
 * src/main.c
 * Versão Final - SharkLog Game
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

int main() {
    screenInit(1);
    keyboardInit();
    timerInit(1000); // 1 Segundo para movimento dos tubarões
    srand((unsigned) time(NULL));

    Tabuleiro *tab = criar_tabuleiro(ALTURA_JOGO, LARGURA_JOGO);
    if (!tab) return 1;

    Jogador *surfista = criar_jogador(tab);
    if (!surfista) return 1;

    // Inicializa com a pontuação 0 (6 tubarões)
    jogo_inicializar_tubaroes(tab, surfista->pontuacao); 
    int game_running = 1;

    while (game_running && surfista->vidas > 0) {
        
        desenhar_HUD(surfista);
        desenhar_tabuleiro(tab, surfista->x, surfista->y);
        
        if (!jogo_fase_perguntas(tab, surfista)) {
            game_running = 0; 
            break;
        }

        // --- CORREÇÃO PRINCIPAL ---
        // Agora atualizamos os tubarões IMEDIATAMENTE após ganhar pontos.
        // Se você tem 50 pts, isso vai gerar 11 tubarões agora mesmo.
        jogo_inicializar_tubaroes(tab, surfista->pontuacao);

        screenClear(); 
        desenhar_HUD(surfista);
        desenhar_tabuleiro(tab, surfista->x, surfista->y);
        
        int Y_MSG = MINY + ALTURA_JOGO + 4;
        screenGotoxy(MINX, Y_MSG);
        printf(" SUA VEZ! [W, A, S, D] para mover... ");
        
        int moveu = 0;
        
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

            // 2. Movimento dos Tubarões (Tempo Real)
            if (timerTimeOver()) {
                jogo_mover_tubaroes(tab, surfista);
                
                if (verificar_colisao(surfista, tab)) {
                    moveu = 1; 
                }
                desenhar_tabuleiro(tab, surfista->x, surfista->y);
            }
        }
        
        if (!game_running) break;
        screenGotoxy(MINX, Y_MSG);
        printf("                                    ");

        // FASE DE COLISÃO
        if (verificar_colisao(surfista, tab)) {
            surfista->x = 1; 
            surfista->y = 1; 
            desenhar_tabuleiro(tab, surfista->x, surfista->y);

            if (jogo_pergunta_tubarao(tab, surfista)) {
                // Acertou
            } else {
                // Errou
            }
            // Recalcula também ao morrer/colidir
            jogo_inicializar_tubaroes(tab, surfista->pontuacao); 
        }
    }

    screenClear();
    screenGotoxy(MAXX/2 - 10, MAXY/2);
    if (surfista->vidas <= 0) {
        screenSetColor(RED, BLACK);
        printf("GAME OVER! A lógica falhou.");
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
