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
    timerInit(100); 
    srand((unsigned) time(NULL));

    Tabuleiro *tab = criar_tabuleiro(ALTURA_JOGO, LARGURA_JOGO);
    if (!tab) return 1;

    Jogador *surfista = criar_jogador(tab);
    if (!surfista) return 1;

    jogo_inicializar_tubaroes(tab); 

    int game_running = 1;

    while (game_running && surfista->vidas > 0) {
        
        // 1. Fase de Perguntas
        desenhar_HUD(surfista);
        desenhar_tabuleiro(tab, surfista->x, surfista->y);
        if (!jogo_fase_perguntas(surfista)) {
            game_running = 0; 
            break;
        }

        // 2. Fase de Movimento Jogador
        desenhar_HUD(surfista);
        desenhar_tabuleiro(tab, surfista->x, surfista->y);
        screenGotoxy(MINX + 2, MAXY - 2);
        printf(" SUA VEZ! [W, A, S, D] para mover... ");
        
        int moveu = 0;
        while (!moveu && game_running) {
            if (keyhit()) {
                int ch = readch();
                if (ch == 'q' || ch == 'Q') game_running = 0;
                if (mover_jogador(surfista, tab, ch)) moveu = 1;
            }
        }
        if (!game_running) break;

        // 3. Fase Movimento Tubarões
        jogo_mover_tubaroes(tab, surfista);
        
        // 4. Verificação de Colisão
        if (verificar_colisao(surfista, tab)) {
            int oldX = surfista->x, oldY = surfista->y;
            surfista->x = 1; surfista->y = 1; // Tira do perigo visualmente

            if (jogo_pergunta_tubarao(surfista)) {
                // Acertou: Tubarões resetam, vida salva
            } else {
                // Errou: Já perdeu vida na função, só reseta
            }
            while(!keyhit()); readch(); // Pausa
            jogo_inicializar_tubaroes(tab); 
        }
        
        // Limpa texto auxiliar
        screenGotoxy(MINX + 2, MAXY - 2);
        printf("                                       ");
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
    printf("Pontuacao Final: %d", surfista->pontuacao);

    screenUpdate();
    usleep(3000 * 1000); 

    destruir_jogador(surfista);
    destruir_tabuleiro(tab);
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}