/**
 * src/jogo.c
 * Implementa a lógica central do jogo.
 */

#include <string.h> 
#include <stdlib.h> 
#include "jogo.h"
#include "tabuleiro.h" 
#include "jogador.h"
#include "logica.h"
#include "screen.h"
#include "keyboard.h"
#include "timer.h"    

// Guarda o estado do jogo. 0 = rodando, 1 = deve terminar
static int g_estado_jogo = 0; 

// ESTRUTURA GLOBAL DO JOGO
static Tabuleiro *g_tabuleiro = NULL;
static const int TAB_LINHAS = 20;
static const int TAB_COLUNAS = 70;

// Posição do Hello World (jogador temporário)
static int g_x = 34;
static int g_y = 12;

// Direção 
static int g_incX = 1;
static int g_incY = 1;

static const char* g_texto = "Hello World!";

static void mover_e_desenhar_hello(int nextX, int nextY)
{
    // Como estamos redesenhando o tabuleiro inteiro em jogo_desenhar,
    // não precisamos apagar o rastro manualmente aqui, mas se o tabuleiro
    // for estático, precisamos limpar o rastro do Hello World.
    
    // Vamos apenas atualizar as coordenadas aqui
    g_x = nextX;
    g_y = nextY;
}

void jogo_inicializar(void) {
    // 1. Configura a tela e limpa tudo
    screenInit(1); 
    keyboardInit();
    timerInit(100);

    // 2. Inicializa o Tabuleiro
    g_tabuleiro = criar_tabuleiro(TAB_LINHAS, TAB_COLUNAS);
    if (g_tabuleiro == NULL) {
        screenDestroy();
        exit(1); 
    }
    
    // 3. DESENHO INICIAL OBRIGATÓRIO
    // Desenha o tabuleiro pela primeira vez para garantir que apareça
    desenhar_tabuleiro(g_tabuleiro, g_x - MINX - 1, g_y - MINY - 1);
    
    // Desenha o texto inicial por cima
    screenGotoxy(g_x, g_y);
    screenSetColor(CYAN, DARKGRAY);
    screenPuts(g_texto);
    
    screenUpdate(); // Garante que tudo apareça na tela
}

void jogo_processar_input(int tecla) {
    if (tecla == 10) { // Enter
        g_estado_jogo = 1; 
    }
}

void jogo_atualizar_estado(void) {
    int newX = g_x + g_incX;

    // Colisão básica com as bordas do tabuleiro
    // O tabuleiro começa em MINX+1 (borda esquerda) e termina em MINX+1+COLUNAS
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
    // Redesenha o tabuleiro a cada frame para garantir que ele não suma
    // Nota: Isso pode causar um pouco de "flicker" (piscar), mas garante visualização
    desenhar_tabuleiro(g_tabuleiro, g_x - MINX - 1, g_y - MINY - 1);
    
    // Desenha o texto por cima
    screenGotoxy(g_x, g_y);
    screenSetColor(CYAN, DARKGRAY);
    screenPuts(g_texto);
    
    screenUpdate(); // Atualiza a tela
}

int jogo_deve_terminar(void) {
    return g_estado_jogo;
}

void jogo_finalizar(void) {
    screenClear();
    destruir_tabuleiro(g_tabuleiro);
    screenGotoxy(10, 10);
    screenPuts("Obrigado por jogar!");
    screenUpdate();
    timerDelay(1000);
    
    // Restaura o terminal
    keyboardDestroy();
    screenDestroy();
}
