#!/bin/bash

PROG=./RPN   # بدّلها لاسم executable ديالك إذا مختلف

GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"

run_test() {
  input="$1"
  expected="$2"
  # كنخلي inputs بحال tabs و multiple spaces يخدمو مزيان: خاصك تدوزهم بين quotes
  output=$($PROG "$input" 2>&1 | tr -d '\r')

  if [ "$output" = "$expected" ]; then
    echo -e "${GREEN}OK${RESET}  | \"$input\" => $output"
  else
    echo -e "${RED}KO${RESET}  | \"$input\""
    echo "   expected: $expected"
    echo "   got     : $output"
  fi
}

echo "========== VALID TESTS =========="
run_test "1 2 +" "3"
run_test "1 2 3 - +" "0"
run_test "8 9 * 9 - 9 - 9 - 4 - 1 +" "42"
run_test "3 5 8 * 7 + *" "141"
run_test "9 9 *" "81"
run_test "9 1 /" "9"
run_test "0 9 +" "9"
run_test "9 0 +" "9"
run_test "7 2 - 3 *" "15"      # (7-2)*3=15
run_test "7 2 3 * -" "1"       # 7-(2*3)=1

echo
echo "========== VALID (WEIRD WHITESPACE) =========="
run_test "   1   2   +   " "3"
run_test "1    2    3   -   +" "0"
run_test $'1\t2\t+\t' "3"                 # tabs
run_test $' \t 8  \t9\t*\t ' "72"          # tabs + spaces

echo
echo "========== SHOULD BE ERROR (STACK LEFTOVERS) =========="
run_test "1 2 3 +" "Error"     # leftover [1,5]
run_test "1 2 3" "Error"       # no operators, leftover 3 numbers
run_test "1 2 + 3" "Error"     # leftover [3,3]
run_test "1 2 + +" "Error"     # extra operator

echo
echo "========== SHOULD BE ERROR (NOT ENOUGH OPERANDS) =========="
run_test "+" "Error"
run_test "-" "Error"
run_test "*" "Error"
run_test "/" "Error"
run_test "1 +" "Error"
run_test "1 2 +"$'\n' "Error"  # إذا برنامجك كيرفض newline داخل arg (غالباً ما كيوصلش)، تقدر تحيدها

echo
echo "========== SHOULD BE ERROR (DIV BY ZERO) =========="
run_test "1 0 /" "Error"
run_test "9 0 /" "Error"
run_test "0 0 /" "Error"

echo
echo "========== SHOULD BE ERROR (BAD TOKENS / FORMAT) =========="
run_test "" "Error"
run_test "   " "Error"
run_test "1 2 a +" "Error"
run_test "1 2 ^" "Error"
run_test "1 2 ++" "Error"
run_test "++" "Error"
run_test "1,2 +" "Error"
run_test "1 2+ " "Error"
run_test "1 2 +  " "3"          # هذا VALID: spaces زايدين ماشي مشكل

echo
echo "========== SHOULD BE ERROR (MULTI-DIGIT / NEGATIVE TOKENS) =========="
# إذا subject ديالك يسمح غير بـ digits 0..9، هادو خاصهم Error:
run_test "10 2 +" "Error"
run_test "1 20 +" "Error"
run_test "-1 2 +" "Error"
run_test "1 -2 +" "Error"

echo
echo "========== SHOULD BE ERROR (MANY OPERATORS) =========="
run_test "1 2 + - *" "Error"
run_test "1 2 3 + + +" "Error"

echo
echo "Done."
