/**
 * src/jogo.c
 * Implementa a lógica central do jogo.
 * É o "maestro" que coordena o tabuleiro, jogador e lógica.
 */

#include <string.h> // Para strlen
#include "jogo.h"

// Módulos de elementos do jogo
#include "tabuleiro.h"
#include "jogador.h"
#include "logica.h"

// Bibliotecas de interface (tela, teclado, temporizador)
#include "cli-lib/include/screen.h"
#include "cli-lib/include/keyboard.h"
#include "cli-lib/include/timer.h"    

// Guarda o estado do jogo. 0 = rodando, 1 = deve terminar
static int g_estado_jogo = 0; 

// Posição do texto
static int g_x = 34;
static int g_y = 12;

// Direção do texto ("velocidade")
static int g_incX = 1;
static int g_incY = 1;

// Texto a ser exibido
static const char* g_texto = "Hello World!";

static void mover_e_desenhar_hello(int nextX, int nextY)
{
    // Define a cor (ex: CYAN no fundo DARKGRAY)
    screenSetColor(CYAN, DARKGRAY);

    // 1. Apaga o texto na posição ANTIGA (g_x, g_y)
    screenGotoxy(g_x, g_y);
    
    // Preenche a área antiga com espaços em branco
    for (size_t i = 0; i < strlen(g_texto); i++) {
        screenPuts(" "); // Envia um caractere de espaço
    }

    // 2. Atualiza as coordenadas globais
    g_x = nextX;
    g_y = nextY;

    // 3. Escreve o texto na NOVA posição (g_x, g_y)
    screenGotoxy(g_x, g_y);
    screenPuts(g_texto);
}


void jogo_inicializar(void) {
    // Desenha o texto na posição inicial no primeiro frame.
    screenGotoxy(g_x, g_y);
    screenSetColor(CYAN, DARKGRAY);
    screenPuts(g_texto);
}

void jogo_processar_input(int tecla) {
    // A tecla 'Enter' (código 10) finaliza o jogo
    if (tecla == 10) {
        g_estado_jogo = 1; // Manda o jogo parar
        return;
    }
    
    // Outras teclas podem ser processadas aqui (ex: movimento)
}

void jogo_atualizar_estado(void) {
    // Esta função contém a lógica executada a cada "tick" do timer.

    // Calcula a próxima posição X
    int newX = g_x + g_incX;

    // Verifica colisão nas bordas X (MINX e MAXX vêm de screen.h)
    if (newX >= (MAXX - strlen(g_texto) - 1) || newX <= MINX + 1) {
        g_incX = -g_incX; // Inverte a direção X
    }

    // Calcula a próxima posição Y
    int newY = g_y + g_incY;
    
    // Verifica colisão nas bordas Y (MINY e MAXY vêm de screen.h)
    if (newY >= MAXY - 1 || newY <= MINY + 1) {
        g_incY = -g_incY; // Inverte a direção Y
    }

    // Aplica o movimento e redesenha o texto na tela
    mover_e_desenhar_hello(newX, newY);
}

void jogo_desenhar(void) {

}

int jogo_deve_terminar(void) {
    return g_estado_jogo; // Retorna 1 se for para parar, 0 se não
}

void jogo_finalizar(void) {
    // Limpa a tela
    screenClear();
    
    // Mostra mensagem final
    screenGotoxy(10, 10);
    screenPuts("Obrigado por jogar!");
    screenUpdate();
    
    // Pausa rápida para o usuário ver a mensagem
    timerDelay(1000); // Espera 1 segundo (1000 ms)

    // Libera qualquer memória alocada (malloc) aqui.
}