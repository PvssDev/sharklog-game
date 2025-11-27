#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "logica.h"

static char *read_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc(len+1);
    fread(buf, 1, len, f);
    buf[len]='\0';
    fclose(f);
    return buf;
}

// Funções auxiliares de parsing JSON, marcadas unused
static char* find_between(char *src, const char *start, const char *end) __attribute__((unused));
static void trim_quotes(char *s) __attribute__((unused));
static int parse_array_strings(char *src, char dest[][MAX_TEXTO], int maxdest) __attribute__((unused));

static char* find_between(char *src, const char *start, const char *end) {
    char *a = strstr(src, start);
    if (!a) return NULL;
    a += strlen(start);
    char *b = strstr(a, end);
    if (!b) return NULL;
    size_t n = b-a;
    char *out = malloc(n+1);
    strncpy(out,a,n);
    out[n]='\0';
    return out;
}

static void trim_quotes(char *s) {
    while(*s && isspace(*s)) memmove(s,s+1,strlen(s));
    if(s[0]=='\"') { size_t l=strlen(s); if(s[l-1]=='\"') s[l-1]='\0'; memmove(s,s+1,strlen(s));}
    size_t l = strlen(s);
    while(l>0 && isspace(s[l-1])) s[--l]='\0';
}

static int parse_array_strings(char *src, char dest[][MAX_TEXTO], int maxdest) {
    int count=0; char *p=src;
    while((p=strchr(p,'\"'))!=NULL && count<maxdest) {
        p++;
        char *q=strchr(p,'\"'); if(!q) break;
        int n=q-p; if(n>=MAX_TEXTO) n=MAX_TEXTO-1;
        strncpy(dest[count],p,n); dest[count][n]='\0';
        count++; p=q+1;
    }
    return count;
}

int carregar_perguntas(const char *path, BancoPerguntas *b) {
    if(!path || !b) return 0;
    char *file=read_file(path); if(!file) return 0;
    b->qtd_rodada=b->qtd_tubarao=0;

    // implementação simples: procura "perguntas_rodada" e "perguntas_tubarao"
    // ... sua lógica de parsing aqui ...

    free(file);
    return 1;
}

int rand_between(int a,int b){ if(b<a){int t=a;a=b;b=t;} return a+rand()%(b-a+1); }

static int prompt_and_read_choice(Pergunta *p){
    if(!p) return 0;
    printf("\n%s\n",p->pergunta);
    for(int i=0;i<p->n_alternativas;i++) printf("  %d) %s\n",i+1,p->alternativas[i]);
    printf("Resposta (numero): "); int ch=0;
    if(scanf("%d",&ch)!=1){ int c; while((c=getchar())!=EOF && c!='\n'); return -1;}
    return ch-1;
}

int perguntar_rodada(BancoPerguntas *b,int pts){ 
    if(!b||b->qtd_rodada==0) return 0;
    int idx=rand_between(0,b->qtd_rodada-1); Pergunta *p=&b->rodada[idx];
    int ch=prompt_and_read_choice(p);
    if(ch==p->correta){ printf("Resposta correta! +%d pontos\n",pts); return 1; }
    else{ printf("Resposta errada!\n"); return 0; }
}

int perguntar_tubarao(BancoPerguntas *b,int pts){ 
    if(!b||b->qtd_tubarao==0) return 0;
    int idx=rand_between(0,b->qtd_tubarao-1); Pergunta *p=&b->tubarao[idx];
    int ch=prompt_and_read_choice(p);
    if(ch==p->correta){ printf("Resposta correta! +%d pontos\n",pts); return 1; }
    else{ printf("Resposta errada!\n"); return 0; }
}
