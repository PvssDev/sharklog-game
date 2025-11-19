/**
 * src/screen.c
 * Implementa as funções de manipulação de tela (ANSI escape codes).
 */

#include "screen.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Implementação de screenPuts para imprimir uma string
void screenPuts(const char *s) {
    if (s) {
        printf("%s", s);
    }
}

// Implementação de screenInit (usa funções estáticas do cabeçalho)
void screenInit(int drawBorders) {
    screenClear();
    screenHideCursor();
    screenSetNormal();
    
    // Se precisar desenhar bordas, adicione a lógica aqui
    if (drawBorders) {
        // Lógica para desenhar uma moldura simples
    }
    screenUpdate();
}

// Implementação de screenDestroy
void screenDestroy() {
    screenClear();
    screenHomeCursor();
    screenShowCursor();
    screenSetNormal();
    screenBoxDisable();
    screenUpdate();
}

// Implementação de screenGotoxy
void screenGotoxy(int x, int y) {
    // Sequência ANSI para mover o cursor: \033[<ROW>;<COL>H ou f
    printf("%s[%d;%df", ESC, y, x);
}

// Implementação de screenSetColor
void screenSetColor(screenColor fg, screenColor bg) {
    // Configura as cores de foreground e background usando códigos ANSI.
    // Código de foreground: 30 + cor
    // Código de background: 40 + cor

    // Mapeamento simplificado:
    int fgCode = 30;
    int bgCode = 40;

    // Lógica simplificada de mapeamento, assumindo que as cores em screenColor
    // correspondem aos códigos ANSI básicos (0-7) e estendidos (8-15).
    // Para simplificar, usamos apenas as 8 primeiras cores.

    // Foreground
    if (fg >= BLACK && fg <= LIGHTGRAY) {
        fgCode = 30 + fg; // BLACK=30, RED=31, etc.
    } else if (fg >= DARKGRAY && fg <= WHITE) {
        // Para cores claras, pode-se usar 90+cor ou a flag BOLD
        fgCode = 90 + (fg - DARKGRAY);
    }

    // Background
    if (bg >= BLACK && bg <= LIGHTGRAY) {
        bgCode = 40 + bg;
    }

    printf("%s[%d;%dm", ESC, fgCode, bgCode);
}
