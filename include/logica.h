#ifndef __LOGICA_H__
#define __LOGICA_H__

#define MAX_PERGUNTAS 128
#define MAX_ALTERNATIVAS 8
#define MAX_TEXTO 256

typedef struct {
    char pergunta[MAX_TEXTO];
    char alternativas[MAX_ALTERNATIVAS][MAX_TEXTO];
    int n_alternativas;
    int correta; // index starting at 0
} Pergunta;

typedef struct {
    Pergunta rodada[MAX_PERGUNTAS];
    int qtd_rodada;
    Pergunta tubarao[MAX_PERGUNTAS];
    int qtd_tubarao;
} BancoPerguntas;

// carrega arquivo JSON simples
int carregar_perguntas(const char *path, BancoPerguntas *b);

// faz uma pergunta do tipo "rodada", índice aleatório; retorna 1 se acertou, 0 se errou
int perguntar_rodada(BancoPerguntas *b, int pontos_por_acerto);

// faz uma pergunta difícil (tubarão), índice aleatório; retorna 1 se acertou, 0 se errou
int perguntar_tubarao(BancoPerguntas *b, int pontos_por_acerto);

// função utilitária para escolher índice aleatório
int rand_between(int a, int b);

#endif
