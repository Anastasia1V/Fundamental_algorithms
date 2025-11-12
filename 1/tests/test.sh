EXE="../../task1.exe"

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

# 1. Пустая строка —> help
expected_usage="Использование: <десятичное число> <r>"
run_test "Тест 1. Пустая строка" "$expected_usage" ""

# 2. 10 в 2^1 = 2 -> 1010
expected_usage=$'Использование: <десятичное число> <r>\n1010'
run_test "Тест 2. 10 1 -> 1010" "$expected_usage" "10 1\n"

# 3. 255 в 2^1 = 2 -> 11111111
expected_usage=$'Использование: <десятичное число> <r>\n11111111'
run_test "Тест 3. 255 1 -> 11111111" "$expected_usage" "255 1\n"

# 4. 31 в 2^5 = 32 -> V
expected_usage=$'Использование: <десятичное число> <r>\nV'
run_test "Тест 4. 31 5 -> V" "$expected_usage" "31 5\n"

# 5. 1023 в 2^5 = 32 -> VV
expected_usage=$'Использование: <десятичное число> <r>\nVV'
run_test "Тест 5. 1023 5 -> VV" "$expected_usage" "1023 5\n"

# 6. 0 в 2^3 = 8 -> 0
expected_usage=$'Использование: <десятичное число> <r>\n0'
run_test "Тест 6. 0 3 -> 0" "$expected_usage" "0 3\n"

# 7. 7 в 2^2 = 4 -> 13
expected=$'Использование: <десятичное число> <r>\n13'
run_test "Тест 7. 7 2 -> 13" "$expected" "7 2\n"

# 8. 26 в 2^4 = 16 -> 1A
expected=$'Использование: <десятичное число> <r>\n1A'
run_test "Тест 8. 26 4 -> 1A" "$expected" "26 4\n"

echo -e "\n=== Результаты ==="
echo "PASS: $pass"
echo "FAIL: $fail"
echo -e "\n"
