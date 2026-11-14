/*
 * Caixa Eletrônico em C — Atividade 2 (projeto introdutório)
 *
 * - Saldo inicial de R$ 600,00.
 * - Lê o valor do saque (só números inteiros) e repete a leitura em caso
 *   de entrada inválida, valor zero/negativo ou valor acima do saldo.
 * - Entrega o valor em notas de 100, 50, 20, 10, 5 e 2 e moedas de 1.
 * - Depois de cada saque ou consulta mostra o menu:
 *     1 - Consultar saldo | 2 - Novo saque | 3 - Sair
 *
 * Compilar:  gcc -std=c11 -Wall -Wextra caixa_eletronico.c -o caixa_eletronico.exe
 * Executar:  .\caixa_eletronico.exe
 *
 * Usa apenas stdio.h. As mensagens não têm acento para aparecerem
 * corretamente no console do Windows.
 */
#include <stdio.h>

#define SALDO_INICIAL 600

/* Resultado da leitura de um número inteiro */
#define LEITURA_OK 1
#define LEITURA_INVALIDA 0
#define LEITURA_FIM (-1)

/*
 * Lê um número inteiro ocupando a linha inteira.
 * - %9d limita a nove caracteres, para não estourar o int.
 * - Espaços antes e depois do número são aceitos.
 * - Letras, decimais ou números extras tornam a entrada inválida.
 * Retorna LEITURA_OK, LEITURA_INVALIDA ou LEITURA_FIM (entrada encerrada).
 */
int ler_inteiro(int *valor)
{
    int lidos = scanf("%9d", valor);
    int c;
    int sobrou_texto = 0;

    if (lidos == EOF) {
        return LEITURA_FIM;
    }

    /* Limpa o resto da linha e verifica se sobrou algo além de espaços */
    while ((c = getchar()) != '\n' && c != EOF) {
        if (c != ' ' && c != '\t' && c != '\r') {
            sobrou_texto = 1;
        }
    }

    if (lidos != 1 || sobrou_texto) {
        if (c == EOF) {
            return LEITURA_FIM;
        }
        return LEITURA_INVALIDA;
    }
    return LEITURA_OK;
}

/* Mostra as notas e moedas entregues (só as quantidades positivas) */
void entregar_cedulas(int valor)
{
    const int notas[] = {100, 50, 20, 10, 5, 2};
    const int total_notas = sizeof(notas) / sizeof(notas[0]);
    int i;
    int quantidade;

    printf("Retire o dinheiro:\n");
    for (i = 0; i < total_notas; i++) {
        quantidade = valor / notas[i];
        valor = valor % notas[i];
        if (quantidade > 0) {
            printf("  %d nota(s) de R$ %d,00\n", quantidade, notas[i]);
        }
    }
    if (valor > 0) {
        printf("  %d moeda(s) de R$ 1,00\n", valor);
    }
}

/*
 * Repete a leitura até receber um saque válido e o realiza.
 * Retorna 1 se o saque foi feito ou 0 se a entrada foi encerrada.
 */
int realizar_saque(int *saldo)
{
    int valor;
    int leitura;

    while (1) {
        printf("\nSaldo disponivel: R$ %d,00\n", *saldo);
        printf("Digite o valor do saque (somente numeros inteiros): ");
        leitura = ler_inteiro(&valor);

        if (leitura == LEITURA_FIM) {
            return 0;
        }
        if (leitura == LEITURA_INVALIDA) {
            printf("Entrada invalida. Digite apenas numeros inteiros, sem R$ e sem centavos.\n");
        } else if (valor <= 0) {
            printf("Valor invalido. O saque deve ser maior que zero.\n");
        } else if (valor > *saldo) {
            printf("Saldo insuficiente. Voce pode sacar ate R$ %d,00.\n", *saldo);
        } else {
            *saldo -= valor;
            printf("\nSaque de R$ %d,00 realizado.\n", valor);
            entregar_cedulas(valor);
            printf("Saldo atual: R$ %d,00\n", *saldo);
            return 1;
        }
    }
}

int main(void)
{
    int saldo = SALDO_INICIAL;
    int opcao;
    int leitura;

    printf("=== CAIXA ELETRONICO ===\n");

    if (!realizar_saque(&saldo)) {
        printf("\nEntrada encerrada. Atendimento finalizado.\n");
        return 0;
    }

    while (1) {
        printf("\nDeseja realizar uma nova transacao?\n");
        printf("1 - Consultar saldo\n");
        printf("2 - Novo saque\n");
        printf("3 - Sair\n");
        printf("Opcao: ");
        leitura = ler_inteiro(&opcao);

        if (leitura == LEITURA_FIM) {
            printf("\nEntrada encerrada. Atendimento finalizado.\n");
            break;
        }
        if (leitura == LEITURA_INVALIDA || opcao < 1 || opcao > 3) {
            printf("Opcao invalida. Escolha 1, 2 ou 3.\n");
            continue;
        }

        if (opcao == 1) {
            printf("Saldo atual: R$ %d,00\n", saldo);
        } else if (opcao == 2) {
            if (saldo > 0) {
                if (!realizar_saque(&saldo)) {
                    printf("\nEntrada encerrada. Atendimento finalizado.\n");
                    break;
                }
            } else {
                printf("Saldo insuficiente. Seu saldo e R$ 0,00.\n");
            }
        } else {
            printf("Atendimento finalizado. Obrigado por usar o caixa eletronico!\n");
            break;
        }
    }

    return 0;
}
