#include "partidas.h"
#include "times.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Macro para definição/teste do tamanho do padding nas tabelas
#define PADDING 15
#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

// Aloca, carrega as partidas do CSV e retorna o ponteiro
BDPartidas* bdp_carregar(const char* arquivo_partidas)
{
    FILE* partidas = fopen(arquivo_partidas, "r");
    if (partidas == NULL) {
        printf("Erro ao abrir o arquivo '%s'.\n", arquivo_partidas);
        return NULL;
    }
    
    char linha[128];

    fgets(linha, sizeof(linha), partidas); // Descarta o cabeçalho
    
    BDPartidas *bdp = (BDPartidas*)malloc(sizeof(BDPartidas));
    if (bdp == NULL) {
        printf("Erro ao alocar memória para a estrutura BDPartidas.\n");
        fclose(partidas);
        return NULL;
    }
    
    int i = 0;
    
    while (fgets(linha, sizeof(linha), partidas) && i < sizeof(bdp->partidas)/sizeof(bdp->partidas[0])) {
        sscanf(linha, "%d,%d,%d,%d,%d", &bdp->partidas[i].ID, &bdp->partidas[i].time1_id,
               &bdp->partidas[i].time2_id, &bdp->partidas[i].gols_time1, &bdp->partidas[i].gols_time2);
        i++;
    }

    bdp->num_partidas = i;
    
    fclose(partidas);

    return bdp;
}

// Libera a memória alocada para BDPartidas
void bdp_liberar(BDPartidas* bdp)
{
    if (bdp != NULL) {
        free(bdp);
    }
}

// Implementa a funcionalidade 2 - Consultar partidas
void bdp_consultar_partidas(BDPartidas* bdp, BDTimes* bdt)
{
    int escolha;
    do {
        printf(
            "\nEscolha o modo de consulta:\n"
            "1 - Por time mandante\n"
            "2 - Por time visitante\n"
            "3 - Por time mandante ou visitante\n"
            "4 - Retornar ao menu principal\n\n"
            "Escolha uma opção: "
        );
        
        scanf("%d", &escolha);

        if (escolha == 4) {
            break;
        }

        if (escolha < 1 || escolha > 4) {
            printf("Opção inválida. Tente novamente.\n");
            continue;
        }

        char prefixo[50];
        printf("\nDigite o nome ou prefixo do time: ");
        scanf("%49s", prefixo);

        int encontrado = 0;

        for (int i = 0; i < bdp->num_partidas; i++) {
            Partida* partida = &bdp->partidas[i];
            Time* time1 = bdt_busca_por_id(bdt, partida->time1_id);
            Time* time2 = bdt_busca_por_id(bdt, partida->time2_id);

            if (time1 == NULL || time2 == NULL) {
                continue; // Pula partidas com times não encontrados
            }

            int mandante = (strncmp(time1->nome, prefixo, strlen(prefixo)) == 0);
            int visitante = (strncmp(time2->nome, prefixo, strlen(prefixo)) == 0);
            
            // Decide se deve imprimir com base na escolha do usuário
            int deve_imprimir = 0;
            switch (escolha) {
                case 1:
                    if (mandante) deve_imprimir = 1;
                    break;
                case 2:
                    if (visitante) deve_imprimir = 1;
                    break;
                case 3:
                    if (mandante || visitante) deve_imprimir = 1;
                    break;
            }

            if (deve_imprimir) {
                if (!encontrado) {
                    printf("\n%-3s %-" TO_STRING(PADDING) "s %-10s %-" TO_STRING(PADDING) "s\n", "ID", "Time1", "", "Time2");
                }
                encontrado = 1;

                int largura_nome = largura_visual_utf8(time1->nome);
                int padding = PADDING - largura_nome;
                if (padding < 0) padding = 0;

                printf("%-3d %s%*s %2d  x %2d   %-" TO_STRING(PADDING) "s\n",
                    partida->ID,
                    time1->nome,
                    padding, "",
                    partida->gols_time1,
                    partida->gols_time2,
                    time2->nome);
            }
        }

        if (!encontrado) {
            printf("\nNenhuma partida encontrada para o time informado.\n");
        }

    } while (escolha != 4);
}