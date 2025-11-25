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
    char *buf = malloc(len + 1);
    if (!buf) { fclose(f); return NULL; }
    fread(buf, 1, len, f);
    buf[len] = '\0';
    fclose(f);
    return buf;
}

// Helpers simples para extrair tokens do JSON mínimo
static char* find_between(char *src, const char *start, const char *end) {
    char *a = strstr(src, start);
    if (!a) return NULL;
    a += strlen(start);
    char *b = strstr(a, end);
    if (!b) return NULL;
    size_t n = b - a;
    char *out = malloc(n+1);
    strncpy(out, a, n);
    out[n] = '\0';
    return out;
}

// remove leading/trailing spaces and quotes
static void trim_quotes(char *s) {
    // trim leading spaces
    while (*s && isspace((unsigned char)*s)) memmove(s, s+1, strlen(s));
    // remove starting quote
    if (s[0] == '\"') {
        size_t len = strlen(s);
        if (s[len-1] == '\"') {
            s[len-1] = '\0';
            memmove(s, s+1, strlen(s));
        } else {
            // remove first quote only
            memmove(s, s+1, strlen(s));
        }
    }
    // trim trailing spaces
    size_t l = strlen(s);
    while (l>0 && isspace((unsigned char)s[l-1])) { s[l-1]='\0'; l--; }
}

// parse alternativas array like ["A","B","C"]
// returns number parsed and fills dest
static int parse_array_strings(char *src, char dest[][MAX_TEXTO], int maxdest) {
    int count = 0;
    char *p = src;
    while ((p = strchr(p, '\"')) != NULL && count < maxdest) {
        p++; // move after quote
        char *q = strchr(p, '\"');
        if (!q) break;
        int n = q - p;
        if (n >= MAX_TEXTO) n = MAX_TEXTO-1;
        strncpy(dest[count], p, n);
        dest[count][n] = '\0';
        count++;
        p = q + 1;
    }
    return count;
}

int carregar_perguntas(const char *path, BancoPerguntas *b) {
    if (!path || !b) return 0;
    char *file = read_file(path);
    if (!file) return 0;

    b->qtd_rodada = 0;
    b->qtd_tubarao = 0;

    // Extract the perguntas_rodada block
    char *rod_block = find_between(file, "\"perguntas_rodada\"", "]"); // up to closing bracket
    if (rod_block) {
        // we want the substring starting at '[' to the matching ']' we already used find_between incorrectly: adjust
        char *start = strstr(file, "\"perguntas_rodada\"");
        if (start) {
            start = strchr(start, '[');
            if (start) {
                char *end = start;
                int depth = 0;
                while (*end) {
                    if (*end == '[') depth++;
                    else if (*end == ']') {
                        depth--;
                        if (depth == 0) { end++; break; }
                    }
                    end++;
                }
                if (end) {
                    size_t n = end - start;
                    char *arr = malloc(n+1);
                    strncpy(arr, start, n);
                    arr[n] = '\0';
                    // iterate objects inside arr
                    char *obj = arr;
                    while ((obj = strstr(obj, "{")) != NULL && b->qtd_rodada < MAX_PERGUNTAS) {
                        char *close = strstr(obj, "}");
                        if (!close) break;
                        size_t on = close - obj + 1;
                        char *objtxt = malloc(on+1);
                        strncpy(objtxt, obj, on);
                        objtxt[on] = '\0';
                        // parse fields
                        char *p_perg = find_between(objtxt, "\"pergunta\"", ",");
                        if (!p_perg) p_perg = find_between(objtxt, "\"pergunta\"", "}");
                        if (p_perg) {
                            trim_quotes(p_perg);
                            strncpy(b->rodada[b->qtd_rodada].pergunta, p_perg, MAX_TEXTO-1);
                            free(p_perg);
                        }
                        // alternativas
                        char *alt = find_between(objtxt, "\"alternativas\"", "]");
                        if (alt) {
                            char *sq = strchr(alt, '[');
                            if (sq) {
                                char altcopy[4096];
                                strncpy(altcopy, sq, sizeof(altcopy)-1);
                                altcopy[sizeof(altcopy)-1] = '\0';
                                b->rodada[b->qtd_rodada].n_alternativas = parse_array_strings(altcopy, b->rodada[b->qtd_rodada].alternativas, MAX_ALTERNATIVAS);
                            }
                            free(alt);
                        }
                        // correta
                        char *c = find_between(objtxt, "\"correta\"", ",");
                        if (!c) c = find_between(objtxt, "\"correta\"", "}");
                        if (c) {
                            trim_quotes(c);
                            int idx = atoi(c);
                            b->rodada[b->qtd_rodada].correta = idx;
                            free(c);
                        }
                        free(objtxt);
                        b->qtd_rodada++;
                        obj = close + 1;
                    }
                    free(arr);
                }
            }
        }
    }

    // perguntas_tubarao (same approach)
    char *start2 = strstr(file, "\"perguntas_tubarao\"");
    if (start2) {
        start2 = strchr(start2, '[');
        if (start2) {
            char *end = start2;
            int depth = 0;
            while (*end) {
                if (*end == '[') depth++;
                else if (*end == ']') {
                    depth--;
                    if (depth == 0) { end++; break; }
                }
                end++;
            }
            if (end) {
                size_t n = end - start2;
                char *arr = malloc(n+1);
                strncpy(arr, start2, n);
                arr[n] = '\0';
                char *obj = arr;
                while ((obj = strstr(obj, "{")) != NULL && b->qtd_tubarao < MAX_PERGUNTAS) {
                    char *close = strstr(obj, "}");
                    if (!close) break;
                    size_t on = close - obj + 1;
                    char *objtxt = malloc(on+1);
                    strncpy(objtxt, obj, on);
                    objtxt[on] = '\0';
                    // parse fields
                    char *p_perg = find_between(objtxt, "\"pergunta\"", ",");
                    if (!p_perg) p_perg = find_between(objtxt, "\"pergunta\"", "}");
                    if (p_perg) {
                        trim_quotes(p_perg);
                        strncpy(b->tubarao[b->qtd_tubarao].pergunta, p_perg, MAX_TEXTO-1);
                        free(p_perg);
                    }
                    // alternativas
                    char *alt = find_between(objtxt, "\"alternativas\"", "]");
                    if (alt) {
                        char *sq = strchr(alt, '[');
                        if (sq) {
                            char altcopy[4096];
                            strncpy(altcopy, sq, sizeof(altcopy)-1);
                            altcopy[sizeof(altcopy)-1] = '\0';
                            b->tubarao[b->qtd_tubarao].n_alternativas = parse_array_strings(altcopy, b->tubarao[b->qtd_tubarao].alternativas, MAX_ALTERNATIVAS);
                        }
                        free(alt);
                    }
                    // correta
                    char *c = find_between(objtxt, "\"correta\"", ",");
                    if (!c) c = find_between(objtxt, "\"correta\"", "}");
                    if (c) {
                        trim_quotes(c);
                        int idx = atoi(c);
                        b->tubarao[b->qtd_tubarao].correta = idx;
                        free(c);
                    }
                    free(objtxt);
                    b->qtd_tubarao++;
                    obj = close + 1;
                }
                free(arr);
            }
        }
    }

    free(file);
    return 1;
}

int rand_between(int a, int b) {
    if (b < a) { int t=a; a=b; b=t; }
    return a + (rand() % (b - a + 1));
}

static int prompt_and_read_choice(Pergunta *p) {
    if (!p) return 0;
    // print question and alternatives
    printf("\n%s\n", p->pergunta);
    for (int i = 0; i < p->n_alternativas; i++) {
        printf("  %d) %s\n", i+1, p->alternativas[i]);
    }
    printf("Resposta (numero): ");
    int choice = 0;
    if (scanf("%d", &choice) != 1) {
        // flush input
        int c; while ((c = getchar()) != EOF && c != '\n');
        return -1;
    }
    return choice-1;
}

int perguntar_rodada(BancoPerguntas *b, int pontos_por_acerto) {
    if (!b || b->qtd_rodada == 0) return 0;
    int idx = rand_between(0, b->qtd_rodada-1);
    Pergunta *p = &b->rodada[idx];
    int ch = prompt_and_read_choice(p);
    if (ch == p->correta) {
        printf("Resposta correta! +%d pontos\n", pontos_por_acerto);
        return 1;
    } else {
        printf("Resposta errada!\n");
        return 0;
    }
}

int perguntar_tubarao(BancoPerguntas *b, int pontos_por_acerto) {
    if (!b || b->qtd_tubarao == 0) return 0;
    int idx = rand_between(0, b->qtd_tubarao-1);
    Pergunta *p = &b->tubarao[idx];
    int ch = prompt_and_read_choice(p);
    if (ch == p->correta) {
        printf("Resposta correta! +%d pontos\n", pontos_por_acerto);
        return 1;
    } else {
        printf("Resposta errada!\n");
        return 0;
    }
}
