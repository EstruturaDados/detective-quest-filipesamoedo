#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * * PROJETO: Detective Quest - Mansão Enigma
 * * DESCRIÇÃO: Simulador de exploração de uma mansão misteriosa usando uma
 * estrutura de dados em árvore binária. O jogador navega por diferentes salas
 * (nós) e toma decisões que afetam o caminho da investigação.
 * * * REQUISITOS:
 * * 1. Implementar uma estrutura de árvore binária para representar a mansão.
 * * 2. Criar uma função de exploração interativa que permita ao jogador escolher
 * os caminhos (esquerda ou direita) a partir de cada sala.
 * * 3. Garantir que o programa lide corretamente com casos de salas sem saídas
 * (nós-folha) e permita ao jogador sair do jogo a qualquer momento.
 * * 4. Implementar uma função de limpeza de memória para evitar vazamentos.
 *  **/

// DEFINIÇÃO DAS ESTRUTURAS DE DADOS
/* Estrutura que representa uma Sala (Nó da Árvore Binária).
 * * Cada sala contém o seu nome e ponteiros para as duas próximas salas possíveis
 * (esquerda e direita). Se ambos os ponteiros forem NULL, a sala é um nó-folha.
 */
typedef struct Sala {
    char nome[50];          // Nome do cômodo da mansão
    struct Sala *esquerda;  // Ponteiro para a sala à esquerda
    struct Sala *direita;   // Ponteiro para a sala à direita
} Sala;

// PROTÓTIPOS DAS FUNÇÕES

Sala* criarSala(const char *nome);
void explorarMansao(Sala *raiz);
void liberarMemoria(Sala *raiz);

// FUNÇÃO PRINCIPAL (MAIN)

int main() {

    printf("==================================================\n");
    printf("         DETECTIVE QUEST: MANSÃO ENIGMA           \n");
    printf("==================================================\n\n");

    /* * MONTAGEM AUTOMÁTICA DA ÁRVORE DA MANSÃO
     * * Visualização da estrutura criada:
     *              [Hall de Entrada] -> Raiz da Árvore
     *               /             \
     *          [Biblioteca]       [Sala de Estar]
     *          /          \                   \
     *    [Escritório]    [Sótão]          [Cozinha]
     */
    
    // Criando os nós (salas) individualmente
    Sala *hall        = criarSala("Hall de Entrada");
    Sala *biblioteca  = criarSala("Biblioteca");
    Sala *salaEstar   = criarSala("Sala de Estar");
    Sala *escritorio  = criarSala("Escritório");
    Sala *sotao       = criarSala("Sótão");
    Sala *cozinha     = criarSala("Cozinha");

    // Conectando os nós para formar a árvore binária da mansão
    hall->esquerda = biblioteca;
    hall->direita  = salaEstar;

    biblioteca->esquerda = escritorio;
    biblioteca->direita  = sotao;

    salaEstar->esquerda = NULL; // Não há caminho à esquerda na Sala de Estar
    salaEstar->direita  = cozinha;

    // Inicia a exploração interativa a partir da raiz (Hall de Entrada)
    explorarMansao(hall);

    // Limpeza da memória alocada dinamicamente antes de fechar o programa
    liberarMemoria(hall);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}

// IMPLEMENTAÇÃO DAS FUNÇÕES

/**
 * @brief Cria e aloca dinamicamente uma nova sala.
 * * @param nome O nome que será atribuído à sala.
 * @return Sala* Ponteiro para a nova sala criada em memória.
 */
Sala* criarSala(const char *nome) { // Const para garantir que a string de entrada não seja modificada. * porque a função é responsável apenas por criar a sala, não por alterar o nome passado.
    // Aloca memória dinamicamente para uma estrutura Sala
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    
    // Verifica se a alocação de memória foi bem-sucedida
    if (novaSala == NULL) {
        printf("Erro crítico: Falha ao alocar memória para a sala '%s'.\n", nome);
        exit(1); 
    }
    
    // Copia o nome passado por parâmetro para o campo nome da struct
    strcpy(novaSala->nome, nome);
    
    // Inicializa os caminhos (filhos da árvore) como nulos
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * @brief Controla a navegação iterativa do jogador pela mansão.
 * * A função exibe onde o jogador está e pede para ele escolher o próximo passo
 * (Esquerda ou Direita). O ciclo termina quando o jogador escolhe sair ou 
 * quando alcança uma sala sem saídas (nó-folha).
 * * @param raiz Ponteiro para a sala onde a exploração começa.
 */
void explorarMansao(Sala *raiz) {
    // Se a árvore estiver vazia, encerra a função
    if (raiz == NULL) {
        printf("A mansão ainda não foi criada.\n");
        return;
    }

    Sala *salaAtual = raiz;
    char escolha;

    printf("Você entrou na mansão e começa sua investigação...\n\n");

    // Loop contínuo de exploração
    while (salaAtual != NULL) {
        printf("--------------------------------------------------\n");
        printf("Você está na sala:  %s \n", salaAtual->nome);

        // REQUISITO: Verificar se alcançou um cômodo sem saídas (Nó-Folha)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("\n[FIM DA LINHA] Você chegou a um beco sem saída! Não há mais caminhos a seguir.\n");
            printf("Investigação concluída neste setor da mansão: %s\n", salaAtual->nome);
            break; // Sai do loop de exploração
        }

        // Exibe os caminhos disponíveis dinamicamente
        printf("Caminhos disponíveis:\n");
        if (salaAtual->esquerda != NULL) {
            printf("  [e] Ir para a Esquerda (%s)\n", salaAtual->esquerda->nome);
        } else {
            printf("  [e] Caminho da Esquerda está bloqueado.\n");
        }

        if (salaAtual->direita != NULL) {
            printf("  [d] Ir para a Direita (%s)\n", salaAtual->direita->nome);
        } else {
            printf("  [d] Caminho da Direita está bloqueado.\n");
        }
        
        printf("  [s] Sair do jogo\n");
        printf("Escolha sua direção: ");
        
        // Lê o caractere digitado ignorando espaços e quebras de linha anteriores
        scanf(" %c", &escolha);

        // Processa a escolha do jogador
        if (escolha == 's' || escolha == 'S') {
            printf("\nVocê decidiu abandonar a investigação.\n");
            break;
        } 
        else if (escolha == 'e' || escolha == 'E') {
            if (salaAtual->esquerda != NULL) {
                salaAtual = salaAtual->esquerda; // Avança para o nó à esquerda
            } else {
                printf("\n[AVISO] Não há nada para o lado esquerdo! Escolha outro caminho.\n");
            }
        } 
        else if (escolha == 'd' || escolha == 'D') {
            if (salaAtual->direita != NULL) {
                salaAtual = salaAtual->direita; // Avança para o nó à direita
            } else {
                printf("\n[AVISO] Não há nada para o lado direito! Escolha outro caminho.\n");
            }
        } 
        else {
            printf("\n[OPÇÃO INVÁLIDA] Digite apenas 'e', 'd' ou 's'.\n");
        }
    }
}

/**
 * @brief Função RECURSIVA para liberar toda a memória alocada para a árvore.
 * * Passa por cada nó da árvore limpando os filhos antes de limpar o nó pai, 
 * evitando vazamento de memória (Memory Leak).
 * * @param raiz Ponteiro para a raiz da árvore (ou subárvore) a ser limpa.
 */
void liberarMemoria(Sala *raiz) {
    // Caso base da recursão: se o ponteiro for nulo, não há nada para liberar
    if (raiz == NULL) {
        return;
    }

    // Passo recursivo: Visita e limpa primeiro as subárvores filhas
    liberarMemoria(raiz->esquerda);
    liberarMemoria(raiz->direita);

    // Após garantir que os filhos foram limpos, libera o nó atual
    free(raiz);
}
