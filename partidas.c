#include "partidas.h"
#include "times.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Macro para definição/teste do tamanho do padding nas tabelas
#define PADDING 15
#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

// Define o TAD para uma partida
typedef struct partida
{
    int ID; // Identificador da partida
    int time1_id;  // ID do Time1
    int time2_id;  // ID do Time2
    int gols_time1; // Gols do Time1
    int gols_time2; // Gols do Time2
} Partida;

// Define o TAD para o nó da lista encadeada de partidas
typedef struct partida_node {
    Partida data;
    struct partida_node* next;
} PartidaNode;

// Define o TAD para o vetor de todas as partidas
struct bdpartidas
{
    PartidaNode* inicio;
    int num_partidas;
    int proximo_id;
};

static void bdp_inserir_no_inicio(BDPartidas* bdp, Partida partida) {
    PartidaNode* node = (PartidaNode*)malloc(sizeof(PartidaNode));
    if (!node) {
        printf("Erro de memória.\n");
        exit(1);
    }
    node->data = partida;
    node->next = bdp->inicio;
    bdp->inicio = node;
    bdp->num_partidas++;
}

// Aloca, carrega as partidas do CSV e retorna o ponteiro
BDPartidas* bdp_carregar(const char* arquivo_partidas)
{
    FILE* partidas = fopen(arquivo_partidas, "r");
    if (partidas == NULL) {
        BDPartidas* bdp = (BDPartidas*)malloc(sizeof(BDPartidas));
        bdp->inicio = NULL;
        bdp->num_partidas = 0;
        bdp->proximo_id = 0; 
        return bdp;
    }
    
    char linha[128];
    fgets(linha, sizeof(linha), partidas); // Descarta o cabeçalho

    BDPartidas* bdp = (BDPartidas*)malloc(sizeof(BDPartidas));
    if (bdp == NULL) {
        printf("Erro ao alocar memória para a estrutura BDPartidas.\n");
        fclose(partidas);
        return NULL;
    }
    bdp->inicio = NULL;
    bdp->num_partidas = 0;
    bdp->proximo_id = 0;

    while (fgets(linha, sizeof(linha), partidas)) {
        Partida p;
        sscanf(linha, "%d,%d,%d,%d,%d", &p.ID, &p.time1_id, &p.time2_id, &p.gols_time1, &p.gols_time2);
        
        bdp_inserir_no_inicio(bdp, p);

        if (p.ID >= bdp->proximo_id) {
            bdp->proximo_id = p.ID + 1;
        }
    }
    
    fclose(partidas);

    return bdp;
}

void bdp_processar_tabela(BDPartidas* bdp, BDTimes* bdt) {
    // 1. Zera estatísticas (função de times.h)
    bdt_zerar_estatisticas(bdt);

    // 2. Itera sobre partidas (só partidas.c sabe como fazer isso)
    PartidaNode* p = bdp->inicio;
    while (p) {
        Time* t1 = bdt_busca_por_id(bdt, p->data.time1_id);
        Time* t2 = bdt_busca_por_id(bdt, p->data.time2_id);

        if (t1 && t2) {
            // 3. Registra resultado (só times.c sabe como calcular pontos)
            time_registrar_resultado(t1, p->data.gols_time1, p->data.gols_time2);
            time_registrar_resultado(t2, p->data.gols_time2, p->data.gols_time1);
        }
        p = p->next;
    }
}

// Libera a memória alocada para BDPartidas
void bdp_liberar(BDPartidas* bdp) {
    if (bdp == NULL) return;
    PartidaNode* p = bdp->inicio;
    while (p != NULL) {
        PartidaNode* t = p->next;
        free(p);
        p = t;
    }
    free(bdp);
}

// Implementa a funcionalidade 2 - Consultar partidas
void bdp_consultar_partidas(BDPartidas* bdp, BDTimes* bdt)
{
    int escolha;
    printf(
        "\nEscolha o modo de consulta:\n"
        "1 - Por time mandante\n"
        "2 - Por time visitante\n"
        "3 - Por time mandante ou visitante\n"
        "4 - Retornar ao menu principal\n\n"
        "Escolha uma opção: "
    );
    
    scanf("%d", &escolha);

    if (escolha == 4) return;

    char prefixo[50];
    printf("\nDigite o nome ou prefixo do time: ");
    scanf("%49s", prefixo);

    int encontrado = 0;

    PartidaNode* p = bdp->inicio;
    while (p != NULL) {
        Time* t1 = bdt_busca_por_id(bdt, p->data.time1_id);
        Time* t2 = bdt_busca_por_id(bdt, p->data.time2_id);

        if (t1 && t2) {
            char* n1 = time_get_nome(t1);
            char* n2 = time_get_nome(t2);
            
            int match = 0;
            if (escolha == 1 && strncmp(n1, prefixo, strlen(prefixo)) == 0) match = 1;
            else if (escolha == 2 && strncmp(n2, prefixo, strlen(prefixo)) == 0) match = 1;
            else if (escolha == 3 && (strncmp(n1, prefixo, strlen(prefixo)) == 0 || strncmp(n2, prefixo, strlen(prefixo)) == 0)) match = 1;

            if (match) {
                int pad = PADDING - largura_visual_utf8(n1);
                if (pad < 0) pad = 0;
                printf("%-3d %s%*s %d x %d      %s\n", p->data.ID, n1, pad, "", p->data.gols_time1, p->data.gols_time2, n2);
                encontrado = 1;
            }
        }
        p = p->next;
    }
    if (!encontrado) printf("\nNenhuma partida encontrada para o time informado.\n");
}

// Implementa a funcionalidade 5 - Inserir partida
int bdp_inserir_partida(BDPartidas* bdp, BDTimes* bdt) {
    int id1, id2, g1, g2;
    
    printf("\nPara inserir um novo registro, digite os valores para os campos Time1, Time2, Placar1 e Placar2:\n");

    scanf("%d", &id1);
    scanf("%d", &id2);
    scanf("%d", &g1);
    scanf("%d", &g2);

    Time* t1 = bdt_busca_por_id(bdt, id1);
    Time* t2 = bdt_busca_por_id(bdt, id2);

    if (t1 == NULL || t2 == NULL) {
        printf("\nErro: ID de time invalido informado. Operacao cancelada.\n");
        return 0;
    }
    if (id1 == id2) {
        printf("\nErro: Um time nao pode jogar contra si mesmo. Operacao cancelada.\n");
        return 0;
    }

    int novo_id = bdp->proximo_id;

    printf("\nConfirma a inserção do registro abaixo? (S/N)\n\n");
    
    printf("%-3s %-" TO_STRING(PADDING) "s %-" TO_STRING(PADDING) "s %-8s %-8s\n", 
           "ID", "Time1", "Time2", "Placar1", "Placar2");

    char* n1 = time_get_nome(t1);
    char* n2 = time_get_nome(t2);
    int pad1 = PADDING - largura_visual_utf8(n1); if (pad1 < 0) pad1 = 0;
    int pad2 = PADDING - largura_visual_utf8(n2); if (pad2 < 0) pad2 = 0;

    printf("%-3d %s%*s %s%*s %-8d %-8d\n",
           novo_id,
           n1, pad1, "", 
           n2, pad2, "",
           g1, g2);

    char resp; scanf(" %c", &resp);
    if (resp == 'S' || resp == 's') {
        struct partida nova = {bdp->proximo_id++, id1, id2, g1, g2};
        bdp_inserir_no_inicio(bdp, nova);

        printf("\nO registro foi inserido com sucesso.\n");
        return 1;
    }
    
    printf("\nOperação cancelada.\n");
    return 0;
}

// Implementa a funcionalidade 4 - Remover partida
int bdp_remover_partida(BDPartidas* bdp, BDTimes* bdt) {
    bdp_consultar_partidas(bdp, bdt);

    int id_alvo;
    printf("\nDigite o ID do registro a ser removido: ");
    
    if (scanf("%d", &id_alvo) != 1) {
        while (getchar() != '\n');
        printf("Erro: Entrada invalida.\n");
        return 0;
    }

    PartidaNode* atual = bdp->inicio;
    PartidaNode* anterior = NULL;

    while (atual != NULL && atual->data.ID != id_alvo) {
        anterior = atual;
        atual = atual->next;
    }

    if (atual == NULL) {
        printf("Erro: A partida com o ID %d nao foi encontrada.\n", id_alvo);
        return 0;
    }

    Time* t1 = bdt_busca_por_id(bdt, atual->data.time1_id);
    Time* t2 = bdt_busca_por_id(bdt, atual->data.time2_id);
    
    if (t1 == NULL || t2 == NULL) {
        printf("Erro critico: Dados inconsistentes. Times nao encontrados.\n");
        return 0;
    }
    
    printf("\nTem certeza de que deseja excluir o registro abaixo? (S/N)\n");
    
    printf("%-3s %-" TO_STRING(PADDING) "s %-" TO_STRING(PADDING) "s %-8s %-8s\n", 
           "ID", "Time1", "Time2", "Placar1", "Placar2");
    
    char* n1 = time_get_nome(t1);
    char* n2 = time_get_nome(t2);
    int pad1 = PADDING - largura_visual_utf8(n1); if (pad1 < 0) pad1 = 0;
    int pad2 = PADDING - largura_visual_utf8(n2); if (pad2 < 0) pad2 = 0;

    printf("%-3d %s%*s %s%*s %-8d %-8d\n",
           id_alvo,
           n1, pad1, "", 
           n2, pad2, "",
           atual->data.gols_time1, atual->data.gols_time2);


    char resp; scanf(" %c", &resp);
    if (resp == 'S' || resp == 's') {
        if (!anterior) bdp->inicio = atual->next;
        else anterior->next = atual->next;
        free(atual);
        bdp->num_partidas--;
        printf("Registro removido com sucesso.\n");
        return 1;
    }
    printf("Remoção cancelada.\n");
    return 0;
}

// Implementa a funcionalidade 3 - Atualizar partida
int bdp_atualizar_partida(BDPartidas* bdp, BDTimes* bdt) {
    bdp_consultar_partidas(bdp, bdt);

    int id_alvo;
    printf("\nDigite o ID do registro a ser atualizado: ");
    if (scanf("%d", &id_alvo) != 1) {
        while (getchar() != '\n');
        printf("Erro: Entrada inválida.\n");
        return 0;
    }

    PartidaNode* p = bdp->inicio;
    while (p != NULL && p->data.ID != id_alvo) {
        p = p->next;
    }

    if (p == NULL) {
        printf("Erro: Partida com ID %d não encontrada.\n", id_alvo);
        return 0;
    }

    Time* t1 = bdt_busca_por_id(bdt, p->data.time1_id);
    Time* t2 = bdt_busca_por_id(bdt, p->data.time2_id);

    if (t1 == NULL || t2 == NULL) {
        printf("Erro: Times da partida não encontrados.\n");
        return 0;
    }

    char buf1[10], buf2[10];
    
    printf("\nDigite o novo valor para os campos Placar1 e Placar2 (para manter o valor atual de um campo, digite '-'):\n");
    
    scanf("%s", buf1);
    scanf("%s", buf2);

    int novo_g1 = (buf1[0] == '-') ? p->data.gols_time1 : atoi(buf1);
    int novo_g2 = (buf2[0] == '-') ? p->data.gols_time2 : atoi(buf2);

    printf("\nConfirma os novos valores para o registro abaixo? (S/N)\n\n");

    printf("%-3s %-" TO_STRING(PADDING) "s %-" TO_STRING(PADDING) "s %-8s %-8s\n", 
           "ID", "Time1", "Time2", "Placar1", "Placar2");

    char* n1 = time_get_nome(t1);
    char* n2 = time_get_nome(t2);
    int pad1 = PADDING - largura_visual_utf8(n1); if (pad1<0) pad1=0;
    int pad2 = PADDING - largura_visual_utf8(n2); if (pad2<0) pad2=0;

    printf("%-3d %s%*s %s%*s %-8d %-8d\n",
           id_alvo,
           n1, pad1, "", 
           n2, pad2, "",
           novo_g1, novo_g2);

    char resp;
    scanf(" %c", &resp);

    if (resp == 'S' || resp == 's') {
        p->data.gols_time1 = novo_g1;
        p->data.gols_time2 = novo_g2;
        
        printf("\nRegistro atualizado com sucesso.\n");
        return 1;
    }
    
    printf("\nOperação cancelada.\n");
    return 0;
}