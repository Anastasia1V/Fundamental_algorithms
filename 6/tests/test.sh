EXE="../../task6.exe"

if [ ! -f "$EXE" ]; then
    echo "Ошибка: $EXE не найден."
    exit 1
fi

pass=0
fail=0

run_test() {
    local description="$1"
    local expected="$2"
    local input="$3"
    output=$(printf "%s" "$input" | "$EXE" 2>&1)
    if [ "$output" = "$expected" ]; then
        echo "[PASS] $description"
        pass=$((pass+1))
    else
        echo "[FAIL] $description"
        echo "  ---stdin---"
        printf "%s\n" "$input"
        echo "  ---Ожидалось---"
        printf "%s\n" "$expected"
        echo "  ---Получено---"
        printf "%s\n" "$output"
        echo "---------------"
        fail=$((fail+1))
    fi
}

# 1. Пустая строка
expected=""
run_test "Тест 1. Пустая строка" "$expected" ""

# 2. Правильные скобки ()
expected="Да"
run_test "Тест 2. ()" "$expected" "()"

# 3. Правильные скобки {[]}(<>)
expected="Да"
run_test "Тест 3. {[]}(<>)" "$expected" "{[]}(<>)"

# 4. Неправильные скобки ([)]
expected="Нет"
run_test "Тест 4. ([)]" "$expected" "([)]"

# 5. Правильные скобки 123(abc{<qwe()><[456{789}]321>})37642[abc(012)]
expected="Да"
run_test "Тест 5. 123(abc{<qwe()><[456{789}]321>})37642[abc(012)]" "$expected" "123(abc{<qwe()><[456{789}]321>})37642[abc(012)]"

# 6. Правильные скобки {    ([<  > []])}     <>
expected="Да"
run_test "Тест 6. {    ([<  > []])}     <>" "$expected" "{    ([<  > []])}     <>"

# 7. Неправильные скобки ([]){(<)})
expected="Нет"
run_test "Тест 7. ([]){(<)})" "$expected" "([]){(<)})"

# 8. Неправильные скобки (([]){(<)}
expected="Нет"
run_test "Тест 8. (([]){(<)}" "$expected" "(([]){(<)}"

# 9. Правильные скобки 12345((({}abc  <[]()>)))
expected="Да"
run_test "Тест 9. 12345((({}abc  <[]()>)))" "$expected" "12345((({}abc  <[]()>)))"

# 10. Текст без скобок
expected="Да"
run_test "Тест 10. 123 abc" "$expected" "123 abc"

echo -e "\n"
echo -e "=== Результаты ==="
echo "PASS: $pass"
echo "FAIL: $fail"
echo -e "\n"
