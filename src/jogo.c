#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include "../include/jogo.h"
#include "../include/logica.h"
#include "../include/screen.h"
#include "../include/keyboard.h"

// --- BANCO DE PERGUNTAS (Seu JSON) ---
static const char* PERGUNTAS_NORMAIS[][4] = {
    {"Se P eh V e Q eh F, valor de P ^ Q?", "Verdadeiro", "Falso", "1"}, 
    {"Se P eh V e Q eh F, valor de P v Q?", "Verdadeiro", "Falso", "0"}, 
    {"Se P eh F e Q eh F, valor de P v Q?", "Verdadeiro", "Falso", "1"},
    {"Se P eh F e Q eh V, valor de P -> Q?", "Verdadeiro", "Falso", "0"},
    {"A operacao ~P representa:", "P eh verdadeiro", "A negacao de P", "1"},
    {"Se P <-> Q eh verdadeiro, entao:", "P e Q tem mesmo valor", "P e Q sao falsos", "0"},
    {"Qual eh o resultado de ~(Verdadeiro)?", "Verdadeiro", "Falso", "1"},
    {"Se P eh V e Q eh V, valor de P <-> Q?", "Verdadeiro", "Falso", "0"},
    {"A operacao P ^ Verdadeiro resulta em:", "P", "Falso", "0"},
    {"A operacao P v Falso resulta em:", "P", "Verdadeiro", "0"},
    {"Se P eh falso, qual valor de ~P?", "Verdadeiro", "Falso", "0"},
    {"Se P eh V, valor de P -> Verdadeiro?", "Verdadeiro", "Falso", "0"},
    {"Se P eh V, valor de P -> Falso?", "Verdadeiro", "Falso", "1"},
    {"Se P eh F, valor de P -> Verdadeiro?", "Verdadeiro", "Falso", "0"},
    {"A expressao P ^ ~P eh sempre:", "Falsa", "Verdadeira", "0"}, 
    {"A expressao P v ~P eh sempre:", "Verdadeira", "Falsa", "0"},
    {"Se P e Q sao V, P v (~Q ^ P) eh:", "Verdadeiro", "Falso", "0"},
    {"Se P eh F e Q eh V, ~P ^ Q eh:", "Verdadeiro", "Falso", "0"},
    {"Qual operacao exige ambos V?", "Ou (v)", "E (^)", "1"},
    {"Qual operacao so eh falsa se V -> F?", "Ou (v)", "Implica (->)", "1"} 
};

static const char* PERGUNTAS_DIFICEIS[][4] = {
    {"Se P=V, Q=F, valor de (P ^ Q) v (~Q)?", "Verdadeiro", "Falso", "0"},
    {"Qual expressao equivalente a ~(P v Q)?", "~P ^ ~Q", "~P v ~Q", "0"},
    {"Tabela P -> Q eh falsa apenas quando:", "P=V e Q=F", "P=F e Q=V", "0"},
    {"Qual destas proposicoes eh tautologia?", "P v ~P", "P ^ ~P", "0"},
    {"Se P=F, Q=V, valor de (P -> Q) ^ (Q -> P)?", "Verdadeiro", "Falso", "1"},
    {"Expressao ~(P -> Q) eh equivalente a:", "P ^ ~Q", "~P ^ Q", "0"},
    {"Qual expressao equivalente a (P <-> Q)?", "(P^Q)v(~P^~Q)", "(P^~Q)", "0"}
};

// --- INTERFACE DE PERGUNTA (GUI) ---
int fazer_pergunta_gui(const char* p, const char* r1, const char* r2, int indice_correta) {
    // Define a posição Y para desenhar LOGO ABAIXO da borda inferior
    // ALTURA_JOGO (15) + MINY (1) + Borda(1) + Margem(1) = Linha 18
    int START_Y = MINY + ALTURA_JOGO + 4; 

    // 1. Limpa APENAS a área das perguntas (Não usa screenClear global)
    screenSetColor(WHITE, BLACK);
    for(int i=0; i<8; i++) {
        screenGotoxy(MINX, START_Y + i);
        // Limpa a linha com espaços vazios para apagar texto antigo
        printf("                                                                     "); 
    }

    // 2. Desenha a pergunta
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(MINX, START_Y);     printf("=== PERGUNTA DE LOGICA ===");
    
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MINX, START_Y + 2); printf("PERGUNTA: %s", p);
    
    screenGotoxy(MINX, START_Y + 4); printf("1) %s", r1);
    screenGotoxy(MINX, START_Y + 5); printf("2) %s", r2);
    
    screenSetColor(CYAN, BLACK);
    screenGotoxy(MINX, START_Y + 7); printf("Digite [1] ou [2] (Q sair): ");
    
    char ch = ' ';
    while(ch != '1' && ch != '2' && ch != 'q' && ch != 'Q') {
        if(keyhit()) ch = readch();
    }
    
    if (ch == 'q' || ch == 'Q') return -1;
    
    int resp_usuario = (ch == '1') ? 0 : 1;
    return (resp_usuario == indice_correta) ? 1 : 0;
}

// --- LÓGICA DO JOGO ---

void jogo_inicializar_tubaroes(Tabuleiro *tab) {
    jogo_resetar_tubaroes(tab);
}

void jogo_resetar_tubaroes(Tabuleiro *tab) {
    for(int i=0; i<tab->linhas; i++)
        for(int j=0; j<tab->colunas; j++)
            if(tab->matriz[i][j] == 'S') tab->matriz[i][j] = '.';

    for(int k=0; k<6; k++) {
        int rL = rand() % (tab->linhas - 2) + 1; // Margem de segurança
        int rC = rand() % (tab->colunas - 2) + 1;
        if (rL > 3 || rC > 3) tab->matriz[rL][rC] = 'S';
    }
}

int jogo_fase_perguntas(Jogador *j) {
    int qtd_perguntas = 20; 
    
    for(int i=0; i<2; i++) {
        int idx = rand() % qtd_perguntas;
        int correta = atoi(PERGUNTAS_NORMAIS[idx][3]);

        int res = fazer_pergunta_gui(
            PERGUNTAS_NORMAIS[idx][0], 
            PERGUNTAS_NORMAIS[idx][1], 
            PERGUNTAS_NORMAIS[idx][2], 
            correta
        );
        
        int START_Y = MINY + ALTURA_JOGO + 4;
        screenGotoxy(MINX, START_Y + 7); // Mesma linha do input para sobrescrever
        printf("                                    "); // Limpa input
        screenGotoxy(MINX, START_Y + 7);

        if (res == -1) return 0; 

        if (res == 1) {
            j->pontuacao += PONTOS_NORMAL;
            screenSetColor(GREEN, BLACK);
            printf("ACERTOU! +%d pts. (Enter...)", PONTOS_NORMAL);
        } else {
            screenSetColor(RED, BLACK);
            printf("ERROU! (Enter...)");
        }
        
        while(!keyhit()); readch(); 
        while(keyhit()) readch();   
    }
    // Limpa a área de perguntas ao terminar a fase
    int START_Y = MINY + ALTURA_JOGO + 4;
    for(int i=0; i<8; i++) {
        screenGotoxy(MINX, START_Y + i);
        printf("                                                                     ");
    }
    return 1;
}

int jogo_pergunta_tubarao(Jogador *j) {
    int qtd_perguntas = 7; 
    int idx = rand() % qtd_perguntas;
    int correta = atoi(PERGUNTAS_DIFICEIS[idx][3]);

    int res = fazer_pergunta_gui(
        PERGUNTAS_DIFICEIS[idx][0], 
        PERGUNTAS_DIFICEIS[idx][1], 
        PERGUNTAS_DIFICEIS[idx][2], 
        correta
    );

    int START_Y = MINY + ALTURA_JOGO + 4;
    screenGotoxy(MINX, START_Y + 7);

    if (res == 1) {
        j->pontuacao += PONTOS_DIFICIL;
        screenSetColor(GREEN, BLACK);
        printf("ESCAPOU! +%d pts. (Enter...)", PONTOS_DIFICIL);
        return 1;
    } else {
        j->vidas--;
        screenSetColor(RED, BLACK);
        printf("ERROU! -1 VIDA. (Enter...)");
        return 0;
    }
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
    // HUD fica logo abaixo da borda inferior
    int Y_HUD = MINY + ALTURA_JOGO + 2; 
    
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MINX, Y_HUD);
    
    printf(" 🏄 PONTOS: %d  |  VIDAS: %d  |  [WASD] Mover | [Q] Sair ", j->pontuacao, j->vidas);
    // Limpa resto da linha
    printf("               ");
}