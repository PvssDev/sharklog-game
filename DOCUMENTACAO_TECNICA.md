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

### Estrutura da célula (posição no tabuleiro)
```c
typedef struct {
    int temTubarao;
    int temBonus;
    int revelada;
    int dica; // 1 se há tubarão próximo
} Celula;
```

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
O **SharkLog** utiliza **Lógica Proposicional** para definir e deduzir o estado das células do mar.

### 6.1 Representação Lógica

Cada célula `(x, y)` do tabuleiro é uma proposição `P(x, y)`:

- `P(x, y) = 1` → Existe um tubarão na célula.

- `P(x, y) = 0` → A célula é segura.

A dica exibida para o jogador é calculada da seguinte forma:

> D(x, y) = 1 se ∃ P(i, j) = 1 em alguma célula adjacente

Ou seja, **se há ao menos um tubarão nas vizinhanças**, a dica é verdadeira (⚠️).

### 6.2 Inferências Lógicas

Durante o jogo, o jogador aplica raciocínios como:

- `¬D(x, y)` → Nenhum tubarão nas células adjacentes.

- `D(x, y)` → Pelo menos um tubarão está próximo.

- `D(x, y) ∧ ¬P(x, y)` → Existe perigo nas redondezas, mas a célula atual é segura.

Essas inferências permitem tomar decisões **baseadas em proposições booleanas**, refletindo os conteúdos da disciplina de **Lógica Computacional**.

---

## 🧮 7. Sistema de Pontuação
| Ação                           | Pontos             |
| ------------------------------ | ------------------ |
| Avançar para uma célula segura | +10                |
| Encontrar bônus                | +50                |
| Terminar o jogo sem morrer     | +200               |
| Encontrar tubarão              | -100 e fim de jogo |

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

- O jogador insere o nome.

- O tabuleiro é gerado dinamicamente.

- Tubarões são posicionados aleatoriamente.

### 9.2 Execução

- O jogador escolhe uma coordenada (linha e coluna).

- A célula é revelada:

    - Se for segura, mostra quantos tubarões há ao redor.

    - Se tiver tubarão, o jogo termina.

- A cada rodada, o sistema aplica inferência lógica para exibir dicas.

### 9.3 Pontuação

- +10 pontos para cada célula segura revelada.

- -5 pontos para tentativas incorretas (se o modo lógico estiver ativado).

- Bônus por concluir o tabuleiro sem perder.

### 9.4 Fim de jogo

- Exibe pontuação final.

- Oferece opção de reiniciar.

---

## ⚙️ 10. Compilação e Execução

### Compilação manual:
```bash
gcc src/*.c -Iinclude -lcli -o build/sharklog
```
### Execução:
```bash
./build/sharklog
```

---

## 📚 11. Conclusão

O projeto **SharkLog** cumpre os requisitos de **PIF** e **Lógica Computacional**, integrando:

- conceitos de lógica formal aplicados à mecânica do jogo;

- modularização e boas práticas de programação em C;

- manipulação dinâmica de memória;

- interface interativa via terminal.

O jogo promove o raciocínio lógico do jogador, estimulando dedução e estratégia enquanto explora as águas (perigosas) de Boa Viagem. 🦈🌊
