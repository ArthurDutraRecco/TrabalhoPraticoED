#include <stdio.h>
#include <stdlib.h>
#include "times.h"
#include "partidas.h"

int main(void)
{
    BDTimes* bdt = bdt_carregar("bd_times.csv");
    BDPartidas* bdp = bdp_carregar("bd_partidas.csv"); 

    if (!bdt) {
        printf("Erro fatal: Falha ao carregar os arquivos de banco de dados.\n");
        return 1;
    }

    bdp_processar_tabela(bdp, bdt);

    char escolha;
    do {
        printf(
            "\nSistema de Gerenciamento de Partidas\n\n"
            "1 - Consultar time\n"
            "2 - Consultar partidas\n"
            "3 - Atualizar partida\n"
            "4 - Remover partida\n"
            "5 - Inserir partida\n"
            "6 - Imprimir tabela de classificação\n"
            "Q - Sair\n\n"
            "Escolha uma opção: "
        );
        
        scanf(" %c", &escolha);

        int houve_alteracao = 0;

        switch (escolha) {
            case '1':
                bdt_consultar_time(bdt);
                break;

            case '2':
                bdp_consultar_partidas(bdp, bdt);
                break;

            case '3':
                houve_alteracao = bdp_atualizar_partida(bdp, bdt);
                if (houve_alteracao) {
                    bdp_processar_tabela(bdp, bdt);
                }
                break;

            case '4':
                houve_alteracao = bdp_remover_partida(bdp, bdt);
                if (houve_alteracao) {
                    bdp_processar_tabela(bdp, bdt);
                }
                break;

            case '5':
                houve_alteracao = bdp_inserir_partida(bdp, bdt);
                if (houve_alteracao) {
                    bdp_processar_tabela(bdp, bdt);
                }
                break;

            case '6':
                bdt_imprimir_tabela(bdt);
                break;

            case 'Q':
            case 'q':
                printf("Encerrando o sistema...\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (escolha != 'Q' && escolha != 'q');

    bdt_liberar(bdt);
    bdp_liberar(bdp);
    
    return 0;
}