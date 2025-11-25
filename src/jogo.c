#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include "../include/jogo.h"
#include "../include/logica.h"
#include "../include/screen.h"
#include "../include/keyboard.h"

// --- BANCO DE PERGUNTAS (Português correto) ---
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
    {"Se P é V, valor de P -> Verdadeiro?", "Verdadeiro", "Falso", "0"},
    {"Se P é V, valor de P -> Falso?", "Verdadeiro", "Falso", "1"},
    {"Se P é F, valor de P -> Verdadeiro?", "Verdadeiro", "Falso", "0"},
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

// --- INTERFACE DE PERGUNTA (GUI) ---
int fazer_pergunta_gui(const char* p, const char* r1, const char* r2, int indice_correta) {
    // Posição Y calculada (Logo abaixo do HUD)
    int START_Y = MINY + ALTURA_JOGO + 4; 

    // 1. Limpa área das perguntas
    screenSetColor(WHITE, BLACK);
    for(int i=0; i<8; i++) {
        screenGotoxy(MINX, START_Y + i);
        printf("                                                                     "); 
    }
    screenUpdate(); 

    // 2. Desenha o Título e Pergunta
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(MINX, START_Y);     printf("=== PERGUNTA DE LOGICA ===");
    fflush(stdout);
    
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MINX, START_Y + 2); printf("PERGUNTA: %s", p);
    fflush(stdout);

    // 3. Desenha as Opções Formatadas [1] e [2]
    screenSetColor(CYAN, BLACK); // Cor de destaque
    
    // Opção 1 (com recuo de 2 espaços para visual limpo)
    screenGotoxy(MINX + 2, START_Y + 4); 
    printf("[1] %s", r1);
    fflush(stdout);

    // Opção 2
    screenGotoxy(MINX + 2, START_Y + 5); 
    printf("[2] %s", r2);
    fflush(stdout);
    
    // 4. Input
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MINX, START_Y + 7); printf("Digite [1] ou [2] (Q sair): ");
    
    screenUpdate(); // FORÇA O DESENHO FINAL NA TELA

    char ch = ' ';
    // Loop de espera
    while(ch != '1' && ch != '2' && ch != 'q' && ch != 'Q') {
        if(keyhit()) {
            ch = readch();
        } else {
            usleep(10000); // 10ms delay para não travar CPU
        }
    }
    
    if (ch == 'q' || ch == 'Q') return -1;
    
    int resp_usuario = (ch == '1') ? 0 : 1;
    return (resp_usuario == indice_correta) ? 1 : 0;
}

// --- LÓGICA DO JOGO ---

void jogo_resetar_tubaroes(Tabuleiro *tab) {
    for(int i=0; i<tab->linhas; i++)
        for(int j=0; j<tab->colunas; j++)
            if(tab->matriz[i][j] == 'S') tab->matriz[i][j] = '.';

    for(int k=0; k<6; k++) {
        int rL = rand() % (tab->linhas - 2) + 1;
        int rC = rand() % (tab->colunas - 2) + 1;
        if (rL > 3 || rC > 3) tab->matriz[rL][rC] = 'S';
    }
}

void jogo_inicializar_tubaroes(Tabuleiro *tab) {
    jogo_resetar_tubaroes(tab);
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
        screenGotoxy(MINX, START_Y + 7); 
        printf("                                              "); 
        screenGotoxy(MINX, START_Y + 7);

        if (res == -1) return 0; 

        if (res == 1) {
            j->pontuacao += PONTOS_NORMAL;
            screenSetColor(GREEN, BLACK);
            printf("ACERTOU! +%d pts. ", PONTOS_NORMAL);
        } else {
            screenSetColor(RED, BLACK);
            printf("ERROU! ");
        }
        
        printf("(Pressione ENTER)");
        fflush(stdout);
        screenUpdate(); 

        // Espera OBRIGATÓRIA pelo ENTER
        while(1) {
            if(keyhit()) {
                char c = readch();
                if (c == 10 || c == 13 || c == ' ') break; // Aceita Enter ou Espaço
            }
            usleep(10000);
        }
    }
    
    // Limpeza final
    int START_Y = MINY + ALTURA_JOGO + 4;
    screenSetColor(WHITE, BLACK);
    for(int i=0; i<8; i++) {
        screenGotoxy(MINX, START_Y + i);
        printf("                                                                     ");
    }
    screenUpdate();
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
    printf("                                              "); 
    screenGotoxy(MINX, START_Y + 7);

    if (res == 1) {
        j->pontuacao += PONTOS_DIFICIL;
        screenSetColor(GREEN, BLACK);
        printf("ESCAPOU! +%d pts. (ENTER)", PONTOS_DIFICIL);
        return 1;
    } else {
        j->vidas--;
        screenSetColor(RED, BLACK);
        printf("ERROU! -1 VIDA. (ENTER)");
        return 0;
    }
    fflush(stdout);
    screenUpdate();
    
    // Espera ENTER
    while(1) {
        if(keyhit()) {
            char c = readch();
            if (c == 10 || c == 13 || c == ' ') break;
        }
        usleep(10000);
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