#!/usr/bin/env bash
# Compila o caixa eletrônico e roda os casos de teste do README.
# Uso: bash testes/executar_testes.sh
set -u

cd "$(dirname "$0")/.."

gcc -std=c11 -Wall -Wextra -Werror -pedantic caixa_eletronico.c -o caixa_eletronico.exe || {
  echo "FALHOU: o codigo nao compilou sem avisos"
  exit 1
}

falhas=0
total=0

# caso "nome" "entrada" "texto esperado 1" "texto esperado 2" ...
caso() {
  local nome="$1" entrada="$2"
  shift 2
  total=$((total + 1))
  local saida
  saida=$(printf "%b" "$entrada" | timeout 5 ./caixa_eletronico.exe)
  local codigo=$?
  local ok=1
  if [ "$codigo" -ne 0 ]; then
    ok=0
    echo "  codigo de saida $codigo (esperado 0)"
  fi
  for esperado in "$@"; do
    if ! grep -qF -- "$esperado" <<<"$saida"; then
      ok=0
      echo "  nao encontrou: $esperado"
    fi
  done
  if [ "$ok" -eq 1 ]; then
    echo "OK     $nome"
  else
    echo "FALHOU $nome"
    falhas=$((falhas + 1))
  fi
}

# Casos da tabela "Testes" do README
caso "95, 3" "95\n3\n" \
  "Saque de R\$ 95,00 realizado." "1 nota(s) de R\$ 50,00" "2 nota(s) de R\$ 20,00" "1 nota(s) de R\$ 5,00" \
  "Saldo atual: R\$ 505,00" "Atendimento finalizado"

caso "123, 1, 2, 100, 1, 3" "123\n1\n2\n100\n1\n3\n" \
  "Saque de R\$ 123,00 realizado." "1 nota(s) de R\$ 2,00" "1 moeda(s) de R\$ 1,00" \
  "Saldo atual: R\$ 477,00" "Saque de R\$ 100,00 realizado." "Saldo atual: R\$ 377,00"

caso "500, 2, 101, 100, 1, 3" "500\n2\n101\n100\n1\n3\n" \
  "Saldo atual: R\$ 100,00" "Saldo insuficiente. Voce pode sacar ate R\$ 100,00." \
  "Saque de R\$ 100,00 realizado." "Saldo atual: R\$ 0,00"

caso "600, 2, 1, 3" "600\n2\n1\n3\n" \
  "6 nota(s) de R\$ 100,00" "Saldo insuficiente. Seu saldo e R\$ 0,00." "Saldo atual: R\$ 0,00" \
  "Atendimento finalizado"

caso "50, abc, 4, 1, 3" "50\nabc\n4\n1\n3\n" \
  "Opcao invalida. Escolha 1, 2 ou 3." "Saldo atual: R\$ 550,00"

# Validação da entrada do saque
caso "letras, decimal, negativo, zero, acima do saldo, numero longo" \
  "abc\n12.5\n-5\n0\n700\n1234567890\n12 3\n  20  \n3\n" \
  "Entrada invalida" "Valor invalido" "Saldo insuficiente. Voce pode sacar ate R\$ 600,00." \
  "Saque de R\$ 20,00 realizado." "Saldo atual: R\$ 580,00"

caso "todas as notas e a moeda (188)" "188\n3\n" \
  "1 nota(s) de R\$ 100,00" "1 nota(s) de R\$ 50,00" "1 nota(s) de R\$ 20,00" "1 nota(s) de R\$ 10,00" \
  "1 nota(s) de R\$ 5,00" "1 nota(s) de R\$ 2,00" "1 moeda(s) de R\$ 1,00"

caso "entrada encerrada sem sair pelo menu" "50\n" \
  "Saldo atual: R\$ 550,00" "Entrada encerrada"

caso "fim de linha do Windows (CRLF)" "95\r\n1\r\n3\r\n" \
  "Saldo atual: R\$ 505,00" "Atendimento finalizado"

echo
echo "Resultado: $((total - falhas)) de $total casos OK"
[ "$falhas" -eq 0 ]
