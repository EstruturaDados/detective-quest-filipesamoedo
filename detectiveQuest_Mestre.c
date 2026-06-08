#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Necessária para usar as funções toupper() e tolower()

#define MAX_PASSOS 100
#define TAMANHO_HASH 7

/*******************************************************************************
 * ESTRUTURAS DE DADOS (STRUCTS)
 ******************************************************************************/

/**
 * @brief Estrutura para os nós da Árvore Binária de Busca (BST) de Pistas.
 */
typedef struct PistaNode {
    char textoPista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/**
 * @brief Estrutura que representa uma Sala da Mansão (Árvore Binária).
 */
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/**
 * @brief Estrutura para os nós da Tabela Hash (Lista Encadeada para colisões).
 * Mapeia uma pista (Chave) a um suspeito (Valor).
 */
typedef struct HashNode {
    char chavePista[100];
    char valorSuspeito[50];
    struct HashNode *proximo; // Próximo nó em caso de colisão na mesma posição
} HashNode;

/*******************************************************************************
 * PROTÓTIPOS DAS FUNÇÕES
 ******************************************************************************/

// Gerenciamento da Mansão
Sala* criarSala(const char *nome, const char *pista);
void explorarSalas(Sala *raizMansao, PistaNode **raizPistas);
void liberarMansao(Sala *raiz);

// Gerenciamento da BST de Pistas
PistaNode* inserirPista(PistaNode *raizPistas, const char *novaPista);
void exibirPistas(PistaNode *raizPistas);
void liberarPistas(PistaNode *raizPistas);

// Gerenciamento da Tabela Hash
int calcularHash(const char *chave);
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito);
const char* encontrarSuspeito(HashNode *tabela[], const char *pista);
void liberarTabelaHash(HashNode *tabela[]);

// Lógica de Julgamento Final
int contarPistasDoSuspeito(PistaNode *raizPistas, HashNode *tabela[], const char *suspeitoAlvo);
void verificarSuspeitoFinal(PistaNode *raizPistas, HashNode *tabela[]);
void padronizarNome(char *str); // Função para padronizar o nome do suspeito

/*******************************************************************************
 * FUNÇÃO PRINCIPAL (MAIN)
 ******************************************************************************/

int main() {
    printf("==================================================\n");
    printf("        DETECTIVE QUEST: NÍVEL MESTRE             \n");
    printf("==================================================\n\n");

    // 1. Inicializa o Inventário de Pistas (BST)
    PistaNode *inventarioPistas = NULL;

    // 2. Inicializa a Tabela Hash de Suspeitos (Mural de Investigação)
    HashNode *muralSuspeitos[TAMANHO_HASH];
    for (int i = 0; i < TAMANHO_HASH; i++) {
        muralSuspeitos[i] = NULL;
    }

    // 3. Cadastra as pistas e seus respectivos suspeitos na Tabela Hash
    inserirNaHash(muralSuspeitos, "Pegadas lamacentas perto do tapete.", "Mordomo");
    inserirNaHash(muralSuspeitos, "Um livro de venenos misteriosamente aberto.", "Cozinheira");
    inserirNaHash(muralSuspeitos, "Um relógio de bolso que parou às 22:15.", "Mordomo");
    inserirNaHash(muralSuspeitos, "Testamento rasgado dentro da lixeira.", "Sobrinho");
    inserirNaHash(muralSuspeitos, "Corda com um nó de marinheiro suspeito.", "Sobrinho");
    inserirNaHash(muralSuspeitos, "Xícara com resíduo de pó branco no fundo.", "Cozinheira");

    // 4. Montagem da Árvore Binária da Mansão (Cenário Fixo)
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

    // 5. Fluxo de Jogo: Exploração
    explorarSalas(hall, &inventarioPistas);

    // 6. Fluxo de Jogo: O Julgamento
    verificarSuspeitoFinal(inventarioPistas, muralSuspeitos);

    // 7. Limpeza Absoluta de Memória
    liberarMansao(hall);
    liberarPistas(inventarioPistas);
    liberarTabelaHash(muralSuspeitos);

    printf("\nObrigado por jogar Detective Quest: Modo Mestre!\n");
    return 0;
}

/*******************************************************************************
 * IMPLEMENTAÇÃO: GERENCIAMENTO DA MANSÃO
 ******************************************************************************/

/**
 * @brief Cria dinamicamente um cômodo alocando memória para a struct Sala.
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (!novaSala) exit(1);
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/**
 * @brief Controla a navegação ativa o sistema de pistas com botão voltar e fim automático.
 */
void explorarSalas(Sala *raizMansao, PistaNode **raizPistas) {
    if (raizMansao == NULL) return;

    Sala *salaAtual = raizMansao;
    Sala *historico[MAX_PASSOS];
    int topo = 0;
    char escolha;

    while (salaAtual != NULL) {
        printf("\n--------------------------------------------------\n");
        printf("Você está investigando: ** %s **\n", salaAtual->nome);

        // Se houver pista na sala, coleta e insere na árvore BST
        if (strlen(salaAtual->pista) > 0) {
            printf("🔍 [PISTA ENCONTRADA]: \"%s\"\n", salaAtual->pista);
            printf("   Evidência catalogada na sua árvore de buscas.\n");
            *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
            salaAtual->pista[0] = '\0'; // Coletada! Limpa a sala.
        }

        // Se for um nó-folha, encerra automaticamente
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("\n[FIM DA LINHA] Não há mais saídas neste setor da mansão.\n");
            break;
        }

        // Menu inteligente
        printf("\nDireções possíveis:\n");
        if (salaAtual->esquerda != NULL) printf("  [e] Esquerda (%s)\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL)  printf("  [d] Direita (%s)\n", salaAtual->direita->nome);
        if (topo > 0)                    printf("  [v] Voltar (%s)\n", historico[topo - 1]->nome);
        printf("  [s] Encerrar investigação e ir para o Tribunal\n");
        printf("Escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 's' || escolha == 'S') {
            break;
        }
        else if ((escolha == 'e' || escolha == 'E') && salaAtual->esquerda != NULL) {
            historico[topo++] = salaAtual;
            salaAtual = salaAtual->esquerda;
        }
        else if ((escolha == 'd' || escolha == 'D') && salaAtual->direita != NULL) {
            historico[topo++] = salaAtual;
            salaAtual = salaAtual->direita;
        }
        else if ((escolha == 'v' || escolha == 'V') && topo > 0) {
            salaAtual = historico[--topo];
            printf("\nRetornando pelos corredores...\n");
        }
        else {
            printf("\n[Aviso] Comando inválido ou rota bloqueada!\n");
        }
    }
}

void liberarMansao(Sala *raiz) {
    if (raiz == NULL) return;
    liberarMansao(raiz->esquerda);
    liberarMansao(raiz->direita);
    free(raiz);
}

/*******************************************************************************
 * IMPLEMENTAÇÃO: ÁRVORE BINÁRIA DE BUSCA (BST) DE PISTAS
 ******************************************************************************/

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
    printf(" 📌 %s\n", raizPistas->textoPista);
    exibirPistas(raizPistas->direita);
}

void liberarPistas(PistaNode *raizPistas) {
    if (raizPistas == NULL) return;
    liberarPistas(raizPistas->esquerda);
    liberarPistas(raizPistas->direita);
    free(raizPistas);
}

/*******************************************************************************
 * IMPLEMENTAÇÃO: TABELA HASH (MURAL DE ASSOCIAÇÃO PISTA -> SUSPEITO)
 ******************************************************************************/

/**
 * @brief Função de espalhamento simples (Soma dos caracteres ASCII mod TAMANHO).
 */
int calcularHash(const char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAMANHO_HASH;
}

/**
 * @brief Insere a associação pista/suspeito na tabela hash tratando colisões por encadeamento.
 */
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito) {
    int indice = calcularHash(pista);

    HashNode *novoNo = (HashNode*) malloc(sizeof(HashNode));
    if (!novoNo) exit(1);
    
    strcpy(novoNo->chavePista, pista);
    strcpy(novoNo->valorSuspeito, suspeito);
    
    // Insere no início da lista encadeada daquela posição (encadeamento)
    novoNo->proximo = tabela[indice];
    tabela[indice] = novoNo;
}

/**
 * @brief Consulta a tabela hash para encontrar o suspeito correspondente a uma pista.
 */
const char* encontrarSuspeito(HashNode *tabela[], const char *pista) {
    int indice = calcularHash(pista);
    HashNode *atual = tabela[indice];

    // Varre a lista encadeada daquela posição da tabela em busca da chave exata
    while (atual != NULL) {
        if (strcmp(atual->chavePista, pista) == 0) {
            return atual->valorSuspeito;
        }
        atual = atual->proximo;
    }
    return "Desconhecido";
}

void liberarTabelaHash(HashNode *tabela[]) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashNode *atual = tabela[i];
        while (atual != NULL) {
            HashNode *aux = atual;
            atual = atual->proximo;
            free(aux);
        }
    }
}

/*******************************************************************************
 * IMPLEMENTAÇÃO: LÓGICA DE JULGAMENTO FINAL (RECURSÃO + HASH)
 ******************************************************************************/

/**
 * @brief Função recursiva que percorre a BST de pistas coletadas e usa a Tabela Hash
 * para contar quantas dessas pistas acusam o suspeito alvo.
 */
int contarPistasDoSuspeito(PistaNode *raizPistas, HashNode *tabela[], const char *suspeitoAlvo) {
    if (raizPistas == NULL) return 0;

    int contagemAtual = 0;
    
    // Pergunta para a tabela Hash: "Quem cometeu o crime baseado nesta pista?"
    const char* suspeitoVinculado = encontrarSuspeito(tabela, raizPistas->textoPista);
    
    if (strcmp(suspeitoVinculado, suspeitoAlvo) == 0) {
        contagemAtual = 1;
    }

    // Soma o resultado do nó atual com os resultados das subárvores esquerda e direita
    return contagemAtual + 
           contarPistasDoSuspeito(raizPistas->esquerda, tabela, suspeitoAlvo) + 
           contarPistasDoSuspeito(raizPistas->direita, tabela, suspeitoAlvo);
}

/**
 * @brief Conduz a fase de julgamento final colhendo a denúncia e avaliando provas.
 */
void verificarSuspeitoFinal(PistaNode *raizPistas, HashNode *tabela[]) {
    char acusacao[50];

    printf("\n==================================================\n");
    printf("             TRIBUNAL DE DETECTIVE QUEST          \n");
    printf("==================================================\n");
    printf("Suas pistas coletadas em ordem alfabética:\n");
    
    if (raizPistas == NULL) {
        printf(" Nenhuma evidência coletada! Você veio de mãos vazias.\n");
    } else {
        exibirPistas(raizPistas);
    }
    printf("--------------------------------------------------\n");
    
    // Lista os suspeitos válidos cadastrados no sistema para orientar o jogador
    printf("Lista de Suspeitos na Mansão: [Mordomo] [Cozinheira] [Sobrinho]\n");
    printf("Quem você acusa formalmente pelo crime? ");
    
    // Limpa o buffer e lê o nome digitado
    scanf(" %49s", acusacao);

    // NOVA LINHA: Transforma o que o usuário digitou no padrão do sistema
    padronizarNome(acusacao);

    // REQUISITO: Avaliar recursivamente se há ao menos 2 pistas contra o acusado
    int totalProvas = contarPistasDoSuspeito(raizPistas, tabela, acusacao);

    printf("\n[RESULTADO DO PROCESSO]:\n");
    if (totalProvas >= 2) {
        printf(" Análise Concluída: Você encontrou %d provas incontestáveis!\n", totalProvas);
        printf(" CULPADO CONDENADO! O %s foi preso graças ao seu brilhante trabalho lógico.\n", acusacao);
        printf(" CASO ENCERRADO COM SUCESSO! 🏆\n");
    } else {
        printf(" Análise Concluída: Você apresentou apenas %d prova(s) contra o %s.\n", totalProvas, acusacao);
        printf(" CASO ARQUIVADO! Provas insuficientes. O culpado escapou por falta de indícios.\n");
        printf(" FIM DE JOGO (Tente explorar mais salas da próxima vez!). ❌\n");
    }
    printf("==================================================\n");
}

/**
 * @brief Padroniza uma string para o formato Capitalizado (Ex: "cozinheira" -> "Cozinheira")
 * * @param str A string que será modificada diretamente na memória.
 */
void padronizarNome(char *str) {
    if (str[0] != '\0') {
        // Força a primeira letra a ser MAIÚSCULA
        str[0] = toupper(str[0]); 
        
        // Força o restante das letras a serem MINÚSCULAS
        for (int i = 1; str[i] != '\0'; i++) {
            str[i] = tolower(str[i]);
        }
    }
}
