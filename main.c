#include <stdio.h>
#include "times.h"
#include "partidas.h"

int main(void)
{
    // Carrega os bancos de dados
    BDTimes* bdt = bdt_carregar("times.csv");
    BDPartidas* bdp = bdp_carregar("partidas_parcial.csv");

    // Verifica se os bancos de dados foram carregados corretamente
    if (bdt == NULL || bdp == NULL) {
        printf("Erro ao carregar os arquivos de banco de dados.\n");
        return 1;
    }

    // Processa as partidas para atualizar os dados dos times
    bdt_processar_partidas(bdt, bdp);

    // Loop principal do sistema
    char escolha;
    do {
        printf(
            "\nSistema de Gerenciamento de Partidas\n\n"
            "1 - Consultar time\n"
            "2 - Consultar partidas\n"
            "6 - Imprimir tabela de classificação\n"
            "Q - Sair\n\n"
            "Escolha uma opção: "
        );
        
        scanf(" %c", &escolha);

        switch (escolha) {
            case '1':
                bdt_consultar_time(bdt);
                break;
            case '2':
                bdp_consultar_partidas(bdp, bdt);
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

    // Libera a memória alocada para os bancos de dados
    bdt_liberar(bdt);
    bdp_liberar(bdp);
    
    return 0;
}