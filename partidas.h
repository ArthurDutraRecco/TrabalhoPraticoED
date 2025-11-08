#ifndef PARTIDAS_H
#define PARTIDAS_H

// Declaração antecipada para evitar dependência circular...
typedef struct bdtimes BDTimes;

// Define o TAD para uma partida
typedef struct partida
{
    int ID; // Identificador da partida
    int time1_id;  // ID do Time1
    int time2_id;  // ID do Time2
    int gols_time1; // Gols do Time1
    int gols_time2; // Gols do Time2
} Partida;

// Define o TAD para o vetor de todas as partidas
typedef struct bdpartidas
{
    Partida partidas[50]; // Vetor estático para 45 partidas com margem
    int num_partidas;
} BDPartidas;

// Aloca, carrega as partidas do CSV e retorna o ponteiro
BDPartidas* bdp_carregar(const char* arquivo_partidas);

// Libera a memória alocada para BDPartidas
void bdp_liberar(BDPartidas* bdp);

// Implementa a funcionalidade 2 - Consultar partidas
void bdp_consultar_partidas(BDPartidas* bdp, BDTimes* bdt);

#endif