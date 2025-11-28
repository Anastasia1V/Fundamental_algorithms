EXE="$(cd ../.. && pwd)/task45.exe"
cd ../..

if [ ! -f "$EXE" ]; then
    echo "Ошибка: $EXE не найден."
    exit 1
fi

MAPPINGS="mappings.txt"
: > "$MAPPINGS"

pass=0
fail=0

run_test() {
    local name="$1"
    local input="$2"
    shift 2
    local keys=("$@")
    output=$(printf "%b" "$input" | "$EXE" "$MAPPINGS" 2>&1)
    for key in "${keys[@]}"; do
        if ! printf "%s" "$output" | grep -F -- "$key" >/dev/null 2>&1; then
            echo "[FAIL] $name"
            echo "  Отсутствует ключ: $key"
            echo "---- stdin ----"
            printf "%b\n" "$input"
            echo "---- вывод программы ----"
            printf "%s\n" "$output"
            echo "------------------------"
            fail=$((fail+1))
            return
        fi
    done
    echo "[PASS] $name"
    pass=$((pass+1))
}

run_test_file() {
    local name="$1"
    local input="$2"
    local filename="$3"
    rm -f "$filename"
    output=$(printf "%b" "$input" | "$EXE" "$MAPPINGS" 2>&1)

    if ! printf "%s" "$output" | grep -F -- "Письма успешно записаны в $filename" >/dev/null 2>&1; then
        echo "[FAIL] $name"
        echo "  Ожидалось сообщение о записи в $filename"
        echo "---- stdin ----"
        printf "%b\n" "$input"
        echo "---- вывод программы ----"
        printf "%s\n" "$output"
        fail=$((fail+1))
        return
    fi

    if [ ! -f "$filename" ]; then
        echo "[FAIL] $name"
        echo "  Файл $filename не создан"
        echo "---- stdout ----"
        printf "%s\n" "$output"
        fail=$((fail+1))
        return
    fi

    if [ ! -s "$filename" ]; then
        echo "[FAIL] $name"
        echo "  Файл $filename пустой"
        fail=$((fail+1))
        return
    fi

    echo "[PASS] $name"
    pass=$((pass+1))
    rm -f "$filename"
}

# 1) Добавление офиса
run_test "Добавление офиса" "1\n100\n2\n0\n0\n" \
    "Офис добавлен успешно"

# 2) Удаление офиса
run_test "Добавление и удаление офиса" "1\n110\n3\n0\n2\n110\n0\n" \
    "Офис добавлен успешно" "Офис удалён успешно"

# 3) Создать письмо (нужны два офиса — создаём их в рамках одного запуска)
run_test "Создание письма" "1\n200\n2\n0\n1\n201\n2\n0\n3\nURGENT\n5\n200\n201\nHello\n0\n" \
    "Офис добавлен успешно" "Письмо" "успешно создано"

# 4) Пометить письмо как недоставленное (создаём письмо с id 0 в рамках одного запуска)
run_test "Пометка письма как недоставленное" "1\n300\n2\n0\n1\n301\n2\n0\n3\nINFO\n1\n300\n301\nData\n4\n0\n0\n" \
    "Письмо 0" "помечено как недоставленное"

# 5) Доставка вручную (создаём письмо и затем доставляем вручную id 0)
run_test "Доставка вручную" "1\n400\n2\n0\n1\n401\n2\n0\n3\nFAST\n10\n400\n401\nText\n5\n0\n0\n" \
    "Письмо 0" "доставлено вручную"

# 6) Запись писем в файл (создадим одно письмо и вызовем команду 6)
OUTF="mails_out_test.txt"
run_test_file "Выгрузка писем в файл" "1\n500\n2\n0\n1\n501\n2\n0\n3\nNORM\n2\n500\n501\nPayload\n6\n$OUTF\n0\n" "$OUTF"

# 7) Попытка создать письмо в несуществующих офисах (ожидаем ошибку создания)
run_test "Создание письма в несуществующих офисах" "3\nBAD\n3\n9000\n9001\nX\n0\n" \
    "Ошибка создания письма"

# 8) Создание офиса с соседями и затем добавление письма (проверка ввода соседей)
run_test "Добавить офис с соседями и письмо" "1\n600\n3\n2\n601\n602\n1\n601\n2\n0\n3\nT\n1\n600\n601\nHi\n0\n" \
    "Офис добавлен успешно" "Письмо" "успешно создано"

echo -e "\n=== Результаты ==="
echo "PASS: $pass"
echo "FAIL: $fail"
echo -e "\n"
