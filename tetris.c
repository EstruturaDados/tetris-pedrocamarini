#include <stdio.h>
#include <stdlib.h> // Para system("cls") ou system("clear")
#include <string.h>

// --- Constantes e Estruturas ---

// A fila terá um tamanho fixo de 5, como no vídeo
#define MAX_PECAS 5

// 1. Estrutura (Struct) para a Peca
typedef struct {
    char tipo; // Tipo da peça (I, O, T, L)
    int id;    // ID único da peça
} Peca;

// --- "Banco de Dados" Global da Fila ---

Peca filaPecas[MAX_PECAS]; // O vetor que armazena nossa fila
int inicio = 0;      // Índice de quem é o PRIMEIRO da fila
int fim = 0;         // Índice de onde o PRÓXIMO vai entrar
int totalPecas = 0;  // Contador de quantas peças temos
int proximoId = 0;   // Contador para garantir IDs únicos

// --- Protótipos das Funções ---

void inicializarFila();
void visualizarFila();
void jogarPeca();       // Dequeue (remover do início)
void inserirNovaPeca(); // Enqueue (adicionar ao fim)
void pausarTela();

// --- Função Principal (main) ---

int main() {
    int opcao;

    // 1. Inicializa a fila com peças automáticas
    inicializarFila();

    // 2. Menu interativo com do-while
    do {
        // Limpa a tela
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("======================================\n");
        printf("       TETRIS STACK - FILA DE PECAS\n");
        printf("======================================\n\n");
        
        // 3. Mostra o estado da fila (requisito do vídeo)
        visualizarFila();
        
        printf("\n--- MENU ---\n");
        printf("1. Jogar Peca (usar a proxima peca)\n");
        printf("2. Inserir Nova Peca (no fim da fila)\n");
        printf("0. Sair\n\n");
        printf("Opcao: ");
        
        scanf("%d", &opcao);
        while (getchar() != '\n'); // Limpa o buffer

        switch (opcao) {
            case 1:
                jogarPeca();
                break;
            case 2:
                inserirNovaPeca();
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }

        if (opcao != 0) {
            pausarTela();
        }

    } while (opcao != 0);

    return 0;
}

// --- Implementação das Funções ---

/**
 * @brief Preenche a fila com 5 peças iniciais automaticamente.
 */
void inicializarFila() {
    // Usando os tipos de peças do slide "Saída esperada"
    char tiposIniciais[MAX_PECAS] = {'T', 'O', 'L', 'I', 'I'}; 

    for (int i = 0; i < MAX_PECAS; i++) {
        // Adiciona a peça na posição 'fim'
        filaPecas[fim].tipo = tiposIniciais[i];
        filaPecas[fim].id = proximoId;

        // Atualiza os ponteiros da fila
        // (fim + 1) % MAX_PECAS faz a fila dar a volta (circular)
        fim = (fim + 1) % MAX_PECAS; 
        totalPecas++;
        proximoId++;
    }
}

/**
 * @brief Exibe o estado atual da fila de peças.
 * Formato: Fila de Pecas: [T 0] [O 1] ...
 */
void visualizarFila() {
    printf("Fila de Pecas (%d/%d):\n", totalPecas, MAX_PECAS);

    if (totalPecas == 0) {
        printf("[A fila esta vazia!]\n");
        return;
    }

    // Loop para imprimir uma fila circular:
    // Começa no 'inicio' e imprimi 'totalPecas'
    for (int i = 0; i < totalPecas; i++) {
        // O (inicio + i) % MAX_PECAS calcula a posição correta
        int indiceAtual = (inicio + i) % MAX_PECAS;
        
        // Imprime a peça no formato [Tipo ID]
        printf("[%c %d] ", 
               filaPecas[indiceAtual].tipo, 
               filaPecas[indiceAtual].id);
    }
    printf("\n"); // Quebra de linha no final
}

/**
 * @brief Remove a peça da frente da fila (Dequeue).
 */
void jogarPeca() {
    printf("\n--- Jogar Peca ---\n");

    // 1. Verifica se a fila está vazia
    if (totalPecas == 0) {
        printf("A fila de pecas esta vazia! Nao ha pecas para jogar.\n");
        return; // Retorna ao menu
    }

    // 2. Pega a peça do início da fila (para mostrar a mensagem)
    Peca pecaJogada = filaPecas[inicio];

    printf("Peca [%c %d] foi jogada (removida da frente da fila).\n", 
           pecaJogada.tipo, 
           pecaJogada.id);

    // 3. Atualiza o ponteiro de 'inicio'
    // (inicio + 1) % MAX_PECAS faz o ponteiro "dar a volta"
    inicio = (inicio + 1) % MAX_PECAS;

    // 4. Diminui o total de peças
    totalPecas--;
}

/**
 * @brief Adiciona uma nova peça no final da fila (Enqueue).
 */
void inserirNovaPeca() {
    printf("\n--- Inserir Nova Peca ---\n");

    // 1. Verifica se a fila está cheia
    if (totalPecas >= MAX_PECAS) {
        printf("A fila de pecas esta cheia! Jogue uma peca antes de inserir outra.\n");
        return; // Retorna ao menu
    }

    char tipoPeca;
    printf("Digite o tipo da nova peca (I, O, T, L): ");
    scanf(" %c", &tipoPeca); // O espaço antes do %c ignora "Enter"
    while (getchar() != '\n'); // Limpa o buffer

    // 2. Adiciona a nova peça na posição 'fim'
    filaPecas[fim].tipo = tipoPeca;
    filaPecas[fim].id = proximoId; // Usa o contador global de IDs

    printf("\nPeca [%c %d] inserida no fim da fila.\n", 
           filaPecas[fim].tipo, 
           filaPecas[fim].id);

    // 3. Atualiza os ponteiros e contadores
    proximoId++; // Incrementa o ID global
    totalPecas++; // Aumenta o total de peças
    
    // Avança o ponteiro 'fim' de forma circular
    fim = (fim + 1) % MAX_PECAS;
}

/**
 * @brief Pausa a execucao ate o usuario pressionar Enter.
 */
void pausarTela() {
    printf("\nPressione Enter para continuar...");
    while (getchar() != '\n'); // Aguarda o usuário pressionar Enter
}