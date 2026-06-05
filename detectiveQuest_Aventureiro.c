#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PASSOS 100

typedef struct PistaNode {
    char textoPista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Protótipos
Sala* criarSala(const char *nome, const char *pista);
void explorarSalasComPistas(Sala *raizMansao, PistaNode **raizPistas);
void liberarMansao(Sala *raiz);
PistaNode* inserirPista(PistaNode *raizPistas, const char *novaPista);
void exibirPistas(PistaNode *raizPistas);
void liberarPistas(PistaNode *raizPistas);

int main() {
    printf("==================================================\n");
    printf("     DETECTIVE QUEST: EDICAO PROFISSIONAL         \n");
    printf("==================================================\n\n");

    PistaNode *inventarioPistas = NULL;

    // Cenário fixo solicitado pelo exercício
    Sala *hall        = criarSala("Hall de Entrada", "Pegadas lamacentas perto do tapete.");
    Sala *biblioteca  = criarSala("Biblioteca", "Um livro de venenos misteriosamente aberto.");
    Sala *salaEstar   = criarSala("Sala de Estar", "Um relógio de bolso que parou às 22:15.");
    Sala *escritorio  = criarSala("Escritório", "Testamento rasgado dentro da lixeira.");
    Sala *sotao       = criarSala("Sótão", "Corda com um nó de marinheiro suspeito.");
    Sala *cozinha     = criarSala("Cozinha", "Xícara com resíduo de pó branco no fundo.");

    hall->esquerda = biblioteca;
    hall->direita  = salaEstar;
    biblioteca->esquerda = escritorio;
    biblioteca->direita  = sotao;
    salaEstar->direita  = cozinha;

    explorarSalasComPistas(hall, &inventarioPistas);

    printf("\n==================================================\n");
    printf("             RELATÓRIO FINAL DO CASO              \n");
    printf("==================================================\n");
    printf("Pistas coletadas e organizadas (Ordem Alfabetica):\n");
    if(inventarioPistas == NULL) {
        printf(" Noenhuma pista foi coletada.\n");
    } else {
        exibirPistas(inventarioPistas);
    }
    printf("--------------------------------------------------\n");

    liberarMansao(hall);
    liberarPistas(inventarioPistas);
    return 0;
}

Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (!novaSala) exit(1);
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

void explorarSalasComPistas(Sala *raizMansao, PistaNode **raizPistas) {
    if (raizMansao == NULL) return;

    Sala *salaAtual = raizMansao;
    
    // Histórico (Pilha) para permitir a ação de voltar
    Sala *historico[MAX_PASSOS];
    int topo = 0; 

    char escolha;

    while (salaAtual != NULL) {
        printf("\n--------------------------------------------------\n");
        printf("Você está em: ** %s **\n", salaAtual->nome);

        // Coleta a pista se houver
        if (strlen(salaAtual->pista) > 0) {
            printf("[NOVA PISTA]: \"%s\" (Salva no inventário)\n", salaAtual->pista);
            *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
            salaAtual->pista[0] = '\0'; // Evita recoleta
        }

        // MELHORIA PROFISSIONAL: Se for nó-folha, acaba o jogo direto aqui
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("\n[FIM DA LINHA] Você chegou ao último cômodo deste setor. Não há saídas.\n");
            printf("Investigação dada como encerrada automaticamente.\n");
            break; 
        }

        // Exibe opções inteligentes (só mostra o que realmente existe)
        printf("\nOpções de movimentação:\n");
        if (salaAtual->esquerda != NULL) printf("  [e] Ir para a Esquerda (%s)\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL)  printf("  [d] Ir para a Direita (%s)\n", salaAtual->direita->nome);
        if (topo > 0)                    printf("  [v] Voltar para a sala anterior (%s)\n", historico[topo - 1]->nome);
        printf("  [s] Sair do jogo\n");
        printf("Escolha: ");
        
        scanf(" %c", &escolha);

        if (escolha == 's' || escolha == 'S') {
            break;
        } 
        else if ((escolha == 'e' || escolha == 'E') && salaAtual->esquerda != NULL) {
            historico[topo++] = salaAtual; // Guarda a atual no histórico antes de ir
            salaAtual = salaAtual->esquerda;
        } 
        else if ((escolha == 'd' || escolha == 'D') && salaAtual->direita != NULL) {
            historico[topo++] = salaAtual; // Guarda a atual no histórico antes de ir
            salaAtual = salaAtual->direita;
        } 
        else if ((escolha == 'v' || escolha == 'V') && topo > 0) {
            // Mecânica de voltar: pega o último elemento guardado na pilha
            salaAtual = historico[--topo];
            printf("\nVocê voltou uma sala...\n");
        } 
        else {
            printf("\n[Erro] Comando inválido ou caminho bloqueado!\n");
        }
    }
}

PistaNode* inserirPista(PistaNode *raizPistas, const char *novaPista) {
    if (raizPistas == NULL) {
        PistaNode *novoNo = (PistaNode*) malloc(sizeof(PistaNode));
        if (!novoNo) exit(1);
        strcpy(novoNo->textoPista, novaPista);
        novoNo->esquerda = NULL;
        novoNo->direita = NULL;
        return novoNo;
    }
    if (strcmp(novaPista, raizPistas->textoPista) < 0) {
        raizPistas->esquerda = inserirPista(raizPistas->esquerda, novaPista);
    } else if (strcmp(novaPista, raizPistas->textoPista) > 0) {
        raizPistas->direita = inserirPista(raizPistas->direita, novaPista);
    }
    return raizPistas;
}

void exibirPistas(PistaNode *raizPistas) {
    if (raizPistas == NULL) return;
    exibirPistas(raizPistas->esquerda);
    printf(" 🔍 %s\n", raizPistas->textoPista);
    exibirPistas(raizPistas->direita);
}

void liberarMansao(Sala *raiz) {
    if (raiz == NULL) return;
    liberarMansao(raiz->esquerda);
    liberarMansao(raiz->direita);
    free(raiz);
}

void liberarPistas(PistaNode *raizPistas) {
    if (raizPistas == NULL) return;
    liberarPistas(raizPistas->esquerda);
    liberarPistas(raizPistas->direita);
    free(raizPistas);
}
