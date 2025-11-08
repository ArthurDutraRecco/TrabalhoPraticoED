# Trabalho de Programação — Parte I (Campeonato Computacional de Futebol)

Este repositório contém a implementação da primeira parte do trabalho prático da disciplina de Estrutura de Dados. O objetivo é um sistema em C para gerenciamento de um campeonato de futebol, permitindo carregar dados de times e partidas a partir de arquivos CSV e realizar consultas.

## Estrutura do Repositório

* **`main.c`**: Arquivo principal contendo o menu e o fluxo de execução.
* **`times.c` / `times.h`**: Implementação do TAD para times e gerenciamento do banco de times.
* **`partidas.c` / `partidas.h`**: Implementação do TAD para partidas e consultas relacionadas.
* **`Makefile`**: Arquivo para automação da compilação.
* **`times.csv`**: Base de dados dos times.
* **`partidas_*.csv`**: Bases de dados das partidas (vazio, parcial e completo) para diferentes cenários de teste.

## Tipos Abstratos de Dados (TADs)

O sistema foi estruturado utilizando os seguintes TADs:

### 1. TAD Time (`times.h`)
Representa uma equipe individual. Armazena o ID, nome e acumula as estatísticas calculadas (vitórias, empates, derrotas, gols marcados/sofridos).

### 2. TAD BDTimes (`times.h`)
Gerencia a coleção de todos os times do campeonato. Utiliza um vetor estático, dado que o número de times é fixo (10).

### 3. TAD Partida (`partidas.h`)
Representa o registro de um jogo, contendo os IDs dos times envolvidos e o placar.

### 4. TAD BDPartidas (`partidas.h`)
Gerencia a coleção de todas as partidas carregadas do arquivo CSV. Também utiliza um vetor estático para armazenamento.

## Decisões de Implementação

* **Estruturas Estáticas:** Conforme sugerido na especificação, foram utilizados vetores estáticos para armazenar os times e as partidas em memória, simplificando o gerenciamento já que o volume de dados é previsível nesta etapa.
* **Cálculo de Estatísticas:** O arquivo `times.csv` contém apenas ID e Nome. Todas as estatísticas (pontos, vitórias, saldo de gols, etc.) são calculadas em tempo de execução ao processar a lista de partidas carregadas.
* **Formatação de Saída:** Foi implementada uma função auxiliar para calcular a largura visual de strings UTF-8, garantindo que a tabela de classificação fique alinhada corretamente mesmo com caracteres acentuados nos nomes dos times.
* **Leitura de CSV:** A leitura dos arquivos utiliza `fgets` para leitura linha a linha e `sscanf` para extração ("parsing") dos campos.

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
