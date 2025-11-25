/**
 * src/jogo.c
 * CORRIGIDO: Substituindo screenPuts e timerDelay por funções padrão
 */

#include <string.h> 
#include <stdlib.h> 
#include <stdio.h>   // Necessário para printf
#include <unistd.h>  // Necessário para usleep
#include "jogo.h"
#include "tabuleiro.h" 
#include "jogador.h"
#include "logica.h"
#include "screen.h"
#include "keyboard.h"
#include "timer.h"    

static int g_estado_jogo = 0; 

// ESTRUTURA GLOBAL DO JOGO
static Tabuleiro *g_tabuleiro = NULL;
static const int TAB_LINHAS = 20;
static const int TAB_COLUNAS = 70;

static int g_x = 34;
static int g_y = 12;

static int g_incX = 1;
static int g_incY = 1;

static const char* g_texto = "Hello World!";

static void mover_e_desenhar_hello(int nextX, int nextY)
{
    g_x = nextX;
    g_y = nextY;
}

void jogo_inicializar(void) {
    screenInit(1); 
    keyboardInit();
    timerInit(100);

    g_tabuleiro = criar_tabuleiro(TAB_LINHAS, TAB_COLUNAS);
    if (g_tabuleiro == NULL) {
        screenDestroy();
        exit(1); 
    }
    
    desenhar_tabuleiro(g_tabuleiro, g_x - MINX - 1, g_y - MINY - 1);
    
    screenGotoxy(g_x, g_y);
    screenSetColor(CYAN, DARKGRAY);
    
    // CORREÇÃO: screenPuts não existe, usando printf
    printf("%s", g_texto);
    
    screenUpdate(); 
}

void jogo_processar_input(int tecla) {
    if (tecla == 10) { 
        g_estado_jogo = 1; 
    }
}

void jogo_atualizar_estado(void) {
    int newX = g_x + g_incX;

    if (newX >= (MINX + TAB_COLUNAS) || newX <= MINX + 1) {
        g_incX = -g_incX;
    }

    int newY = g_y + g_incY;
    if (newY >= (MINY + TAB_LINHAS) || newY <= MINY + 1) {
        g_incY = -g_incY;
    }

    mover_e_desenhar_hello(newX, newY);
}

void jogo_desenhar(void) {
    desenhar_tabuleiro(g_tabuleiro, g_x - MINX - 1, g_y - MINY - 1);
    
    screenGotoxy(g_x, g_y);
    screenSetColor(CYAN, DARKGRAY);
    
    // CORREÇÃO: screenPuts não existe, usando printf
    printf("%s", g_texto);
    
    screenUpdate(); 
}

int jogo_deve_terminar(void) {
    return g_estado_jogo;
}

void jogo_finalizar(void) {
    screenClear();
    destruir_tabuleiro(g_tabuleiro);
    screenGotoxy(10, 10);
    
    // CORREÇÃO: screenPuts não existe
    printf("Obrigado por jogar!");
    
    screenUpdate();
    
    // CORREÇÃO: timerDelay não existe, usando usleep (microsegundos)
    // 1000 ms = 1000000 us
    usleep(1000 * 1000);
    
    keyboardDestroy();
    screenDestroy();
}
