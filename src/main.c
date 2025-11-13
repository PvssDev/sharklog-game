#include <string.h>

// Bibliotecas de interface (tela, teclado, temporizador)
#include "cli-lib/include/screen.h"
#include "cli-lib/include/keyboard.h"
#include "cli-lib/include/timer.h"

// Módulo principal da lógica do jogo
#include "jogo.h"

int main() {
    static int ch = 0;

    // Inicialização das Bibliotecas
    screenInit(1);
    keyboardInit();
    timerInit(50);

    // Inicialização do Jogo
    jogo_inicializar();

    // Loop principal do jogo
    while (jogo_deve_terminar() == 0) { // Continua enquanto o jogo não mandar parar

        // 1. Processa a entrada do usuário
        if (keyhit()) {
            ch = readch();
            jogo_processar_input(ch); // Envia a tecla para a lógica do jogo
        }

        // 2. Atualiza o estado lógico do jogo
        if (timerTimeOver() == 1) { // Executa na cadência do timer
            jogo_atualizar_estado(); // Manda o jogo calcular o próximo "frame"

            // 3. Desenha o estado atual
            jogo_desenhar();
            
            // 4. Atualiza a tela física
            screenUpdate();
        }
    }

    // Finalização
    
    // Limpa os recursos do jogo
    jogo_finalizar();

    // Desaloca os recursos das bibliotecas de interface
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}