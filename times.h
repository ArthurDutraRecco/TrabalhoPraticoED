#ifndef TIMES_H
#define TIMES_H

#include "partidas.h"

// Define o TAD para um time
typedef struct time
{
    int ID; // Identificador do time
    char nome[50];
    int V; // Vitórias
    int E; // Empates
    int D; // Derrotas
    int GM; // Gols marcados
    int GS; // Gols sofridos
} Time;

// Define o TAD para o vetor de todos os times
typedef struct bdtimes
{
    Time times[30]; // Vetor estático para 30 times
    int num_times;
} BDTimes;

// Retorna a pontuação de um time
int pontos_ganhos(Time *time);

// Retorna o saldo de gols de um time
int saldo_gols(Time *time);

// Retorna a largura visual de uma string UTF-8, para formatação correta das tabelas
int largura_visual_utf8(const char *s);

// Aloca, carrega os times do CSV e retorna o ponteiro
BDTimes* bdt_carregar(const char* arquivo_times); 

// Libera a memória alocada para BDTimes
void bdt_liberar(BDTimes* bdt);

// Processa as partidas e atualiza os dados dos times
void bdt_processar_partidas(BDTimes* bdt, BDPartidas* bdp);

// Busca um time pelo ID
Time* bdt_busca_por_id(BDTimes* bdt, int id); 

// Implementa a funcionalidade 1 - Consultar time
void bdt_consultar_time(BDTimes* bdt); 

// Implementa a funcionalidade 6 - Imprimir tabela de classificação
void bdt_imprimir_tabela(BDTimes* bdt);

#endif