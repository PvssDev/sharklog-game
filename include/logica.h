#ifndef __LOGICA_H__
#define __LOGICA_H__

#include "tabuleiro.h"

#define MAX_PERGUNTAS 100
#define MAX_TEXTO 128
#define MAX_ALTERNATIVAS 4

typedef struct {
    char pergunta[MAX_TEXTO];
    char alternativas[MAX_ALTERNATIVAS][MAX_TEXTO];
    int correta;
    int n_alternativas;
} Pergunta;

typedef struct {
    Pergunta rodada[MAX_PERGUNTAS];
    Pergunta tubarao[MAX_PERGUNTAS];
    int qtd_rodada;
    int qtd_tubarao;
} BancoPerguntas;

int carregar_perguntas(const char *path, BancoPerguntas *b);
int perguntar_rodada(BancoPerguntas *b, int pts);
int perguntar_tubarao(BancoPerguntas *b, int pts);

#endif
