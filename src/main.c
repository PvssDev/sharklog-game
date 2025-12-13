/**
 * src/main.c
 * Versão Final - SharkLog Game
 * Lógica Principal de Loop e Colisão
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

// Garante visibilidade das funções
void jogo_inicializar_tubaroes(Tabuleiro *tab, int pontuacao);
void jogo_resetar_tubaroes(Tabuleiro *tab, int pontuacao);
// Funções da Lula
int verificar_colisao_lula(Jogador *j, Tabuleiro *tab);
void gerenciar_lula_ciclo(Tabuleiro *tab, Jogador *j);
void animar_punicao(Tabuleiro *tab, Jogador *j);
void lula_colisao_reset(Tabuleiro *tab, Jogador *j); // Nova função usada

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
    
    // Limpeza inicial
    screenClear(); 

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
        
        if (!game_running) break;
        screenGotoxy(MINX, Y_MSG);
        printf("                                    ");

        // --- 1. COLISÃO COM LULA (🦑) ---
        if (verificar_colisao_lula(surfista, tab)) {
             screenSetColor(MAGENTA, BLACK);
             screenGotoxy(MINX, Y_MSG);
             printf("LULA TE PEGOU! SEGURADO PELOS TENTACULOS!");
             
             screenUpdate();
             fflush(stdout); 
             usleep(1000 * 3000);

             // Aplica a punição (Congelado + Tubarões perseguindo)
             animar_punicao(tab, surfista);
             
             // --- LULA DESAPARECE E CICLO REINICIA ---
             // A lula some e só aparecerá após +50 pontos.
             lula_colisao_reset(tab, surfista);
             
             // Limpeza pós punição
             while(keyhit()) readch();
             screenGotoxy(MINX, Y_MSG);
             printf("                                             ");
        }

        // --- 2. COLISÃO COM TUBARÃO (S) ---
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
            fflush(stdout); 
            usleep(2000 * 1000); 
            
            while(keyhit()) {
                readch();
            }
            
            screenGotoxy(MINX, Y_MSG);
            printf("                                 ");
            screenUpdate();
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
    
    while(keyhit()) {
        readch();
    }

    destruir_jogador(surfista);
    destruir_tabuleiro(tab);
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}