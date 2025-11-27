# 🦈 Documentação Técnica — SharkLog
### Programação Imperativa e Funcional (PIF) + Lógica Computacional  
**Instituição:** CESAR School  
**Semestre:** 2025.2  
**Integrantes:**  
- Eduardo Henrique Albuquerque (@duduhnrq)  
- Luiz Henrique Souza da Conceição (@LouisLuos)
- Pedro Henrique da Silva Marrocos (@Pedrinhosds16)
- Pedro Vinicius Silva de Souza (@PvssDev)
- Paulo César da Silva Marrocos (@paulosds2318)
- Cauã dos Santos Nascimento (@Santos-dev25)

---

## 💡 1. Introdução

O **SharkLog** é um jogo de terminal (CLI) desenvolvido em linguagem **C** com o auxílio da biblioteca **CLI-lib**, cujo objetivo é aplicar os conceitos das disciplinas de **Programação Imperativa e Funcional (PIF)** e **Lógica Computacional**.  

Inspirado na praia de **Boa Viagem**, o jogador precisa avançar sobre o mar evitando **tubarões** escondidos, utilizando raciocínio lógico baseado em **proposições e inferências**, semelhantes à dinâmica do clássico *Campo Minado*.

---

## 🎯 2. Objetivos do Projeto

- Implementar um jogo em **modo texto**, totalmente funcional no terminal.  
- Utilizar **estruturas de controle**, **funções**, **tipos estruturados**, **recursividade** e **alocação dinâmica**.  
- Aplicar **conceitos de lógica computacional** na mecânica do jogo.  
- Garantir **organização modular** e compatibilidade com a biblioteca **CLI-lib**.

---

## 🧩 3. Estrutura do Projeto
```bash
sharklog-game/
├── src/ # Código-fonte (.c)
│ ├── main.c
│ ├── jogo.c
│ ├── tabuleiro.c
│ ├── jogador.c
│ └── logica.c
├── include/ # Cabeçalhos (.h)
│ ├── jogo.h
│ ├── tabuleiro.h
│ ├── jogador.h
│ └── logica.h
├── data
│ └── perguntas.js
├── build/ # Saídas de compilação
├── DOCUMENTACAO_TECNICA.md # Documentação Técnica do Projeto
├── README.md # Este arquivo
├── Makefile # Compilação automatizada
└── LICENSE # Licença MIT
```


### 🔧 Principais módulos:
| Arquivo | Função |
|----------|--------|
| **main.c** | Loop principal, menu inicial e controle de estados do jogo. |
| **jogo.c** | Funções principais do gameplay: movimentação, pontuação e verificações. |
| **tabuleiro.c** | Criação e destruição dinâmica do tabuleiro. |
| **jogador.c** | Controle das ações e atributos do jogador. |
| **logica.c** | Cálculo das pistas e inferência lógica das células adjacentes. |

---

## 🧱 4. Estruturas de Dados Utilizadas

### Estrutura do tabuleiro
```c
typedef struct {
    int linhas;
    int colunas;
    Celula **matriz; // matriz dinâmica de células
} Tabuleiro;
```

### Estrutura do jogador
```c
typedef struct {
    int x, y;
    int pontuacao;
    int vivo;
} Jogador;
```

---

## 💻 5. Conceitos de Programação Aplicados
| Conceito                              | Aplicação no Jogo                                                                  |
| ------------------------------------- | ---------------------------------------------------------------------------------- |
| **Structs**                           | Representam jogador, tabuleiro e células.                                          |
| **Ponteiros e alocação dinâmica**     | Criação do tabuleiro e das células em tempo de execução com `malloc()` e `free()`. |
| **Funções**                           | Código modularizado para facilitar manutenção e leitura.                           |
| **Recursividade**                     | Revela automaticamente regiões seguras adjacentes, semelhante ao campo minado.     |
| **Estruturas de decisão e repetição** | Controle do fluxo do jogo (menus, movimentos e verificação de vitória/derrota).    |
| **Biblioteca CLI-lib**                | Manipula a interface do terminal com cores e posicionamento do cursor.             |

---

## 🧠 6. Aplicação da Lógica Computacional
O **SharkLog** utiliza **Lógica Proposicional** para te desafiar a desviar de tubarões em quanto responde questões de logica.

### 6.1 Tabela-Verdade e Conectivos Lógicos

O jogador é desafiado a resolver expressões lógicas em tempo real para progredir. As perguntas exigem conhecimento prático das tabelas-verdade dos principais conectivos:

Conjunção ($P \land Q$): Verdadeiro apenas se ambas as proposições forem verdadeiras.

Disjunção ($P \lor Q$): Verdadeiro se pelo menos uma proposição for verdadeira.

Condicional ($P \rightarrow Q$): Falso apenas se o antecedente for verdadeiro e o consequente for falso ($V \rightarrow F$).

Negação ($\neg P$): Inverte o valor verdade da proposição.
> D(x, y) = 1 se ∃ P(i, j) = 1 em alguma célula adjacente

Ou seja, **se há ao menos um tubarão nas vizinhanças**, a dica é verdadeira (⚠️).

### 6.2 Avaliação de Proposições e Tautologias

Durante o jogo, o sistema apresenta situações onde o jogador deve avaliar a validade de sentenças lógicas complexas:

Avaliação de Valoração: Dada uma valoração para $P$ e $Q$ (ex: $P=V, Q=F$), qual o valor verdade de $(P \land \neg Q)$?

Identificação de Tautologias: Reconhecer expressões que são sempre verdadeiras, independente da valoração (ex: $P \lor \neg P$).

Equivalências Lógicas: Identificar proposições equivalentes, como a contrapositiva ou as Leis de De Morgan.

Essas mecânicas forçam o jogador a aplicar raciocínio dedutivo rápido sob pressão de tempo, simulando a necessidade de pensamento lógico claro em situações críticas.
---

## 🧮 7. Sistema de Pontuação
| Ação                           | Consequência       |
| ------------------------------ | ------------------ |
| Responder de forma correta     | +10                |
| Errar resposta                 | punição            |
| Encontrar tubarão              | -1 de vida         |

A pontuação é armazenada dentro da estrutura do jogador e atualizada a cada jogada. Ao final, a pontuação total é exibida na interface CLI.

---

## 🖥️ 8. Uso da Biblioteca CLI-lib

A biblioteca **CLI-lib** é utilizada para criar uma interface amigável em modo texto.
Principais funções empregadas:

```c
cli_clear();      // Limpa a tela
cli_move(x, y);   // Move o cursor para coordenadas
cli_printf(" ");  // Escreve texto colorido
cli_getch();      // Captura entrada de tecla
```

Essas funções permitem desenhar o tabuleiro, mover o jogador e exibir as mensagens em tempo real.

---

## 🔁 9. Fluxo de Execução
### 9.1 Inicialização

- O tabuleiro é gerado dinamicamente.

- Tubarões são posicionados aleatoriamente.

### 9.2 Execução

- O jogador se move de acordo com os tubarões
  
- O jogador Tenta responder de forma correta as perguntas

- A cada rodada, o sistema aplica dificuldade adicionando tubarões.

### 9.3 Pontuação

- +10 pontos para pergunta correta.

### 9.4 Fim de jogo

- Exibe pontuação final.

- Oferece opção de reiniciar.

---

## ⚙️ 10. Compilação e Execução

### Compilação manual:
```bash
make clean
make
```
### Execução:
```bash
make run
```

---

## 📚 11. Conclusão

O projeto **SharkLog** cumpre os requisitos de **PIF** e **Lógica Computacional**, integrando:

- conceitos de lógica formal aplicados à mecânica do jogo;

- modularização e boas práticas de programação em C;

- manipulação dinâmica de memória;

- interface interativa via terminal.

O jogo promove o raciocínio lógico do jogador, estimulando dedução e estratégia enquanto explora as águas (perigosas) de Boa Viagem. 🦈🌊