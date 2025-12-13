

#ifndef TABULEIRO_H //basicamente ele pergunta se ja existe, se sim ele ignora

#define TABULEIRO_H// cria a etiqueta tabuleiro

// --- 1. CONFIGURAÇÕES CONSTANTES ---

/**
 * Define as dimensões lógicas do mapa.
 * Nota: A largura visual no terminal (em caracteres) será o dobro de LARGURA_JOGO,
 * para acomodar Emojis e manter o aspecto visual.
 */
#define LARGURA_JOGO 35 
#define ALTURA_JOGO  15

// Visuais (Emojis UTF-8)
#define EMOJI_TUBARAO "🦈"
#define EMOJI_JOGADOR "🏄"

// Visuais (Bordas ASCII)
#define BORDA_CANTOS       "+"
#define BORDA_HORIZONTAL   "-"
#define BORDA_VERTICAL     "|"

// --- 2. ESTRUTURA DE DADOS ---

/**
 * Estrutura principal que representa o tabuleiro do jogo.
 * Contém o tamanho da grade e o ponteiro para os dados reais.
 */
typedef struct {
    int linhas;     // Altura total do mapa
    int colunas;    // Largura total do mapa
    char **matriz;  // Ponteiro para ponteiro: matriz dinâmica onde 'S' é tubarão e '.' é água
} Tabuleiro;

// --- 3. PROTÓTIPOS DE FUNÇÕES (O Contrato) ---

/**
 * Aloca dinamicamente o tabuleiro na memória e o inicializa.
 * param linhas Altura desejada.
 * param colunas Largura desejada.
 * return Ponteiro para a nova estrutura Tabuleiro.
 */
Tabuleiro* criar_tabuleiro(int linhas, int colunas);

/**
 * Libera toda a memória alocada pela função 'criar_tabuleiro'.
 * É crucial para evitar vazamentos de memória (Memory Leaks).
 * param tab Ponteiro para o tabuleiro a ser destruído.
 */
void destruir_tabuleiro(Tabuleiro *tab);

/**
 * Renderiza o mapa, desenhando as bordas, a água, os tubarões e o jogador.
 * param tab Dados do mapa.
 * param jogadorX Posição X (coluna) do surfista.
 * param jogadorY Posição Y (linha) do surfista.
 */
void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY);

#endif // TABULEIRO_H