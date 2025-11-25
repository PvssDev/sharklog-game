#ifndef __LOGICA_H__
#define __LOGICA_H__

#define MAX_PERGUNTAS 128
#define MAX_ALTERNATIVAS 8
#define MAX_TEXTO 256

typedef struct {
    char pergunta[MAX_TEXTO];
    char alternativas[MAX_ALTERNATIVAS][MAX_TEXTO];
    int n_alternativas;
    int correta;
} Pergunta;

typedef struct {
    Pergunta rodada[MAX_PERGUNTAS];
    int qtd_rodada;
    Pergunta tubarao[MAX_PERGUNTAS];
    int qtd_tubarao;
} BancoPerguntas;

int carregar_perguntas(const char *path, BancoPerguntas *b);
int perguntar_rodada(BancoPerguntas *b, int pontos_por_acerto);
int perguntar_tubarao(BancoPerguntas *b, int pontos_por_acerto);
int rand_between(int a, int b);

#endif
