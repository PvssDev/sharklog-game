/**
 * main.c — finalizado com perguntas, pontuação e tubarões perseguição
 *
 * Mantive a estrutura original e apenas incrementei a movimentação para teclado sem scanf.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "tabuleiro.h"
#include "logica.h"

#define PONTOS_NORMAL 10
#define PONTOS_DIFICIL 15
#define VIDAS_INICIAIS 2

// Jogador inicia numa posição segura dentro das bordas
static int jogadorX = 4;
static int jogadorY = 4;

// HUD
static int pontos = 0;
static int vidas = VIDAS_INICIAIS;

// movimento temporário
static int next_moveX = 0;
static int next_moveY = 0;

// função de movimentação (1 passo, respeita bordas)
void aplicar_movimento(Tabuleiro *tab) {
    if (!tab) return;
    int nx = jogadorX + next_moveX;
    int ny = jogadorY + next_moveY;
    if (nx > 0 && nx < tab->colunas-1) jogadorX = nx;
    if (ny > 0 && ny < tab->linhas-1) jogadorY = ny;
    next_moveX = next_moveY = 0;
}

// tubarões perseguem: um passo em direção ao jogador (checa colisões simples)
void mover_tubaroes_perseguicao(Tabuleiro *tab) {
    if (!tab) return;

    // cria uma cópia temporária para evitar conflitos ao mover no mesmo loop
    char **novo = malloc(tab->linhas * sizeof(char*));
    for (int i = 0; i < tab->linhas; i++) {
        novo[i] = malloc(tab->colunas * sizeof(char));
        memcpy(novo[i], tab->matriz[i], tab->colunas * sizeof(char));
    }

    for (int y = 0; y < tab->linhas; y++) {
        for (int x = 0; x < tab->colunas; x++) {
            if (tab->matriz[y][x] == 'S') {
                int best_dx = 0, best_dy = 0;
                int dx = jogadorX - x;
                int dy = jogadorY - y;
                if (abs(dx) > abs(dy)) best_dx = (dx>0?1:-1);
                else if (dy != 0) best_dy = (dy>0?1:-1);
                else { best_dx = 0; best_dy = 0; }
                int nx = x + best_dx;
                int ny = y + best_dy;
                if (nx > 0 && nx < tab->colunas-1 && ny > 0 && ny < tab->linhas-1) {
                    if (novo[ny][nx] == '.' && !(nx==jogadorX && ny==jogadorY)) {
                        novo[ny][nx] = 'S';
                        novo[y][x] = '.';
                    } else if (nx==jogadorX && ny==jogadorY) {
                        novo[ny][nx] = 'S';
                        novo[y][x] = '.';
                    } else {
                        novo[y][x] = 'S';
                    }
                } else {
                    novo[y][x] = 'S';
                }
            }
        }
    }

    // copia de volta
    for (int i = 0; i < tab->linhas; i++) {
        memcpy(tab->matriz[i], novo[i], tab->colunas * sizeof(char));
        free(novo[i]);
    }
    free(novo);
}

static void desenhar_hud(Tabuleiro *tab) {
    int hudY = MINY + tab->linhas + 2;
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(MINX, hudY);
    printf("Pontos: %d  Vidas: %d", pontos, vidas);
    screenSetColor(WHITE, BLACK);
    screenUpdate();
}

int main() {
    srand((unsigned) time(NULL));
    screenInit(1);
    keyboardInit();
    timerInit(50);

    // carrega perguntas
    BancoPerguntas banco;
    if (!carregar_perguntas("data/perguntas.json", &banco)) {
        screenSetColor(RED, BLACK);
        screenGotoxy(1, MAXY + 2);
        printf("Aviso: nao foi possivel carregar data/perguntas.json");
        screenSetColor(WHITE, BLACK);
    }

    // criar tabuleiro
    Tabuleiro *tab = criar_tabuleiro(MAXY, MAXX);
    if (!tab) {
        screenSetColor(RED, BLACK);
        screenGotoxy(1, MAXY + 3);
        printf("Erro: falha ao criar tabuleiro.");
        screenSetColor(WHITE, BLACK);
        keyboardDestroy();
        screenDestroy();
        timerDestroy();
        return 1;
    }

    // popula tubarões iniciais
    int qtd = 12;
    for (int k = 0; k < qtd; k++) {
        int rx = rand() % (tab->colunas - 4) + 2;
        int ry = rand() % (tab->linhas - 4) + 2;
        if (rx == jogadorX && ry == jogadorY) { k--; continue; }
        tab->matriz[ry][rx] = 'S';
    }

    desenhar_tabuleiro(tab, jogadorX, jogadorY);
    desenhar_hud(tab);

    long frames = 0;

    while (1) {
        // duas perguntas normais
        printf("\nPergunta 1 da rodada:\n");
        int ok1 = perguntar_rodada(&banco, PONTOS_NORMAL);
        if (ok1) pontos += PONTOS_NORMAL;
        printf("\nPergunta 2 da rodada:\n");
        int ok2 = perguntar_rodada(&banco, PONTOS_NORMAL);
        if (ok2) pontos += PONTOS_NORMAL;

        // movimentação jogador com keyboard.h
        printf("\nAgora mova o jogador (WASD). Pressione tecla: ");
        if (keyhit()) {
            char cc = readch();
            if (cc == 'w' || cc == 'W') { next_moveY=-1; next_moveX=0; }
            else if (cc == 's' || cc == 'S') { next_moveY=1; next_moveX=0; }
            else if (cc == 'a' || cc == 'A') { next_moveX=-1; next_moveY=0; }
            else if (cc == 'd' || cc == 'D') { next_moveX=1; next_moveY=0; }
            else if (cc == 'q' || cc == 'Q') { break; } // quit
            else { next_moveX = next_moveY = 0; }
        } else { next_moveX = next_moveY = 0; }

        aplicar_movimento(tab);
        mover_tubaroes_perseguicao(tab);

        // colisão com tubarão
        if (tab->matriz[jogadorY][jogadorX] == 'S') {
            printf("\nUm tubarão te capturou! Responda a pergunta difícil para tentar escapar.\n");
            int ok = perguntar_tubarao(&banco, PONTOS_DIFICIL);
            if (ok) {
                pontos += PONTOS_DIFICIL;
                printf("Você escapou!\n");
                tab->matriz[jogadorY][jogadorX] = '.';
            } else {
                vidas--;
                printf("Você errou. Vidas restantes: %d\n", vidas);
                if (vidas <= 0) {
                    printf("\n🦈 GAME OVER — Você foi pego pelo tubarão e não teve mais vidas.\n");
                    break;
                } else {
                    tab->matriz[jogadorY][jogadorX] = '.';
                }
            }
        }

        desenhar_tabuleiro(tab, jogadorX, jogadorY);
        desenhar_hud(tab);
        frames++;
    }

    destruir_tabuleiro(tab);
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    printf("\nPontuação final: %d\n", pontos);
    return 0;
}
