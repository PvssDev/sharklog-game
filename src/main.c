/**
  * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
 *
 * Observação: mantive todas as funções originais e apenas integrei o tabuleiro.
 */

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#include "tabuleiro.h" // ADICIONADO: integração com o tabuleiro

int x = 34, y = 12;
int incX = 1, incY = 1;

void printHello(int nextX, int nextY)
{
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(x, y);
    printf("           ");
    x = nextX;
    y = nextY;
    screenGotoxy(x, y);
    printf("Hello World");
}

void printKey(int ch)
{
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(35, 22);
    printf("Key code :");

    screenGotoxy(34, 23);
    printf("            ");
    
    if (ch == 27) screenGotoxy(36, 23);
    else screenGotoxy(39, 23);

    printf("%d ", ch);
    while (keyhit())
    {
        printf("%d ", readch());
    }
}

int main() 
{
    static int ch = 0;
    static long timer = 0;

    // Inicializações originais
    screenInit(1);
    keyboardInit();
    timerInit(50);

    // -----------------------------
    // ADICIONADO: criar e popular tabuleiro
    // -----------------------------
    srand((unsigned) time(NULL));

    // cria tabuleiro usando as constantes de screen.h (MAXY = linhas, MAXX = colunas)
    Tabuleiro *tab = criar_tabuleiro(MAXY, MAXX);
    if (tab == NULL) {
        // se falhar, continua sem tabuleiro (comportamento antigo)
        screenSetColor(RED, BLACK);
        screenGotoxy(1, MAXY + 2);
        printf("Erro: falha ao criar tabuleiro. Continuando sem tabuleiro...");
        screenSetColor(WHITE, BLACK);
    } else {
        // popula com alguns tubarões aleatórios (caractere 'S')
        int qtd = 12; // número de tubarões inicial (ajustável)
        for (int k = 0; k < qtd; k++) {
            int rx = rand() % (tab->colunas - 4) + 2; // evita bordas
            int ry = rand() % (tab->linhas - 4) + 2;
            // evita sobrescrever a posição inicial do Hello (x,y)
            if (rx == x && ry == y) { k--; continue; }
            tab->matriz[ry][rx] = 'S';
        }

        // desenha o tabuleiro pela primeira vez (antes do Hello)
        desenhar_tabuleiro(tab, x, y);
    }
    // -----------------------------
    // Fim da parte ADICIONADA
    // -----------------------------

    printHello(x, y);
    screenUpdate();

    while (ch != 10 && timer <= 100) //enter or 5s
    {
        // Handle user input
        if (keyhit()) 
        {
            ch = readch();
            printKey(ch);
            screenUpdate();
        }

        // Update game state (move elements, verify collision, etc)
        if (timerTimeOver() == 1)
        {
            int newX = x + incX;
            if (newX >= (MAXX -strlen("Hello World") -1) || newX <= MINX+1) incX = -incX;
            int newY = y + incY;
            if (newY >= MAXY-1 || newY <= MINY+1) incY = -incY;

            // -----------------------------
            // ADICIONADO: redesenha o tabuleiro (com tubarões) a cada frame
            // mantendo o comportamento original do Hello World.
            // desenhar_tabuleiro já posiciona o jogador internamente ao receber x,y.
            // -----------------------------
            if (tab != NULL) {
                desenhar_tabuleiro(tab, newX, newY);
            }

            printHello(newX, newY);

            screenUpdate();
            timer++;
        }
    }

    // -----------------------------
    // ADICIONADO: libera tabuleiro
    // -----------------------------
    if (tab != NULL) {
        destruir_tabuleiro(tab);
        tab = NULL;
    }
    // -----------------------------

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
