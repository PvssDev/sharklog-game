#include <string.h>

// Bibliotecas de interface
#include "keyboard.h"
#include "screen.h"
#include "timer.h"

// Módulo da lógica principal do jogo
#include "jogo.h"

int main() {
    int ch = 0;


    // Inicialização das bibliotecas externas

    screenInit(1);          // habilita tela dupla-buffer
    keyboardInit();         // entrada não bloqueante
    timerInit(50);          // taxa de atualização ~20fps


    // Inicialização do JOGO

    jogo_inicializar();     // cria tabuleiro, jogador, etc.


    // LOOP PRINCIPAL

    while (!jogo_deve_terminar()) {

        // ------ PROCESSA INPUT DO TECLADO ------
        if (keyhit()) {
            ch = readch();
            jogo_processar_input(ch);  // envia para a lógica do jogo
        }

        // ------ ATUALIZAÇÃO PERIÓDICA ------
        if (timerTimeOver()) {

            jogo_atualizar_estado();  // atualiza lógica
            jogo_desenhar();          // desenha TUDO, incluindo o tabuleiro 80x30
            screenUpdate();           // envia desenhado pro terminal
        }
    }


    // FINALIZAÇÃO


    jogo_finalizar();       // libera tabuleiro, jogador, etc.

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    // Garante que o cursor volte ao normal
    screenSetCursor(1);

    return 0;
}
