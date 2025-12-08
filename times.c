#include "times.h"
#include "partidas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macro para definição/teste do tamanho do padding nas tabelas
#define PADDING 15
#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

// Define o TAD para um time
struct time
{
    int ID; // Identificador do time
    char nome[50];
    int V; // Vitórias
    int E; // Empates
    int D; // Derrotas
    int GM; // Gols marcados
    int GS; // Gols sofridos
};

// Define o TAD para o nó da lista encadenada de times
typedef struct time_node {
    Time data;
    struct time_node* next;
} TimeNode;

// Define o TAD para o vetor de todos os times
struct bdtimes
{
    TimeNode* inicio;
    int num_times;
};

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

int time_get_id(Time* t) { return t->ID; }
char* time_get_nome(Time* t) { return t->nome; }
int time_get_pontos(Time* t) { return pontos_ganhos(t); }

void time_registrar_resultado(Time* t, int gols_pro, int gols_contra) {
    t->GM += gols_pro;
    t->GS += gols_contra;
    
    if (gols_pro > gols_contra) {
        t->V++;
    } else if (gols_pro < gols_contra) {
        t->D++;
    } else {
        t->E++;
    }
}

// Função interna para inserir no início da lista encadeada
static void bdt_inserir_no_inicio(BDTimes* bdt, Time dados_time) {
    TimeNode* node = (TimeNode*)malloc(sizeof(TimeNode));
    if (node == NULL) {
        printf("Erro ao alocar memória para um nó da LinkedList Times.\n");
        exit(1);
    }
    node->data = dados_time;
    node->next = bdt->inicio;
    bdt->inicio = node;
    bdt->num_times++;
}

// Aloca, carrega os times do CSV e retorna o ponteiro
BDTimes* bdt_carregar(const char* arquivo_times)
{
    FILE* times = fopen(arquivo_times, "r");
    if (times == NULL) {
        printf("Erro ao abrir o arquivo '%s'.\n", arquivo_times);
        return NULL;
    }
    
    BDTimes *bdt = (BDTimes*)malloc(sizeof(BDTimes));
    if (bdt == NULL) {
        printf("Erro ao alocar memória para a estrutura BDTimes.\n");
        fclose(times);
        return NULL;
    }
    bdt->inicio = NULL;
    bdt->num_times = 0;
    
    char linha[128];
    fgets(linha, sizeof(linha), times); // Descarta o cabeçalho    
    
    while (fgets(linha, sizeof(linha), times)) {
        Time t;
        t.V = 0; t.E = 0; t.D = 0; t.GM = 0; t.GS = 0;
        
        sscanf(linha, "%d,%49[^\n]", &t.ID, t.nome);

        bdt_inserir_no_inicio(bdt, t);
    }
    
    fclose(times);

    return bdt;
}

// Libera a memória alocada para BDTimes
void bdt_liberar(BDTimes* bdt) {
    if (bdt == NULL) return;

    TimeNode* p = bdt->inicio;
    while (p != NULL) {
        TimeNode* t = p->next;
        free(p);
        p = t;
    }
    free(bdt);
}

// Busca um time pelo ID
Time* bdt_busca_por_id(BDTimes* bdt, int id) {
    TimeNode* p = bdt->inicio;
    while (p != NULL) {
        if (p->data.ID == id) {
            return &p->data;
        }
        p = p->next;
    }
    return NULL;
}

// Zera as estatísticas de todos os times
void bdt_zerar_estatisticas(BDTimes* bdt) {
    TimeNode* p = bdt->inicio;  
    while (p != NULL) {
        p->data.V = 0;
        p->data.E = 0;
        p->data.D = 0;
        p->data.GM = 0;
        p->data.GS = 0;
        p = p->next;
    }
}

// Implementa a funcionalidade 1 - Consultar time
void bdt_consultar_time(BDTimes* bdt) {
    char prefixo[50];
    printf("\nDigite o nome ou prefixo do time: ");
    scanf(" %49[^\n]", prefixo);

    int encontrado = 0;
    TimeNode* p = bdt->inicio;

    while (p != NULL) {
        if (strncmp(p->data.nome, prefixo, strlen(prefixo)) == 0) {
            if (!encontrado) {
                printf("\n%-3s %-" TO_STRING(PADDING) "s %-3s %-3s %-3s %-3s %-3s %-3s %-3s\n",
                       "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");
                encontrado = 1;
            }

            Time* t = &p->data;
            int s = saldo_gols(t);
            int pg = pontos_ganhos(t);
            
            int largura_nome = largura_visual_utf8(t->nome);
            int padding = PADDING - largura_nome;
            if (padding < 0) padding = 0;

            printf("%-3d %s%*s %-3d %-3d %-3d %-3d %-3d %-3d %-3d\n",
                t->ID, t->nome, padding, "", t->V, t->E, t->D, t->GM, t->GS, s, pg);
        }
        p = p->next;
    }

    if (!encontrado) {
        printf("\nNenhum time encontrado com o prefixo \"%s\".\n", prefixo);
    }
}


// Ordena a lista de times de acordo com a classificação do campeonato.
static void bdt_ordenar_classificacao(BDTimes* bdt) {
    if (bdt->inicio == NULL) return;

    int trocou;
    TimeNode *ptr1;
    TimeNode *lptr = NULL;

    do {
        trocou = 0;
        ptr1 = bdt->inicio;

        while (ptr1->next != lptr) {
            Time *t1 = &ptr1->data;
            Time *t2 = &ptr1->next->data;

            // Critérios de Classificação:
            // 1. Pontos Ganhos (Decrescente)
            // 2. Vitórias (Decrescente)
            // 3. Saldo de Gols (Decrescente)
            
            int pg1 = pontos_ganhos(t1);
            int pg2 = pontos_ganhos(t2);
            int v1 = t1->V;
            int v2 = t2->V;
            int s1 = saldo_gols(t1);
            int s2 = saldo_gols(t2);

            int deve_trocar = 0;

            if (pg1 < pg2) {
                deve_trocar = 1;
            } else if (pg1 == pg2) {
                if (v1 < v2) {
                    deve_trocar = 1;
                } else if (v1 == v2) {
                    if (s1 < s2) {
                        deve_trocar = 1;
                    }
                }
            }

            if (deve_trocar) {
                Time temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                trocou = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (trocou);
}

// Implementa a funcionalidade 6 - Imprimir tabela de classificação
void bdt_imprimir_tabela(BDTimes* bdt) {
    bdt_ordenar_classificacao(bdt);

    printf("\nImprimindo classificação (ordenada)...\n");

    printf("\n%-3s %-" TO_STRING(PADDING) "s %-3s %-3s %-3s %-3s %-3s %-3s %-3s\n",
           "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");

    TimeNode* p = bdt->inicio;
    while (p != NULL) {
        Time* t = &p->data;
        int s = saldo_gols(t);
        int pg = pontos_ganhos(t);

        int largura_nome = largura_visual_utf8(t->nome);
        int padding = PADDING - largura_nome;
        if (padding < 0) padding = 0;

        printf("%-3d %s%*s %-3d %-3d %-3d %-3d %-3d %-3d %-3d\n",
               t->ID, t->nome, padding, "", t->V, t->E, t->D, t->GM, t->GS, s, pg);
        
        p = p->next;
    }
}