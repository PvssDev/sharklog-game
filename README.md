<img width="512" height="512" alt="Logo do Jogo SharkLog" src="https://github.com/user-attachments/assets/5380b90a-8ce2-4d41-9ce4-596b7fe2c4fd" />

# 🦈 SharkLog — Campo Minado de Boa Viagem

**Disciplina:** Programação Imperativa e Funcional - 2025.2 <br>
**Instituição:** CESAR School  
**Autores:**  
- Eduardo Henrique Albuquerque (@duduhnrq)  
- Luiz Henrique Souza da Conceição (@LouisLuos)
- Pedro Henrique da Silva Marrocos (@Pedrinhosds16)
- Pedro Vinicius Silva de Souza (@PvssDev)
- Paulo César da Silva Marrocos (@paulosds2318)
- Cauã dos Santos Nascimento (@Santos-dev25)

---

## 🎯 Descrição do Jogo

O **SharkLog** é um jogo de terminal (CLI) inspirado na praia de Boa Viagem, em Recife. O jogador deve se aventurar pelo mar, tentando chegar o mais longe possível **sem encontrar um tubarão**!  

Cada passo dado nas águas recifenses é um risco — você pode encontrar um **tubarão**, uma **corrente segura** ou até um **bônus de pontuação** escondido. O jogo termina quando o jogador é pego por um tubarão ou conquista a pontuação máxima atravessando toda a área segura.

---

## 💡 Temática e Lógica

O nome **SharkLog** vem da fusão entre:
- “Shark” (tubarão), símbolo clássico de Boa Viagem;
- “Log”, referência tanto à **Lógica Computacional** quanto ao **log de eventos** que registra cada jogada.

O jogo aplica princípios de **Lógica Proposicional**, onde cada célula do tabuleiro pode representar uma **proposição**.

*Exemplo:*
> “Há um tubarão próximo” → verdadeiro ou falso

Assim, o jogador deve usar **dedução lógica** com base em pistas para prever onde estão os perigos, semelhante ao raciocínio de **campo minado**.

---

## 🧠 Conceitos de Programação Utilizados

- **Structs:** representam o jogador, o tabuleiro e as células (ex: posição, estado, perigo, dica).
- **Ponteiros e alocação dinâmica:** utilizados na criação e destruição do tabuleiro de forma flexível.
- **Funções:** código modularizado em funções como `inicializarJogo()`, `moverJogador()`, `atualizarPontuacao()`, `verificarPerigo()`, etc.
- **Estruturas de decisão e repetição:** controle do fluxo principal do jogo (loops, condições, menus).
- **Recursividade:** utilizada para abrir múltiplas células seguras ou recalcular áreas adjacentes.
- **CLI-lib:** biblioteca para exibir o jogo no terminal com cores, posicionamento de cursor e entrada interativa.

---

## 🕹️ Regras do Jogo

1. O jogador começa na areia (linha inicial) e deve avançar em direção ao mar.  
2. Cada célula pode conter:
   - 🌊 **Segura:** o jogador pode pisar sem riscos;
   - ⚠️ **Pista:** há um tubarão próximo;
   - 🦈 **Tubarão:** o jogo termina;
   - 💎 **Bônus:** aumenta a pontuação.
3. O jogador se move usando as **setas do teclado** (ou `W`, `A`, `S`, `D`).
4. A pontuação aumenta conforme a distância percorrida e os bônus encontrados.
5. O jogo termina ao:
   - Encontrar um tubarão 🦈  
   - Ou completar o percurso com sucesso 🎉  

---

## 🧩 Estrutura do Projeto
```bash
SharkLog/
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
├── README.md # Este arquivo
├── Makefile # Compilação automatizada
└── LICENSE # Licença MIT
```

## ⚙️ Como Compilar e Executar

### 🔧 Pré-requisitos
- Sistema operacional **Linux** ou **macOS**
- Compilador **GCC**
- Biblioteca **CLI-lib** instalada (https://github.com/tgfb/cli-lib)

### 💻 Compilação manual:
```bash
gcc src/*.c -Iinclude -lcli -o build/sharklog
```
### ▶️ Execução
Para iniciar o jogo, execute o binário gerado dentro da pasta `build`:

```bash
./build/sharklog
```
## 🧮 Sistema de Pontuação
| Ação | Pontos |
|------|---------|
| Avançar para uma célula segura | +10 |
| Encontrar bônus | +50 |
| Terminar o jogo sem morrer | +200 |
| Encontrar tubarão | -100 e fim de jogo |

---

## 📚 Conceitos Lógicos Aplicados

O jogo explora **Lógica Proposicional** e **Raciocínio Dedutivo**:

- Cada célula representa uma **proposição p(x, y)**: “Existe um tubarão na célula (x, y)”.
- O jogador recebe **dicas booleanas** (verdadeiras ou falsas) sobre as células vizinhas.
- A tomada de decisão depende de inferências como:
  - `Se há pista → há tubarão em uma célula adjacente`
  - `Se não há pista → todas as células adjacentes são seguras`

Essas relações entre proposições simulam o raciocínio lógico computacional em um ambiente lúdico.

---

## 🧑‍💻 Créditos

Desenvolvido por alunos da **CESAR School**  
Sob orientação das disciplinas de:
- **Programação Imperativa e Funcional (PIF)**
- **Lógica Computacional**

---

## 📜 Licença
Este projeto pode ser utilizado sob a licença **MIT**.  
Sinta-se livre para estudar, modificar e expandir o SharkLog! 🦈
