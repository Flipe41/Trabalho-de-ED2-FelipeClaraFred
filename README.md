

## Gerenciador de Tarefas

### Descrição
Este é um sistema de gerenciamento de tarefas que utiliza diferentes estruturas de dados para simular a organização e execução de tarefas em um projeto de software.

### Funcionalidades
- Adicionar novas tarefas com prioridade e responsável.
- Concluir tarefas pendentes e movê-las para o histórico.
- Listar tarefas pendentes e ordená-las por prioridade.
- Calcular o tempo médio de conclusão das tarefas.
- Buscar tarefas atribuídas a um responsável.
- Consultar histórico de tarefas concluídas.

### Execução
1. Clone o repositório:
   ```bash
   git clone https://github.com/Flipe41/Trabalho-de-ED2-FelipeClaraFred
   ```
2. Compile o código:
   ```bash
   gcc -o gerenciador gerenciador.c
   ```
3. Execute o programa:
   ```bash
   ./gerenciador
   ```

### Dependências
- GCC ou qualquer compilador C compatível.

### Estruturas utilizadas
- Fila
- Pilha
- Tabela Hash

### Exemplo de uso
1. Adicione uma tarefa:
   ```
   Nome: Atualizar Documentação
   Prioridade: 7
   Responsável: Maria
   ```
2. Liste as tarefas pendentes:
   ```
   Nome: Atualizar Documentação; Prioridade: 7; Responsável: Maria
   ```
3. Conclua a tarefa e calcule o tempo médio:
   ```
   Tempo médio de conclusão: 50.25 segundos.
   ```

