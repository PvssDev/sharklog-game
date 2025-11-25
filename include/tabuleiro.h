#ifndef __TABULEIRO_H__
#define __TABULEIRO_H__

// --- NOVO: Definições de tamanho do Jogo ---
// Usamos 35 porque visualmente vai ocupar 70 espaços (devido aos emojis duplos)
#define LARGURA_JOGO 35 
#define ALTURA_JOGO  15

// Emoji do tubarão (UTF-8)
#define EMOJI_TUBARAO "🦈"

// Emoji do jogador surfista (UTF-8)
#define EMOJI_JOGADOR "🏄"

// Borda ASCII
#define BORDA_CANTOS       "+"
#define BORDA_HORIZONTAL   "-"
#define BORDA_VERTICAL     "|"

typedef struct {
    int linhas;
    int colunas;
    char **matriz;
} Tabuleiro;

Tabuleiro* criar_tabuleiro(int linhas, int colunas);
void destruir_tabuleiro(Tabuleiro *tab);
void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY);

#endif
