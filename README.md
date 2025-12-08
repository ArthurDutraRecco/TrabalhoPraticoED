# Trabalho de Programação — Parte II (Campeonato Computacional de Futebol)

Este repositório contém a implementação da segunda parte do trabalho prático da disciplina de Estrutura de Dados. O objetivo é expandir o sistema de gerenciamento de campeonato de futebol, migrando as estruturas de dados para **Listas Encadeadas** e implementando funcionalidades de manutenção (inserção, remoção, atualização) e ordenação.

## Estrutura do Repositório

* **`main.c`**: Arquivo principal contendo o menu, fluxo de execução e lógica de recálculo da tabela.
* **`times.c` / `times.h`**: Implementação do TAD Time e da Lista Encadeada de Times (`BDTimes`).
* **`partidas.c` / `partidas.h`**: Implementação do TAD Partida e da Lista Encadeada de Partidas (`BDPartidas`).
* **`Makefile`**: Arquivo para automação da compilação.
* **`bd_times.csv`**: Base de dados dos times.
* **`bd_partidas.csv`**: Bases de dados das partidas (pode variar conforme o cenário de teste).

## Tipos Abstratos de Dados (TADs)

O sistema foi refatorado para utilizar alocação dinâmica:

### 1. TAD Time (`times.h`)
Representa uma equipe individual. Armazena ID, nome e estatísticas. Sua estrutura é opaca (definida no `.c`) para garantir encapsulamento.

### 2. TAD BDTimes (`times.h`)
Gerencia a coleção de todos os times. Diferente da Parte I, agora utiliza uma **Lista Simplesmente Encadeada**, permitindo alocação dinâmica de memória conforme os dados são carregados.

### 3. TAD Partida (`partidas.h`)
Representa o registro de um jogo. Assim como o Time, sua estrutura interna é ocultada do usuário principal.

### 4. TAD BDPartidas (`partidas.h`)
Gerencia a coleção de partidas. Utiliza uma **Lista Simplesmente Encadeada**, suportando operações de inserção e remoção dinâmica de jogos sem limite fixo de tamanho.

## Decisões de Implementação

* **Listas Encadeadas:** Substituição dos vetores estáticos por listas encadeadas para permitir manipulação dinâmica de memória e suportar um número indefinido de registros.
* **TADs Opacos e Encapsulamento:** As definições das `structs` foram movidas para os arquivos `.c`, expondo apenas ponteiros no `.h`. Isso aumenta a modularização e impede acesso direto indevido aos dados.
* **Separação de Responsabilidades:** O módulo `partidas.c` itera sobre os jogos, mas delega ao módulo `times.c` a lógica de pontuar (vitória/empate/derrota), garantindo baixo acoplamento.
* **Recálculo Dinâmico:** Sempre que uma partida é inserida, removida ou atualizada, o sistema zera e recalcula todas as estatísticas da tabela para garantir consistência.
* **Ordenação (Bubble Sort):** Implementação de algoritmo de ordenação trocando os dados dos nós da lista, classificando os times por Pontos Ganhos, Vitórias e Saldo de Gols.

## Como Compilar e Executar

O projeto conta com um `Makefile` para facilitar a compilação. No terminal, dentro da pasta do projeto:

1.  **Compilar:**
    ```bash
    make
    ```

2.  **Executar:**
    ```bash
    ./TP
    ```

3.  **Limpar arquivos temporários:**
    ```bash
    make clean
    ```