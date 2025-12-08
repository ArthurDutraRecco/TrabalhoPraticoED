#ifndef TIMES_H
#define TIMES_H

// Dá nome ao TAD de um time
typedef struct time Time;

// Dá nome ao TAD de um nó da lista encadeada de times
typedef struct bdtimes BDTimes;

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

// Busca um time pelo ID
Time* bdt_busca_por_id(BDTimes* bdt, int id); 

// Implementa a funcionalidade 1 - Consultar time
void bdt_consultar_time(BDTimes* bdt); 

// Implementa a funcionalidade 6 - Imprimir tabela de classificação
void bdt_imprimir_tabela(BDTimes* bdt);

// Zera as estatísticas de todos os times
void bdt_zerar_estatisticas(BDTimes* bdt);

// Funções de acesso aos dados de Time
int time_get_id(Time* t);
char* time_get_nome(Time* t);
int time_get_pontos(Time* t);

// Atualiza os dados do time com base em um resultado de jogo
void time_registrar_resultado(Time* t, int GM, int GS);

#endif