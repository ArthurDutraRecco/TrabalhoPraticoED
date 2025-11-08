#include "times.h"
#include "partidas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macro para definição/teste do tamanho do padding nas tabelas
#define PADDING 15
#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

// Retorna a pontuação de um time
int pontos_ganhos(Time *time)
{
    return 3*(time->V) + (time->E);
}

// Retorna o saldo de gols de um time
int saldo_gols(Time *time)
{
    return (time->GM) - (time->GS);
}

// Retorna a largura visual de uma string UTF-8, para formatação correta das tabelas
int largura_visual_utf8(const char *s)
{
    int largura = 0;
    while (*s) {
        if ((*s & 0xC0) != 0x80) {
            largura++;
        }
        s++;
    }
    return largura;
}

// Aloca, carrega os times do CSV e retorna o ponteiro
BDTimes* bdt_carregar(const char* arquivo_times)
{
    FILE* times = fopen(arquivo_times, "r");
    if (times == NULL) {
        printf("Erro ao abrir o arquivo '%s'.\n", arquivo_times);
        return NULL;
    }
    
    char linha[128];

    fgets(linha, sizeof(linha), times); // Descarta o cabeçalho
    
    BDTimes *bdt = (BDTimes*)malloc(sizeof(BDTimes));
    if (bdt == NULL) {
        printf("Erro ao alocar memória para a estrutura BDTimes.\n");
        fclose(times);
        return NULL;
    }
    
    int i = 0;
    
    while (fgets(linha, sizeof(linha), times) && i < sizeof(bdt->times)/sizeof(bdt->times[0])) {
        sscanf(linha, "%d,%49[^\n]", &bdt->times[i].ID, bdt->times[i].nome);
        bdt->times[i].V = 0;
        bdt->times[i].E = 0;    
        bdt->times[i].D = 0;
        bdt->times[i].GM = 0;
        bdt->times[i].GS = 0;
        i++;
    }

    bdt->num_times = i;
    
    fclose(times);

    return bdt;
}

// Libera a memória alocada para BDTimes
void bdt_liberar(BDTimes* bdt)
{
    if (bdt != NULL) {
        free(bdt);
    }
}

// Busca um time pelo ID
Time* bdt_busca_por_id(BDTimes* bdt, int id)
{
    for (int i = 0; i < bdt->num_times; i++) {
        if (bdt->times[i].ID == id) {
            return &bdt->times[i];
        }
    }
    return NULL;
}

// Processa as partidas e atualiza os dados dos times
void bdt_processar_partidas(BDTimes* bdt, BDPartidas* bdp)
{
    if (bdt == NULL || bdp == NULL) return;

    for (int i = 0; i < bdp->num_partidas; i++) {
        Partida* partida = &bdp->partidas[i];
        
        Time* time_1 = bdt_busca_por_id(bdt, partida->time1_id);
        Time* time_2 = bdt_busca_por_id(bdt, partida->time2_id);

        // Pular partidas com times inválidos
        if (time_1 == NULL || time_2 == NULL) {
            continue;
        }
        
        time_1->GM += partida->gols_time1;
        time_1->GS += partida->gols_time2;
        
        time_2->GM += partida->gols_time2;
        time_2->GS += partida->gols_time1;
        
        if (partida->gols_time1 > partida->gols_time2) {
            time_1->V++;
            time_2->D++;
        } else if (partida->gols_time1 < partida->gols_time2) {
            time_2->V++;
            time_1->D++;
        } else {
            time_1->E++;
            time_2->E++;
        }
    }
}

// Implementa a funcionalidade 1 - Consultar time
void bdt_consultar_time(BDTimes* bdt)
{
    char prefixo[50];
    printf("\nDigite o nome ou prefixo do time: ");
    scanf("%49s", prefixo);

    int encontrado = 0;

    for (int i = 0; i < bdt->num_times; i++) {
        if (strncmp(bdt->times[i].nome, prefixo, strlen(prefixo)) == 0) {
            if (!encontrado) {
                printf("\n%-3s %-" TO_STRING(PADDING) "s %-3s %-3s %-3s %-3s %-3s %-3s %-3s\n",
                       "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");
                encontrado = 1;
                }

            Time* time = &bdt->times[i];
            int saldo = saldo_gols(time);
            int pontos = pontos_ganhos(time);

            int largura_nome = largura_visual_utf8(time->nome);
            int padding = PADDING - largura_nome;
            if (padding < 0) padding = 0;

            printf("%-3d %s%*s %-3d %-3d %-3d %-3d %-3d %-3d %-3d\n",
                time->ID, 
                time->nome,
                padding, "",
                time->V,
                time->E,
                time->D,
                time->GM, 
                time->GS, 
                saldo, 
                pontos);
        }
    }

    if (!encontrado) {
        printf("\nNenhum time encontrado com o prefixo \"%s\".\n", prefixo);
    }
}

// Implementa a funcionalidade 6 - Imprimir tabela de classificação
void bdt_imprimir_tabela(BDTimes* bdt) {
    printf("\nImprimindo classificação...\n");

    printf("\n%-3s %-" TO_STRING(PADDING) "s %-3s %-3s %-3s %-3s %-3s %-3s %-3s\n",
           "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");
    for (int i = 0; i < bdt->num_times; i++) {
        Time* time = &bdt->times[i];
        int saldo = saldo_gols(time);
        int pontos = pontos_ganhos(time);

        int largura_nome = largura_visual_utf8(time->nome);
        int padding = PADDING - largura_nome;
        if (padding < 0) padding = 0;

        printf("%-3d %s%*s %-3d %-3d %-3d %-3d %-3d %-3d %-3d\n",
               time->ID, 
               time->nome,
               padding, "",
               time->V,
               time->E,
               time->D,
               time->GM, 
               time->GS, 
               saldo, 
               pontos);
    }
}