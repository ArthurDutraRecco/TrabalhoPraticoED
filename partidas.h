#ifndef PARTIDAS_H
#define PARTIDAS_H

// Declaração antecipada de BDTimes para evitar dependência circular
typedef struct bdtimes BDTimes;

// Nomeia o TAD para uma partida
typedef struct partida Partida;

// Nomeia o TAD para o vetor de todas as partidas
typedef struct bdpartidas BDPartidas;

// Aloca, carrega as partidas do CSV e retorna o ponteiro
BDPartidas* bdp_carregar(const char* arquivo_partidas);

// Libera a memória alocada para BDPartidas
void bdp_liberar(BDPartidas* bdp);

// Implementa a funcionalidade 2 - Consultar partidas
void bdp_consultar_partidas(BDPartidas* bdp, BDTimes* bdt);

// Implementa a funcionalidade 3 - Atualizar partida
int bdp_atualizar_partida(BDPartidas* bdp, BDTimes* bdt);

// Implementa a funcionalidade 4 - Remover partida
int bdp_remover_partida(BDPartidas* bdp, BDTimes* bdt);

// Implementa a funcionalidade 5 - Inserir partida
int bdp_inserir_partida(BDPartidas* bdp, BDTimes* bdt);

void bdp_processar_tabela(BDPartidas* bdp, BDTimes* bdt);

#endif