/**
 * src/jogo.c
 * Lógica do Jogo (Perguntas, Movimento, Punição)
 * Versão: Sem Ondas
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <time.h> 

#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/timer.h"
#include "../include/logica.h"
#include "../include/tabuleiro.h" 
#include "../include/jogador.h"
#include "../include/jogo.h"

// Fallback
#ifndef ALTURA_JOGO
#define ALTURA_JOGO 15
#endif
#ifndef LARGURA_JOGO
#define LARGURA_JOGO 35
#endif

// --- BANCO DE PERGUNTAS ---
static const char* PERGUNTAS_NORMAIS[][4] = {
    {"Se P é V e Q é F, valor de P ^ Q?", "Verdadeiro", "Falso", "1"}, 
    {"Se P é V e Q é F, valor de P v Q?", "Verdadeiro", "Falso", "0"}, 
    {"Se P é F e Q é F, valor de P v Q?", "Verdadeiro", "Falso", "1"},
    {"Se P é F e Q é V, valor de P -> Q?", "Verdadeiro", "Falso", "0"},
    {"A operação ~P representa:", "P é verdadeiro", "A negação de P", "1"},
    {"Se P <-> Q é verdadeiro, então:", "P e Q têm mesmo valor", "P e Q são falsos", "0"},
    {"Qual é o resultado de ~(Verdadeiro)?", "Verdadeiro", "Falso", "1"},
    {"Se P é V e Q é V, valor de P <-> Q?", "Verdadeiro", "Falso", "0"},
    {"A operação P ^ Verdadeiro resulta em:", "P", "Falso", "0"},
    {"A operação P v Falso resulta em:", "P", "Verdadeiro", "0"},
    {"Se P é falso, qual valor de ~P?", "Verdadeiro", "Falso", "0"},
    {"Se P é V e Q é V, valor de P -> Q é?", "Verdadeiro", "Falso", "0"},
    {"Se P é V e Q é F, valor de P -> Q é?", "Verdadeiro", "Falso", "1"},
    {"Se P é F e Q é F, valor de P -> Q é?", "Verdadeiro", "Falso", "0"},
    {"A expressão P ^ ~P é sempre:", "Falsa", "Verdadeira", "0"}, 
    {"A expressão P v ~P é sempre:", "Verdadeira", "Falsa", "0"},
    {"Se P e Q são V, P v (~Q ^ P) é:", "Verdadeiro", "Falso", "0"},
    {"Se P é F e Q é V, ~P ^ Q é:", "Verdadeiro", "Falso", "0"},
    {"Qual operação exige ambos V?", "Ou (v)", "E (^)", "1"},
    {"Qual operação só é falsa se V -> F?", "Ou (v)", "Implica (->)", "1"} 
};

static const char* PERGUNTAS_DIFICEIS[][4] = {
    {"Se P=V, Q=F, valor de (P ^ Q) v (~Q)?", "Verdadeiro", "Falso", "0"},
    {"Qual expressão equivalente a ~(P v Q)?", "~P ^ ~Q", "~P v ~Q", "0"},
    {"Tabela P -> Q é falsa apenas quando:", "P=V e Q=F", "P=F e Q=V", "0"},
    {"Qual destas proposições é tautologia?", "P v ~P", "P ^ ~P", "0"},
    {"Se P=F, Q=V, valor de (P -> Q) ^ (Q -> P)?", "Verdadeiro", "Falso", "1"},
    {"Expressão ~(P -> Q) é equivalente a:", "P ^ ~Q", "~P ^ Q", "0"},
    {"Qual expressão equivalente a (P <-> Q)?", "(P^Q)v(~P^~Q)", "(P^~Q)", "0"}
};

void mover_tubaroes_aleatorio_pergunta(Tabuleiro *tab) {
    if (!tab) return;
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            if (tab->matriz[i][j] == 'S') {
                int dir = rand() % 4; 
                int ni = i, nj = j;
                switch(dir) {
                    case 0: nj--; break; case 1: nj++; break;
                    case 2: ni--; break; case 3: ni++; break;
                }
                if (ni > 0 && ni < tab->linhas && nj > 0 && nj < tab->colunas && tab->matriz[ni][nj] == '.') {
                    tab->matriz[ni][nj] = 'S';
                    tab->matriz[i][j] = '.';
                    if(dir == 1 || dir == 3) j++; 
                }
            }
        }
    }
}

void animar_punicao(Tabuleiro *tab, Jogador *j) {
    int START_Y = MINY + ALTURA_JOGO + 3;
    for(int k=0; k<300; k++) {
        if(keyhit()) readch(); 
        if (timerTimeOver()) {
            mover_tubaroes_aleatorio_pergunta(tab);
            desenhar_tabuleiro(tab, j->x, j->y);
            desenhar_HUD(j);
            screenSetColor(RED, BLACK);
            screenGotoxy(MINX, START_Y);
            printf("                                                                     "); 
            screenGotoxy(MINX, START_Y);
            printf("ERROU! PUNIÇÃO: CONGELADO... CUIDADO COM OS TUBARÕES!");
            screenUpdate();
        }
        usleep(10000); 
    }
}

void desenhar_painel_pergunta(const char* p, const char* r1, const char* r2) {
    int START_Y = MINY + ALTURA_JOGO + 3; 
    screenSetColor(WHITE, BLACK);
    for(int i=0; i<6; i++) {
        screenGotoxy(MINX, START_Y + i);
        printf("                                                                     "); 
    }
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(MINX, START_Y);     printf("=== PERGUNTA DE LÓGICA ===");
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MINX, START_Y + 1); printf("P: %s", p); 
    screenSetColor(CYAN, BLACK);
    screenGotoxy(MINX + 2, START_Y + 2); printf("[1] %s", r1);
    screenGotoxy(MINX + 2, START_Y + 3); printf("[2] %s", r2);
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MINX, START_Y + 4); printf("Escolha [1] ou [2] (ou mova-se WASD)");
}

int fazer_pergunta_gui(Tabuleiro *tab, Jogador *j, const char* p, const char* r1, const char* r2, int indice_correta) {
    int respondendo = 1;
    int resultado = 0; 
    int ch = 0;
    desenhar_tabuleiro(tab, j->x, j->y);
    desenhar_HUD(j);
    desenhar_painel_pergunta(p, r1, r2);
    screenUpdate();

    while(respondendo) {
        if(keyhit()) {
            ch = readch();
            if (ch == '1') { resultado = (indice_correta == 0) ? 1 : 0; respondendo = 0; }
            else if (ch == '2') { resultado = (indice_correta == 1) ? 1 : 0; respondendo = 0; }
            else if (ch == 'q' || ch == 'Q') { return -1; }
            else {
                int moveu = mover_jogador(j, tab, ch);
                if (moveu) {
                    desenhar_tabuleiro(tab, j->x, j->y);
                    desenhar_HUD(j);
                    desenhar_painel_pergunta(p, r1, r2);
                    screenUpdate();
                }
            }
        }
        if (timerTimeOver()) {
            mover_tubaroes_aleatorio_pergunta(tab);
            desenhar_tabuleiro(tab, j->x, j->y);
            desenhar_HUD(j);
            desenhar_painel_pergunta(p, r1, r2);
            screenUpdate();
        }
        if (verificar_colisao(j, tab)) return -2; 
    }
    return resultado;
}

// --- INICIALIZAÇÃO E RESET ---
void jogo_inicializar_tubaroes(Tabuleiro *tab, int pontuacao) {
    int tubaroes_existentes = 0;
    for(int i=0; i<tab->linhas; i++)
        for(int j=0; j<tab->colunas; j++)
            if(tab->matriz[i][j] == 'S') tubaroes_existentes++;

    int qtd_base = 6;
    int qtd_extra = pontuacao / 10; 
    int total_desejado = qtd_base + qtd_extra;
    
    int limite_mapa = (tab->linhas * tab->colunas) / 3;
    if (total_desejado > limite_mapa) total_desejado = limite_mapa;

    int faltam = total_desejado - tubaroes_existentes;

    if (faltam > 0) {
        for(int k=0; k<faltam; k++) {
            int rL = rand() % (tab->linhas - 2) + 1;
            int rC = rand() % (tab->colunas - 2) + 1;
            
            if (rL > 3 || rC > 3) {
                if (tab->matriz[rL][rC] == '.') {
                    tab->matriz[rL][rC] = 'S';
                } else {
                    k--; 
                }
            } else {
                k--; 
            }
        }
    }
}

void jogo_resetar_tubaroes(Tabuleiro *tab, int pontuacao) {
    for(int i=0; i<tab->linhas; i++)
        for(int j=0; j<tab->colunas; j++) {
            if(tab->matriz[i][j] == 'S') { // Removida verificação de ondas
                tab->matriz[i][j] = '.';
            }
        }
    jogo_inicializar_tubaroes(tab, pontuacao);
}

int jogo_fase_perguntas(Tabuleiro *tab, Jogador *j) {
    int qtd_perguntas = 20; 
    for(int i=0; i<2; i++) {
        int idx = rand() % qtd_perguntas;
        int correta = atoi(PERGUNTAS_NORMAIS[idx][3]);

        int res = fazer_pergunta_gui(tab, j, PERGUNTAS_NORMAIS[idx][0], PERGUNTAS_NORMAIS[idx][1], PERGUNTAS_NORMAIS[idx][2], correta);
        
        int START_Y = MINY + ALTURA_JOGO + 3;
        screenSetColor(WHITE, BLACK);
        for(int k=0; k<6; k++) {
            screenGotoxy(MINX, START_Y + k);
            printf("                                                                     ");
        }
        screenGotoxy(MINX, START_Y);

        if (res == -1) return 0; 
        if (res == -2) return 1; 

        if (res == 1) {
            j->pontuacao += PONTOS_NORMAL;
            screenSetColor(GREEN, BLACK);
            printf("ACERTOU! +%d pts. ", PONTOS_NORMAL);
            fflush(stdout);
            screenUpdate();
            usleep(1000 * 1000); 
        } else {
            screenSetColor(RED, BLACK);
            printf("ERROU! PUNIÇÃO: CONGELADO...");
            fflush(stdout);
            screenUpdate();
            animar_punicao(tab, j);
        }
    }
    return 1;
}

int jogo_pergunta_tubarao(Tabuleiro *tab, Jogador *j) {
    int qtd = 7; 
    int idx = rand() % qtd;
    int correta = atoi(PERGUNTAS_DIFICEIS[idx][3]);

    int res = fazer_pergunta_gui(tab, j, PERGUNTAS_DIFICEIS[idx][0], PERGUNTAS_DIFICEIS[idx][1], PERGUNTAS_DIFICEIS[idx][2], correta);

    int START_Y = MINY + ALTURA_JOGO + 3;
    screenGotoxy(MINX, START_Y + 5); 
    printf("                                              "); 
    screenGotoxy(MINX, START_Y + 5); 

    if (res == 1) {
        j->pontuacao += PONTOS_DIFICIL;
        screenSetColor(GREEN, BLACK);
        printf("ESCAPOU! +%d pts.", PONTOS_DIFICIL);
        fflush(stdout);
        screenUpdate();
        usleep(1000 * 1000); 
    } else {
        j->vidas--;
        screenSetColor(RED, BLACK);
        printf("ERROU! -1 VIDA & PUNIÇÃO DE 3s.");
        fflush(stdout);
        screenUpdate();
        animar_punicao(tab, j);
    }
    
    return (res == 1);
}

void jogo_mover_tubaroes(Tabuleiro *tab, Jogador *j) {
    char **novaMatriz = (char**)malloc(tab->linhas * sizeof(char*));
    for(int i=0; i<tab->linhas; i++) {
        novaMatriz[i] = (char*)malloc(tab->colunas);
        memcpy(novaMatriz[i], tab->matriz[i], tab->colunas);
    }
    for(int y=0; y<tab->linhas; y++) {
        for(int x=0; x<tab->colunas; x++) {
            if(tab->matriz[y][x] == 'S') {
                novaMatriz[y][x] = '.'; 
                int novoY = y, novoX = x;
                if (abs(x - j->x) > abs(y - j->y)) {
                    if (x < j->x) novoX++; else if (x > j->x) novoX--;
                } else {
                    if (y < j->y) novoY++; else if (y > j->y) novoY--;
                }
                if (posicao_valida(novoX, novoY, tab->linhas, tab->colunas) && novaMatriz[novoY][novoX] == '.') {
                    novaMatriz[novoY][novoX] = 'S';
                } else {
                    novaMatriz[y][x] = 'S'; 
                }
            }
        }
    }
    for(int i=0; i<tab->linhas; i++) {
        memcpy(tab->matriz[i], novaMatriz[i], tab->colunas);
        free(novaMatriz[i]);
    }
    free(novaMatriz);
}

void desenhar_HUD(Jogador *j) {
    int Y_HUD = MINY + ALTURA_JOGO + 2; 
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MINX, Y_HUD);
    printf(" 🏄 PONTOS: %d  |  VIDAS: %d  |  [WASD] Mover | [Q] Sair ", j->pontuacao, j->vidas);
    printf("               ");
    screenUpdate(); 
}