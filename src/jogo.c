#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include "../include/jogo.h"
#include "../include/logica.h"
#include "../include/screen.h"
#include "../include/keyboard.h"

// --- BANCO DE PERGUNTAS ---
static const char* PERGUNTAS_NORMAIS[][3] = {
    {"Se P=V e Q=F, P^Q eh:", "Falso", "Verdadeiro"}, 
    {"Se P=F e Q=F, PvQ eh:", "Falso", "Verdadeiro"},
    {"Negacao de (PvQ):", "~P ^ ~Q", "~P v ~Q"},
    {"Tautologia eh sempre:", "Verdadeira", "Falsa"}
};

static const char* PERGUNTAS_DIFICEIS[][3] = {
    {"(P^Q)v(~Q) se Q=F resulta:", "Verdadeiro", "Falso"},
    {"P -> Q eh falso apenas se:", "V -> F", "F -> V"},
    {"P <-> Q eh verdade se:", "Iguais", "Diferentes"}
};

// Interface de GUI para perguntas
int fazer_pergunta_gui(const char* p, const char* r1, const char* r2, int correta) {
    screenClear();
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MINX+2, MINY+2); printf("=== RODADA DE LOGICA ===");
    screenGotoxy(MINX+2, MINY+4); printf("PERGUNTA: %s", p);
    screenGotoxy(MINX+2, MINY+6); printf("1) %s", r1);
    screenGotoxy(MINX+2, MINY+7); printf("2) %s", r2);
    screenGotoxy(MINX+2, MINY+9); printf("Digite 1 ou 2 (Q sair): ");
    
    char ch = ' ';
    while(ch != '1' && ch != '2' && ch != 'q' && ch != 'Q') {
        if(keyhit()) ch = readch();
    }
    if (ch == 'q' || ch == 'Q') return -1;
    
    int resp = (ch == '1') ? 1 : 2;
    return (resp == correta) ? 1 : 0;
}

void jogo_inicializar_tubaroes(Tabuleiro *tab) {
    // Reseta matriz (remove S antigos)
    for(int i=0; i<tab->linhas; i++)
        for(int j=0; j<tab->colunas; j++)
            if(tab->matriz[i][j] == 'S') tab->matriz[i][j] = '.';

    // Cria 6 tubarões longe do jogador (1,1)
    for(int k=0; k<6; k++) {
        int rL = rand() % (tab->linhas - 4) + 2;
        int rC = rand() % (tab->colunas - 4) + 2;
        if (rL > 5 || rC > 5) tab->matriz[rL][rC] = 'S';
    }
}

int jogo_fase_perguntas(Jogador *j) {
    for(int i=0; i<2; i++) {
        int idx = rand() % 4;
        int res = fazer_pergunta_gui(PERGUNTAS_NORMAIS[idx][0], PERGUNTAS_NORMAIS[idx][1], PERGUNTAS_NORMAIS[idx][2], 1);
        
        screenGotoxy(MINX+2, MAXY-2);
        if (res == -1) return 0; // Sair

        if (res == 1) {
            j->pontuacao += PONTOS_NORMAL;
            printf("ACERTOU! +%d pts. (Enter para continuar)", PONTOS_NORMAL);
        } else {
            printf("ERROU! (Enter para continuar)");
        }
        while(!keyhit()); readch(); 
    }
    return 1;
}

int jogo_pergunta_tubarao(Jogador *j) {
    int idx = rand() % 3;
    int res = fazer_pergunta_gui(PERGUNTAS_DIFICEIS[idx][0], PERGUNTAS_DIFICEIS[idx][1], PERGUNTAS_DIFICEIS[idx][2], 1);

    screenGotoxy(MINX+2, MAXY-2);
    if (res == 1) {
        j->pontuacao += PONTOS_DIFICIL;
        printf("ESCAPOU! +%d pts. (Enter para continuar)", PONTOS_DIFICIL);
        return 1;
    } else {
        j->vidas--;
        printf("ERROU! -1 VIDA. (Enter para continuar)");
        return 0;
    }
}

void jogo_mover_tubaroes(Tabuleiro *tab, Jogador *j) {
    // Matriz temporária para movimento simultâneo
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
                
                // IA Básica: Persegue
                if (abs(x - j->x) > abs(y - j->y)) {
                    if (x < j->x) novoX++; else if (x > j->x) novoX--;
                } else {
                    if (y < j->y) novoY++; else if (y > j->y) novoY--;
                }
                
                // Verifica colisão na matriz NOVA e limites via logica
                if (posicao_valida(novoX, novoY, tab->linhas, tab->colunas) && novaMatriz[novoY][novoX] == '.') {
                    novaMatriz[novoY][novoX] = 'S';
                } else {
                    novaMatriz[y][x] = 'S'; // Bloqueado, fica parado
                }
            }
        }
    }
    
    // Aplica movimentos
    for(int i=0; i<tab->linhas; i++) {
        memcpy(tab->matriz[i], novaMatriz[i], tab->colunas);
        free(novaMatriz[i]);
    }
    free(novaMatriz);
}

void desenhar_HUD(Jogador *j) {
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MINX, MAXY);
    for(int i=MINX; i<=MAXX; i++) printf("-"); 
    screenGotoxy(MINX + 2, MAXY);
    printf(" 🏄 PONTOS: %d  |  VIDAS: %d  |  [WASD] Mover | [Q] Sair ", j->pontuacao, j->vidas);
}