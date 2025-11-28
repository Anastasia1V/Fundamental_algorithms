EXE="$(cd ../.. && pwd)/task7.exe"
cd ../..

if [ ! -f "$EXE" ]; then
    echo "Ошибка: $EXE не найден."
    exit 1
fi

pass=0
fail=0

run_test_file() {
    local name="$1"
    local input_text="$2"
    local infile="$3"
    local outfile="$4"
    rm -f "$infile" "$outfile"
    printf "%b" "$input_text" > "$infile"
    "$EXE" "$infile" "$outfile" >/dev/null 2>&1 || true

    if [ ! -f "$outfile" ]; then
        echo "[FAIL] $name"
        echo "  Файл $outfile не создан"
        fail=$((fail+1))
        return
    fi

    if [ ! -s "$outfile" ]; then
        echo "[FAIL] $name"
        echo "  Файл $outfile пустой"
        fail=$((fail+1))
        return
    fi

    shift 4
    local keys=("$@")
    for key in "${keys[@]}"; do
        if ! grep -F -- "$key" "$outfile" >/dev/null 2>&1; then
            echo "[FAIL] $name"
            echo "  Отсутствует ключ в $outfile: $key"
            echo "---- stdin ----"
            printf "%b\n" "$input_text"
            echo "---- $outfile ----"
            sed -n '1,200p' "$outfile"
            echo "------------------------"
            fail=$((fail+1))
            return
        fi
    done

    echo "[PASS] $name"
    pass=$((pass+1))
    rm -f "$infile" "$outfile"
}

# Тест 1
INPUT1="A = 2
B = 3
C = A + B * 2
D = C ^ B
print(D)
"
run_test_file "Тест 1" "$INPUT1" "tmp_in1.txt" "tmp_out1.txt" \
    "[1] A = 2" "[2] B = 3" "[3] C = A + B * 2" "[4] D = C ^ B" "Print D"

# Тест 2
INPUT2="A = 2
B = 3
C = A + B * 2 D = C ^ B print(D)
"
run_test_file "Тест 2" "$INPUT2" "tmp_in2.txt" "tmp_out2.txt" \
    "[3] C = A + B * 2" "[4] D = C ^ B" "print(D)" "Print D"

# Тест 3
INPUT3="A = 2
B = 3
K = 2 + 3 * 4
print(K)
"
run_test_file "Тест 3" "$INPUT3" "tmp_in3.txt" "tmp_out3.txt" \
    "[3] K = 2 + 3 * 4" "K=14" "Print K"

# Тест 4
INPUT4="A = 2
E = X + 1
"
run_test_file "Тест 4" "$INPUT4" "tmp_in4.txt" "tmp_out4.txt" \
    "Ошибка на линии"

# Тест 5
INPUT5="A = 2
B = 0
C = A / B
"
run_test_file "Тест 5" "$INPUT5" "tmp_in5.txt" "tmp_out5.txt" \
    "Ошибка на линии"

echo -e "\n"
echo -e "=== Результаты ==="
echo "PASS: $pass"
echo "FAIL: $fail"
echo -e "\n"

if [ "$fail" -ne 0 ]; then
    exit 2
fi

exit 0
