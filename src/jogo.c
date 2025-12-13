/**
 * src/jogo.c
 * Versão: menu atualizado
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

#ifndef ALTURA_JOGO
#define ALTURA_JOGO 15
#endif
#ifndef LARGURA_JOGO
#define LARGURA_JOGO 35
#endif

// --- ESTADO DA LULA (static para encapsulamento) ---
static int lula_ativa = 0;
static int lula_contagem_acertos = 0; // Precisa de 5 para sumir
static int lula_ultimo_spawn_pontos = 0; // Controla o spawn a cada 50 pts
static int lula_passos_jogador = 0; // Controla a velocidade (1 passo dela a cada 2 do jogador)

// --- BANCO DE PERGUNTAS ---
static const char* PERGUNTAS_NORMAIS[][4] = {
  {"Se P é V e Q é F, valor de P ^ Q?", "Verdadeiro", "Falso", "Falso"}, 
  {"Se P é V e Q é F, valor de P v Q?", "Verdadeiro", "Falso", "Verdadeiro"}, 
  {"Se P é F e Q é F, valor de P v Q?", "Verdadeiro", "Falso", "Falso"},
  {"Se P é F e Q é V, valor de P -> Q?", "Verdadeiro", "Falso", "Verdadeiro"},
  {"A operação ~P representa:", "P é verdadeiro", "A negação de P", "A negação de P"},
  {"Se P <-> Q é verdadeiro, então:", "P e Q têm mesmo valor", "P e Q são falsos", "P e Q têm mesmo valor"},
  {"Qual é o resultado de ~(Verdadeiro)?", "Verdadeiro", "Falso", "Falso"},
  {"Se P é V e Q é V, valor de P <-> Q?", "Verdadeiro", "Falso", "Verdadeiro"},
  {"A operação P ^ Verdadeiro resulta em:", "P", "Falso", "P"},
  {"A operação P v Falso resulta em:", "P", "Verdadeiro", "P"},
  {"Se P é falso, qual valor de ~P?", "Verdadeiro", "Falso", "Verdadeiro"},
  {"Se P é V e Q é V, valor de P -> Q é?", "Verdadeiro", "Falso", "Verdadeiro"},
  {"Se P é V e Q é F, valor de P -> Q é?", "Verdadeiro", "Falso", "Falso"},
  {"Se P é F e Q é F, valor de P -> Q é?", "Verdadeiro", "Falso", "Verdadeiro"},
  {"A expressão P ^ ~P é sempre:", "Falsa", "Verdadeira", "Falsa"}, 
  {"A expressão P v ~P é sempre:", "Verdadeira", "Falsa", "Verdadeira"},
  {"Se P e Q são V, P v (~Q ^ P) é:", "Verdadeiro", "Falso", "Verdadeiro"},
  {"Se P é F e Q é V, ~P ^ Q é:", "Verdadeiro", "Falso", "Verdadeiro"},
  {"Qual operação exige ambos V?", "Ou (v)", "E (^)", "E (^)"}, 
  {"Qual operação só é falsa se V -> F?", "Ou (v)", "Implica (->)", "Implica (->)"}, 
  {"Se P=V, Q=F, valor de (P ^ Q) v (~Q)?", "Verdadeiro", "Falso", "Falso"},
  {"Qual expressão equivalente a ~(P v Q)?", "~P ^ ~Q", "~P v ~Q", "~P ^ ~Q"},
  {"Tabela P -> Q é falsa apenas quando:", "P=V e Q=F", "P=F e Q=V", "P=V e Q=F"},
  {"Qual destas proposições é tautologia?", "P v ~P", "P ^ ~P", "P v ~P"},
  {"Se P=F, Q=V, valor de (P -> Q) ^ (Q -> P)?", "Verdadeiro", "Falso", "Falso"},
  {"Expressão ~(P -> Q) é equivalente a:", "P ^ ~Q", "~P ^ Q", "P ^ ~Q"},
  {"Qual expressão equivalente a (P <-> Q)?", "(P^Q)v(~P^~Q)", "(P^~Q)", "(P^Q)v(~P^~Q)"},
  {"Se P=V, Q=F, valor de (P ^ Q) v (~Q)?", "Verdadeiro", "Falso", "Falso"},
  {"Qual expressão equivalente a ~(P v Q)?", "~P ^ ~Q", "~P v ~Q", "~P ^ ~Q"},
  {"Tabela P -> Q é falsa apenas quando:", "P=V e Q=F", "P=F e Q=V", "P=V e Q=F"},
  {"Qual destas proposições é tautologia?", "P v ~P", "P ^ ~P", "P v ~P"},
  {"Se P=F, Q=V, valor de (P -> Q) ^ (Q -> P)?", "Verdadeiro", "Falso", "Falso"},
  {"Expressão ~(P -> Q) é equivalente a:", "P ^ ~Q", "~P ^ Q", "P ^ ~Q"},
  {"Qual expressão equivalente a (P <-> Q)?", "(P^Q)v(~P^~Q)", "(P^~Q)", "(P^Q)v(~P^~Q)"}
};

// Protótipo interno
void desenhar_HUD(Jogador *j);
int verificar_colisao_lula(Jogador *j, Tabuleiro *tab);

// --- LÓGICA DA LULA ---

void spawn_lula(Tabuleiro *tab, Jogador *j) {
    for(int k=0; k<100; k++) {
        int rL = rand() % (tab->linhas - 2) + 1;
        int rC = rand() % (tab->colunas - 2) + 1;
        
        if (abs(rL - j->y) + abs(rC - j->x) > 8) {
            if (tab->matriz[rL][rC] == '.') {
                tab->matriz[rL][rC] = 'L'; 
                lula_ativa = 1;
                lula_contagem_acertos = 0;
                lula_passos_jogador = 0; // Reseta contador de passos ao spawnar
                return;
            }
        }
    }
    // Fallback
    if (tab->matriz[1][1] == '.') {
        tab->matriz[1][1] = 'L';
        lula_ativa = 1;
        lula_contagem_acertos = 0;
        lula_passos_jogador = 0;
    }
}

void remover_lula(Tabuleiro *tab) {
    for(int i=0; i<tab->linhas; i++) {
        for(int j=0; j<tab->colunas; j++) {
            if(tab->matriz[i][j] == 'L') {
                tab->matriz[i][j] = '.';
            }
        }
    }
    lula_ativa = 0;
}

// Reseta o ciclo da Lula após colisão
void lula_colisao_reset(Tabuleiro *tab, Jogador *j) {
    if (!lula_ativa) return;
    
    remover_lula(tab);

    lula_contagem_acertos = 0;

    // Isso faz com que a lula só apareça novamente quando o jogador fizer +50 pontos a partir de AGORA
    lula_ultimo_spawn_pontos = j->pontuacao;
}

void teleportar_lula(Tabuleiro *tab, Jogador *j) {
    if (!lula_ativa) return;
    remover_lula(tab); // Temporário para reposicionar
    lula_ativa = 1; // Reativa pois remover zera
    
    for(int k=0; k<100; k++) {
        int rL = rand() % (tab->linhas - 2) + 1;
        int rC = rand() % (tab->colunas - 2) + 1;
        if (abs(rL - j->y) + abs(rC - j->x) > 6) {
            if (tab->matriz[rL][rC] == '.') {
                tab->matriz[rL][rC] = 'L';
                return;
            }
        }
    }
    tab->matriz[1][1] = 'L';
}

void mover_lula(Tabuleiro *tab, Jogador *j) {
    if (!lula_ativa) return;

    // Localiza a lula
    int lx = -1, ly = -1;
    for(int i=0; i<tab->linhas; i++) {
        for(int k=0; k<tab->colunas; k++) {
            if(tab->matriz[i][k] == 'L') {
                ly = i; lx = k;
                break;
            }
        }
    }

    if (lx == -1) return; 

    // --- DESVIO DE OBSTÁCULOS ---
    
    int dx = j->x - lx; // Diferença X
    int dy = j->y - ly; // Diferença Y
    
    int novoX = lx;
    int novoY = ly;
    int movimento_escolhido_valido = 0;

    // Tenta primeiro o eixo com maior distância
    // Se a distância X for maior, tenta mover na Horizontal
    if (abs(dx) > abs(dy)) {
        // Tenta Horizontal
        int testeX = lx + (dx > 0 ? 1 : -1);
        int testeY = ly;

        // Verifica se é válido (dentro do mapa) E não é um tubarão ('S')
        if (posicao_valida(testeX, testeY, tab->linhas, tab->colunas)) {
            char conteudo = tab->matriz[testeY][testeX];
            if (conteudo == '.' || (testeX == j->x && testeY == j->y)) {
                novoX = testeX;
                novoY = testeY;
                movimento_escolhido_valido = 1;
            }
        }

        // Se a horizontal falhou (tem tubarão), tenta vertical como segunda opção
        if (!movimento_escolhido_valido && dy != 0) {
            int altY = ly + (dy > 0 ? 1 : -1);
            int altX = lx;
            if (posicao_valida(altX, altY, tab->linhas, tab->colunas)) {
                char conteudo = tab->matriz[altY][altX];
                if (conteudo == '.' || (altX == j->x && altY == j->y)) {
                    novoX = altX;
                    novoY = altY;
                    movimento_escolhido_valido = 1;
                }
            }
        }

    } else {
        // Tenta Vertical (distância Y é maior ou igual)
        int testeY = ly + (dy > 0 ? 1 : -1);
        int testeX = lx;

        if (posicao_valida(testeX, testeY, tab->linhas, tab->colunas)) {
            char conteudo = tab->matriz[testeY][testeX];
            if (conteudo == '.' || (testeX == j->x && testeY == j->y)) {
                novoX = testeX;
                novoY = testeY;
                movimento_escolhido_valido = 1;
            }
        }

        // Se a vertical falhou (tem tubarão), tenta horizontal como segunda opção
        if (!movimento_escolhido_valido && dx != 0) {
            int altX = lx + (dx > 0 ? 1 : -1);
            int altY = ly;
            if (posicao_valida(altX, altY, tab->linhas, tab->colunas)) {
                char conteudo = tab->matriz[altY][altX];
                if (conteudo == '.' || (altX == j->x && altY == j->y)) {
                    novoX = altX;
                    novoY = altY;
                    movimento_escolhido_valido = 1;
                }
            }
        }
    }

    // Se encontrou um movimento válido, executa
    if (movimento_escolhido_valido) {
        tab->matriz[ly][lx] = '.';
        tab->matriz[novoY][novoX] = 'L';
    }
}

int verificar_colisao_lula(Jogador *j, Tabuleiro *tab) {
    if (tab->matriz[j->y][j->x] == 'L') return 1;
    return 0;
}

void gerenciar_lula_ciclo(Tabuleiro *tab, Jogador *j) {
    if (!lula_ativa) {
        if (j->pontuacao >= lula_ultimo_spawn_pontos + 50) {
            spawn_lula(tab, j);
            screenSetColor(MAGENTA, BLACK);
            screenGotoxy(MINX, MINY + ALTURA_JOGO + 4);
            printf("CUIDADO! UMA LULA GIGANTE APARECEU!");
	    fflush(stdout);
            screenUpdate();
            usleep(1500000);
        }
    } else {
        if (lula_contagem_acertos >= 5) {
            remover_lula(tab);
            lula_ultimo_spawn_pontos = j->pontuacao; 
            
            screenSetColor(CYAN, BLACK);
            screenGotoxy(MINX, MINY + ALTURA_JOGO + 4);
            printf("A LULA FUGIU! VOCE A ASSUSTOU!");
            fflush(stdout);
	    screenUpdate();
            usleep(1500000);
        }
    }
}

// --- FUNÇÕES DE TUBARÃO E JOGO ---
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
            jogo_mover_tubaroes(tab, j);
            
            if(lula_ativa) {
                if(verificar_colisao_lula(j, tab)) {
                    remover_lula(tab); 
                } else {
                    mover_lula(tab, j); 
                }
            }

            desenhar_tabuleiro(tab, j->x, j->y);
            desenhar_HUD(j);
            
            screenSetColor(RED, BLACK);
            screenGotoxy(MINX, START_Y);

            printf("                                                                                "); 
            
            screenGotoxy(MINX, START_Y); // Offset para centralizar
            printf("VOCE CAIU DA PRANCHA! OS INIMIGOS ESTAO CHEGANDO!");
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

        printf("                                                                                "); 
    }
    
    screenSetColor(YELLOW, BLACK);
    // Título
    screenGotoxy(MINX , START_Y);     
    printf("=== PERGUNTA DE LÓGICA ===");
    
    screenSetColor(WHITE, BLACK);
    // Pergunta
    screenGotoxy(MINX , START_Y + 1); 
    printf("P: %s", p); 
    
    screenSetColor(CYAN, BLACK);
    // Respostas
    screenGotoxy(MINX , START_Y + 2); printf("[1] %s", r1);
    screenGotoxy(MINX , START_Y + 3); printf("[2] %s", r2);
    
    screenSetColor(WHITE, BLACK);
    // Instrução
    screenGotoxy(MINX , START_Y + 4); 
    printf("Escolha [1] ou [2] (ou mova-se WASD)");
}

int fazer_pergunta_gui(Tabuleiro *tab, Jogador *j, const char* p, const char* r1, const char* r2, int indice_correta) {
    int respondendo = 1;
    int resultado = 0; 
    int ch = 0;
    int tick_lula = 0;

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
                    // LÓGICA DE VELOCIDADE DA LULA (1 para cada 2 do jogador)
                    if (lula_ativa) {
                        lula_passos_jogador++;
                        if (lula_passos_jogador >= 2) {
                            mover_lula(tab, j);
                            lula_passos_jogador = 0;
                        }
                    }

                    desenhar_tabuleiro(tab, j->x, j->y);
                    desenhar_HUD(j);
                    desenhar_painel_pergunta(p, r1, r2);
                    screenUpdate();
                }
            }
        }
        if (timerTimeOver()) {
            mover_tubaroes_aleatorio_pergunta(tab); 
            
            tick_lula++;
            if (tick_lula > 10 && lula_ativa) { 
                mover_lula(tab, j);
                tick_lula = 0;
            }

            desenhar_tabuleiro(tab, j->x, j->y);
            desenhar_HUD(j);
            desenhar_painel_pergunta(p, r1, r2);
            screenUpdate();
        }
        
        if (verificar_colisao(j, tab)) return -2; 
        if (verificar_colisao_lula(j, tab)) return -3; 
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
            if(tab->matriz[i][j] == 'S') {
                tab->matriz[i][j] = '.';
            }
        }
    jogo_inicializar_tubaroes(tab, pontuacao);
}

int jogo_fase_perguntas(Tabuleiro *tab, Jogador *j) {
    int qtd_perguntas = 20; 
    
    gerenciar_lula_ciclo(tab, j);

    for(int i=0; i<2; i++) {
        int idx = rand() % qtd_perguntas;
        int correta = atoi(PERGUNTAS_NORMAIS[idx][3]);

        int res = fazer_pergunta_gui(tab, j, PERGUNTAS_NORMAIS[idx][0], PERGUNTAS_NORMAIS[idx][1], PERGUNTAS_NORMAIS[idx][2], correta);
        
        int START_Y = MINY + ALTURA_JOGO + 3;
        
        // --- Limpa área de perguntas ---
        screenSetColor(WHITE, BLACK);
        for(int k=0; k<6; k++) {
            screenGotoxy(MINX, START_Y + k);
            printf("                                                                                ");
        }
        screenGotoxy(MINX, START_Y);

        if (res == -1) return 0; 
        if (res == -2) return 1; 
        
        if (res == -3) {
             screenSetColor(MAGENTA, BLACK);
             screenGotoxy(MINX , START_Y);
             printf("A LULA TE PEGOU! TENTACULOS GELADOS!");
             fflush(stdout);
             screenUpdate();
             usleep(1000 * 2000);
             
             lula_colisao_reset(tab, j); 
             
             animar_punicao(tab, j);
             return 1;
        }

        if (res == 1) {
            j->pontuacao += PONTOS_NORMAL;
            
            if (lula_ativa) {
                lula_contagem_acertos++;
                screenSetColor(CYAN, BLACK);
                screenGotoxy(MINX , START_Y);
                printf("ACERTOU! (Lula: %d/5 para despistar)", lula_contagem_acertos);
        
                fflush(stdout);
                screenUpdate();
                usleep(1000 * 1500);
            } else {
                screenSetColor(GREEN, BLACK);
                screenGotoxy(MINX , START_Y);
                printf("ACERTOU! +%d pts. ", PONTOS_NORMAL);
        
                fflush(stdout);
                screenUpdate();
                usleep(1000 * 800);
            }

            while(keyhit()) {
               readch();
            }
            
            gerenciar_lula_ciclo(tab, j);

        } else {
            screenSetColor(RED, BLACK);
            screenGotoxy(MINX , START_Y);
            printf("ERROU! PUNIÇÃO: VOCE ESCORREGOU...");
            fflush(stdout);
            screenUpdate();
            animar_punicao(tab, j); 
        }
    }
    return 1;
}

void desenhar_HUD(Jogador *j) {
    int Y_HUD = MINY + ALTURA_JOGO + 2; 
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MINX , Y_HUD);
    printf(" 🏄 PONTOS: %d  |  VIDAS: %d  |  [WASD] Mover | [Q] Sair ", j->pontuacao, j->vidas);
    
    if(lula_ativa) {
        screenSetColor(MAGENTA, BLACK);
        printf(" | 🦑 ALERTA (%d/5)", lula_contagem_acertos);
    } else {
        printf("                    ");
    }
    
    screenUpdate(); 
}