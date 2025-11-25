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
    // 1. Inicialização das Bibliotecas
    screenInit(1);
    keyboardInit();
    timerInit(100); 
    srand((unsigned) time(NULL));

    // 2. Criação dos Objetos
    Tabuleiro *tab = criar_tabuleiro(ALTURA_JOGO, LARGURA_JOGO);
    if (!tab) return 1;

    Jogador *surfista = criar_jogador(tab);
    if (!surfista) return 1;

    // 3. Setup Inicial
    jogo_inicializar_tubaroes(tab); 

    int game_running = 1;

    // --- LOOP PRINCIPAL DO JOGO ---
    while (game_running && surfista->vidas > 0) {
        
        // ==========================================
        // FASE 1: PERGUNTAS DE LÓGICA (2 por rodada)
        // ==========================================
        desenhar_HUD(surfista);
        desenhar_tabuleiro(tab, surfista->x, surfista->y);
        
        if (!jogo_fase_perguntas(surfista)) {
            game_running = 0; // Jogador pediu para sair (Q)
            break;
        }

        // ==========================================
        // FASE 2: MOVIMENTO DO JOGADOR
        // ==========================================
        // Redesenha para limpar a área onde estavam as perguntas
        screenClear(); 
        desenhar_HUD(surfista);
        desenhar_tabuleiro(tab, surfista->x, surfista->y);
        
        // Mensagem de instrução (Abaixo do HUD)
        int Y_MSG = MINY + ALTURA_JOGO + 4;
        screenGotoxy(MINX, Y_MSG);
        printf(" SUA VEZ! [W, A, S, D] para mover... ");
        
        int moveu = 0;
        // Loop de espera ativa pelo movimento
        while (!moveu && game_running) {
            if (keyhit()) {
                int ch = readch();
                // Verifica saída
                if (ch == 'q' || ch == 'Q') {
                    game_running = 0;
                }
                // Tenta mover
                else if (mover_jogador(surfista, tab, ch)) {
                    moveu = 1;
                }
            }
        }
        if (!game_running) break;

        // Limpa a mensagem "SUA VEZ"
        screenGotoxy(MINX, Y_MSG);
        printf("                                    ");

        // ==========================================
        // FASE 3: MOVIMENTO DOS TUBARÕES (IA)
        // ==========================================
        jogo_mover_tubaroes(tab, surfista);
        
        // ==========================================
        // FASE 4: VERIFICAÇÃO DE COLISÃO
        // ==========================================
        if (verificar_colisao(surfista, tab)) {
            // Move o surfista para o "Spawn" visualmente (1,1) para não desenhar em cima do tubarão
            surfista->x = 1; 
            surfista->y = 1; 
            
            // Atualiza a tela para mostrar o impacto antes da pergunta
            desenhar_tabuleiro(tab, surfista->x, surfista->y);

            // Chama o desafio do tubarão
            if (jogo_pergunta_tubarao(surfista)) {
                // Acertou: ganha pontos, tubarões resetam, vida salva.
            } else {
                // Errou: perde vida (já descontado na função), tubarões resetam.
            }

            // Pequena pausa para ler o resultado "ACERTOU/ERROU"
            while(!keyhit()); // Espera pressionar algo
            readch();         // Limpa o caractere
            
            // Reseta posições dos inimigos para evitar spawn kill
            jogo_inicializar_tubaroes(tab); 
        }
    }

    // --- TELA FINAL ---
    screenClear();
    screenGotoxy(MAXX/2 - 10, MAXY/2);
    
    if (surfista->vidas <= 0) {
        screenSetColor(RED, BLACK);
        printf("GAME OVER! A lógica falhou.");
    } else {
        screenSetColor(GREEN, BLACK);
        printf("JOGO ENCERRADO. Até logo!");
    }
    
    screenGotoxy(MAXX/2 - 10, MAXY/2 + 2);
    printf("Pontuação Final: %d", surfista->pontuacao);

    screenUpdate();
    usleep(3000 * 1000); // Espera 3 segundos antes de fechar

    // --- LIMPEZA DE MEMÓRIA ---
    destruir_jogador(surfista);
    destruir_tabuleiro(tab);
    
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}