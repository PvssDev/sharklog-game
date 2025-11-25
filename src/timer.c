/**
 * src/timer.c
 * Implementa as funções de temporização (ex: delay, inicialização).
 */

#include "timer.h"
#include <unistd.h> // Para usleep (necessário para timerDelay em Linux)
#include <stdio.h> // Para outras funções de timer que possam precisar de I/O

/**
 * Pausa a execução por um número especificado de milissegundos.
 * Esta é a função que estava faltando e causava o erro 'undefined reference'.
 * @param ms Tempo em milissegundos para esperar.
 */
void timerDelay(int ms) {
    // usleep espera em microsegundos, então multiplicamos por 1000
    if (ms > 0) {
        usleep(ms * 1000);
    }
}

// Stubs (implementações mínimas) para as outras funções de timer.h
// O código real destas funções é tipicamente mais complexo.

void timerInit(int valueMilliSec) {
    // Função para inicializar o temporizador.
}

void timerDestroy() {
    // Função para liberar recursos do temporizador.
}

void timerUpdateTimer(int valueMilliSec) {
    // Função para atualizar o valor do temporizador.
}

int timerTimeOver() {
    // Função para verificar se o tempo acabou.
    return 0; 
}

void timerPrint() {
    // Função para imprimir informações do timer.
}
