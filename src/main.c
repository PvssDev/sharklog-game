#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "tabuleiro.h"
#include "logica.h"

#define PONTOS_NORMAL 10
#define PONTOS_DIFICIL 15
#define VIDAS_INICIAIS 2

static int pontos = 0;
static int vidas = VIDAS_INICIAIS;

static void desenhar_hud(Tabuleiro *tab) {
    int hudY = MINY + tab->linhas + 2;
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(MINX, hudY);
    printf("Pontos: %d  Vidas: %d", pontos, vidas);
    screenSetColor(WHITE, BLACK);
    screenUpdate();
}

int main() {
    srand((unsigned)time(NULL));

    screenInit(1);
    keyboardInit();
    timerInit(50);

    jogadorX = 4;
    jogadorY = 4;
    next_moveX = next_moveY = 0;

    BancoPerguntas banco;
    if (!carregar_perguntas("data/perguntas.json", &banco)) {
        screenSetColor(RED, BLACK);
        screenGotoxy(1, MAXY + 2);
        printf("Aviso: nao foi possivel carregar data/perguntas.json");
        screenSetColor(WHITE, BLACK);
    }

    Tabuleiro *tab = criar_tabuleiro(MAXY, MAXX);
    if (!tab) {
        printf("Erro ao criar tabuleiro.\n");
        keyboardDestroy();
        screenDestroy();
        timerDestroy();
        return 1;
    }

    // popula tubarões
    for (int k = 0; k < 12; k++) {
        int rx = rand() % (tab->colunas - 4) + 2;
        int ry = rand() % (tab->linhas - 4) + 2;
        if (rx == jogadorX && ry == jogadorY) { k--; continue; }
        tab->matriz[ry][rx] = 'S';
    }

    desenhar_tabuleiro(tab, jogadorX, jogadorY);
    desenhar_hud(tab);

    while (1) {
        printf("\nPergunta 1 da rodada:\n");
        if (perguntar_rodada(&banco, PONTOS_NORMAL)) pontos += PONTOS_NORMAL;

        printf("\nPergunta 2 da rodada:\n");
        if (perguntar_rodada(&banco, PONTOS_NORMAL)) pontos += PONTOS_NORMAL;

        printf("\nMovimente-se (WASD), 'Q' para sair: ");
        if (keyhit()) {
            char cc = readch();
            if (cc == 'w' || cc == 'W') { next_moveY = -1; next_moveX = 0; }
            else if (cc == 's' || cc == 'S') { next_moveY = 1; next_moveX = 0; }
            else if (cc == 'a' || cc == 'A') { next_moveX = -1; next_moveY = 0; }
            else if (cc == 'd' || cc == 'D') { next_moveX = 1; next_moveY = 0; }
            else if (cc == 'q' || cc == 'Q') break;
        }

        aplicar_movimento(tab);
        mover_tubaroes_perseguicao(tab);

        if (checar_colisao(tab)) {
            printf("\nUm tubarão te capturou! Responda a pergunta difícil:\n");
            if (perguntar_tubarao(&banco, PONTOS_DIFICIL)) {
                pontos += PONTOS_DIFICIL;
                printf("Você escapou!\n");
                tab->matriz[jogadorY][jogadorX] = '.';
            } else {
                vidas--;
                printf("Você errou. Vidas restantes: %d\n", vidas);
                tab->matriz[jogadorY][jogadorX] = '.';
                if (vidas <= 0) {
                    printf("\n🦈 GAME OVER\n");
                    break;
                }
            }
        }

        desenhar_tabuleiro(tab, jogadorX, jogadorY);
        desenhar_hud(tab);
    }

    destruir_tabuleiro(tab);
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    printf("\nPontuação final: %d\n", pontos);
    return 0;
}
