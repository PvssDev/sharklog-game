//Contém as funções que podem ser chamadas por outros módulos (como a main).
 

#ifndef JOGO_H
#define JOGO_H

// Configura o estado inicial do jogo (ex: posição de elementos).
void jogo_inicializar(void);

// Processa uma tecla de entrada do usuário.
void jogo_processar_input(int tecla);

// Atualiza o estado lógico do jogo (ex: movimento, colisões).
// Esta função é chamada a cada "tick" do temporizador.
void jogo_atualizar_estado(void);

// Desenha o estado atual do jogo na tela.
void jogo_desenhar(void);

// Verifica se o jogo deve terminar.
// Retorna 1 (true) para terminar, 0 (false) para continuar.
int jogo_deve_terminar(void);

// Libera recursos alocados pelo jogo antes de fechar.
void jogo_finalizar(void);

#endif // JOGO_H