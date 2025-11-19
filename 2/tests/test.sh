EXE="../../task2.exe"

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

# expected=$''
# run_test ""

echo -e "\n=== Результаты ==="
echo "PASS: $pass"
echo "FAIL: $fail"
echo -e "\n"
