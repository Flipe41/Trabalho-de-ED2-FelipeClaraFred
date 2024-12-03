#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição de estruturas para gerenciar tarefas
typedef struct Tarefa {
    char nome[50];
    int prioridade;
    char responsavel[50];
    time_t timestamp;
    struct Tarefa* proximo;
} Tarefa;

typedef struct Historico {
    char nome[50];
    char responsavel[50];
    time_t inicio;
    time_t conclusao;
    struct Historico* proximo;
} Historico;

typedef struct TabelaHash {
    Tarefa* tabela[50]; // Aumentado para 50 para reduzir colisões
} TabelaHash;

// Estruturas globais
Tarefa* filaPendentes = NULL; // Fila de tarefas pendentes
Historico* pilhaHistorico = NULL; // Pilha de histórico de tarefas concluídas
TabelaHash tabelaResponsaveis = {NULL}; // Tabela hash para mapeamento

// Funções auxiliares
int calcularIndice(const char* chave) {
    int hash = 0;
    while (*chave) {
        hash += *chave++;
    }
    return hash % 50;
}

void adicionarTabelaHash(const char* nome, const char* responsavel) {
    int indice = calcularIndice(nome);
    Tarefa* novaTarefa = (Tarefa*)malloc(sizeof(Tarefa));
    strcpy(novaTarefa->nome, nome);
    strcpy(novaTarefa->responsavel, responsavel);
    novaTarefa->proximo = tabelaResponsaveis.tabela[indice];
    tabelaResponsaveis.tabela[indice] = novaTarefa;
}

const char* buscarTabelaHash(const char* nome) {
    int indice = calcularIndice(nome);
    Tarefa* atual = tabelaResponsaveis.tabela[indice];
    while (atual) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual->responsavel;
        }
        atual = atual->proximo;
    }
    return NULL;
}

// Função para limpar o buffer
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funções principais
void adicionarTarefa() {
    Tarefa* novaTarefa = (Tarefa*)malloc(sizeof(Tarefa));
    printf("\nDigite o nome da tarefa: ");
    scanf(" %[^\n]", novaTarefa->nome);

    int prioridade;
    while (1) {
        printf("Digite a prioridade da tarefa (1-10): ");
        if (scanf("%d", &prioridade) == 1 && prioridade >= 1 && prioridade <= 10) {
            novaTarefa->prioridade = prioridade;
            break;
        } else {
            printf("Entrada inválida. Por favor, insira um número entre 1 e 10.\n");
            limparBuffer(); // Limpar o buffer de entrada
        }
    }

    limparBuffer(); // Limpar buffer antes de receber a próxima entrada
    printf("Digite o nome do responsável: ");
    scanf(" %[^\n]", novaTarefa->responsavel);

    novaTarefa->timestamp = time(NULL);
    novaTarefa->proximo = NULL;

    adicionarTabelaHash(novaTarefa->nome, novaTarefa->responsavel);

    if (filaPendentes == NULL) {
        filaPendentes = novaTarefa;
    } else {
        Tarefa* temp = filaPendentes;
        while (temp->proximo) {
            temp = temp->proximo;
        }
        temp->proximo = novaTarefa;
    }

    printf("\nTarefa '%s' adicionada com sucesso.\n\n", novaTarefa->nome);
}

void concluirTarefa() {
    if (filaPendentes == NULL) {
        printf("\nNenhuma tarefa pendente para concluir.\n\n");
        return;
    }

    printf("\nTarefas pendentes:\n");
    Tarefa* temp = filaPendentes;
    int i = 1;
    while (temp) {
        printf("%d. %s (Prioridade: %d, Responsável: %s)\n",
               i++, temp->nome, temp->prioridade, temp->responsavel);
        temp = temp->proximo;
    }

    printf("Escolha a tarefa a ser concluída (número): ");
    int escolha;
    scanf("%d", &escolha);
    limparBuffer();

    Tarefa* anterior = NULL;
    temp = filaPendentes;
    for (i = 1; temp && i < escolha; i++) {
        anterior = temp;
        temp = temp->proximo;
    }

    if (temp == NULL) {
        printf("Escolha inválida.\n\n");
        return;
    }

    Historico* novaConcluida = (Historico*)malloc(sizeof(Historico));
    strcpy(novaConcluida->nome, temp->nome);
    strcpy(novaConcluida->responsavel, temp->responsavel);
    novaConcluida->inicio = temp->timestamp;
    novaConcluida->conclusao = time(NULL);
    novaConcluida->proximo = pilhaHistorico;
    pilhaHistorico = novaConcluida;

    printf("Tarefa '%s' concluída com sucesso.\n\n", temp->nome);

    if (anterior == NULL) {
        filaPendentes = temp->proximo;
    } else {
        anterior->proximo = temp->proximo;
    }
    free(temp);
}

void listarEOrdenarTarefas() {
    if (filaPendentes == NULL) {
        printf("\nNenhuma tarefa pendente.\n\n");
        return;
    }

    // Contar número de tarefas
    int contador = 0;
    Tarefa* atual = filaPendentes;
    while (atual) {
        contador++;
        atual = atual->proximo;
    }

    // Copiar tarefas para array
    Tarefa** array = malloc(contador * sizeof(Tarefa*));
    atual = filaPendentes;
    for (int i = 0; i < contador; i++) {
        array[i] = atual;
        atual = atual->proximo;
    }

    // Ordenar array por prioridade
    for (int i = 0; i < contador - 1; i++) {
        for (int j = i + 1; j < contador; j++) {
            if (array[i]->prioridade > array[j]->prioridade) {
                Tarefa* temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }

    // Exibir tarefas ordenadas
    printf("\nTarefas pendentes (ordenadas por prioridade):\n");
    for (int i = 0; i < contador; i++) {
        printf("Nome: %s; Responsável: %s\n",
               array[i]->nome, array[i]->responsavel);
    }
    printf("\n");

    free(array);
}

void calcularTempoMedioConclusao() {
    if (!pilhaHistorico) {
        printf("\nNenhuma tarefa concluída ainda.\n\n");
        return;
    }

    double somaTempos = 0;
    int contador = 0;
    Historico* temp = pilhaHistorico;

    while (temp) {
        somaTempos += difftime(temp->conclusao, temp->inicio);
        contador++;
        temp = temp->proximo;
    }

    double tempoMedio = somaTempos / contador;
    printf("\nTempo médio de conclusão: %.2f segundos.\n\n", tempoMedio);
}

void buscarTarefasPorResponsavel() {
    char responsavel[50];
    printf("\nDigite o nome do responsável para buscar as tarefas: ");
    scanf(" %[^\n]", responsavel); // Captura corretamente a entrada

    int encontrado = 0;
    for (int i = 0; i < 50; i++) { // Itera sobre a tabela hash
        Tarefa* atual = tabelaResponsaveis.tabela[i];
        while (atual) {
            if (strcmp(atual->responsavel, responsavel) == 0) {
                printf("\nTarefa: %s\n", atual->nome);
                encontrado = 1;
            }
            atual = atual->proximo;
        }
    }

    if (!encontrado) {
        printf("Nenhuma tarefa encontrada para o responsável '%s'.\n\n", responsavel);
    }
}


void consultarHistorico() {
    if (pilhaHistorico == NULL) {
        printf("\nNenhuma tarefa foi concluída ainda.\n\n");
    } else {
        printf("\nHistórico de Tarefas Concluídas:\n");
        Historico* temp = pilhaHistorico;
        while (temp) {
            printf("Tarefa: %s; Responsável: %s; Início: %s; Conclusão: %s\n",
                   temp->nome,
                   temp->responsavel,
                   ctime(&temp->inicio), // Converte o timestamp para string legível
                   ctime(&temp->conclusao));
            temp = temp->proximo;
        }
        printf("\n");
    }
}


int main() {
    int opcao;
    do {
        printf("--- Gerenciador de Tarefas ---\n");
        printf("1. Adicionar Tarefa\n");
        printf("2. Concluir Tarefa\n");
        printf("3. Listar Tarefas Pendentes\n");
        printf("4. Calcular Tempo Médio para Concluir Tarefas\n");
        printf("5. Buscar Tarefas por Responsável\n");
        printf("6. Consultar histórico de Tarefas\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) { // Verifica se a entrada é um número
            printf("Entrada inválida! Por favor, insira um número entre 1 e 7.\n\n");
            limparBuffer(); // Limpa o buffer de entrada
            continue;       // Reinicia o loop
        }

        limparBuffer(); // Garante que o buffer está limpo para próximas entradas

        switch (opcao) {
            case 1:
                adicionarTarefa();
                break;
            case 2:
                concluirTarefa();
                break;
            case 3:
                listarEOrdenarTarefas();
                break;
            case 4:
                calcularTempoMedioConclusao();
                break;
            case 5:
                buscarTarefasPorResponsavel();
                break;
            case 6:
                consultarHistorico();
                break;
            case 7:
                printf("\nEncerrando o programa.\n");
                break;
            default:
                printf("Opção inválida. Por favor, escolha um número entre 1 e 7.\n\n");
        }
    } while (opcao != 7);

    return 0;
}
