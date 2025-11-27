EXE="$(cd ../.. && pwd)/task3.exe"
cd ../..

if [ ! -f "$EXE" ]; then
    echo "Ошибка: $EXE не найден."
    exit 1
fi

pass=0
fail=0

run_test() {
    local name="$1"
    local input="$2"
    shift 2
    local keys=("$@")
    output=$(printf "%b" "$input" | "$EXE" 2>&1)
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
    output=$(printf "%b" "$input" | "$EXE" 2>&1)

    if ! printf "%s" "$output" | grep -F -- "Данные успешно записаны в $filename" >/dev/null 2>&1; then
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

# 1) Поиск

# По id
run_test "Поиск по id" "1\n1\n3\n0\n" \
    "id: 3" "Фамилия: Petrov" "Имя: Ivan"

# По фамилии
run_test "Поиск по фамилии" "1\n2\nSmirnova\n0\n" \
    "id: 2" "Фамилия: Smirnova" "Имя: Anna"

# По имени
run_test "Поиск по имени (2)" "1\n3\nMaria\n0\n" \
    "Фамилия: Petrova" "Фамилия: Orlova" "Имя: Maria"

# По отчеству
run_test "Поиск по отчеству" "1\n4\nAlexandrovich\n0\n" \
    "id: 3" "Отчество: Alexandrovich"

# По дате
run_test "Поиск по дате" "1\n5\n01.01.1985\n0\n" \
    "id: 3" "Дата: 01.01.1985" "Фамилия: Petrov"

# По дню
run_test "Поиск по дню" "1\n6\n1\n0\n" \
    "id: 3" "Дата: 01.01.1985"

# По месяцу
run_test "Поиск по месяцу" "1\n7\n1\n0\n" \
    "id: 3" "Дата: 01.01.1985"

# По году
run_test "Поиск по году" "1\n8\n1985\n0\n" \
    "id: 3" "Дата: 01.01.1985"

# По полу
run_test "Поиск по полу (W)" "1\n9\nW\n0\n" \
    "Пол: W"

# По доходу
run_test "Поиск по доходу" "1\n10\n2100\n0\n" \
    "id: 9" "Фамилия: Orlova" "Доход: 2100.00"

# 2) Изменение

# Изменение фамилии
run_test "Изменение фамилии (id=1)" \
"2\n1\n2\nIvanovNew\n1\n2\nIvanovNew\n0\n" \
    "Житель изменён." "Фамилия: IvanovNew" "id: 1"

# Изменение имени
run_test "Изменение имени (id=2)" \
"2\n2\n3\nAnnaNew\n1\n3\nAnnaNew\n0\n" \
    "Житель изменён." "Имя: AnnaNew" "id: 2"

# Изменение отчества
run_test "Изменение отчества (id=0)" \
"2\n0\n4\nTestovitch\n1\n4\nTestovitch\n0\n" \
    "Житель изменён." "Отчество: Testovitch" "id: 0"

# Изменение даты
run_test "Изменение даты (id=3)" \
"2\n3\n5\n02.02.1986\n1\n5\n02.02.1986\n0\n" \
    "Житель изменён." "Дата: 02.02.1986" "id: 3"

# Изменение дня
run_test "Изменение дня (id=4)" \
"2\n4\n6\n10\n1\n6\n10\n0\n" \
    "Житель изменён." "Дата: 10." "id: 4"

# Изменение месяца
run_test "Изменение месяца (id=4)" \
"2\n4\n7\n12\n1\n7\n12\n0\n" \
"Житель изменён." "Дата: 15.12.1992" "id: 4"

# Изменение года
run_test "Изменение года (id=4)" \
"2\n4\n8\n2001\n1\n8\n2001\n0\n" \
"Житель изменён." "Дата: 15.09.2001" "id: 4"

# Изменение пола
run_test "Изменение пола (id=8)" \
"2\n8\n9\nW\n1\n9\nW\n0\n" \
    "Житель изменён." "Пол: W" "id: 8"

# Изменение дохода
run_test "Изменение дохода (id=6)" \
"2\n6\n10\n3333\n1\n10\n3333\n0\n" \
    "Житель изменён." "Доход: 3333.00" "id: 6"

# 3) Удаление
run_test "Удаление (id=5)" \
"3\n5\n1\n1\n5\n0\n" \
    "Житель с id = 5 удалён." "Не найдено."

# 4) Добавление
run_test "Добавление (id=20)" \
"4\n20\nTestov\nAlex\n\n01.01.2000\nM\n5000\n1\n1\n20\n0\n" \
    "Житель с id = 20 добавлен." "id: 20" "Фамилия: Testov" "Имя: Alex" "Дата: 01.01.2000"

# 5) Выгрузка в файл
OUTF="out_test.txt"
run_test_file "Выгрузка в файл" "5\n$OUTF\n0\n" "$OUTF"

# 6.1) Undo: недостаточно модификаций
run_test "Undo недостаточно модификаций" "6\n0\n" \
    "Нужно минимум 2 модификации для отмены."

# 6.2) UNDO: 2 модификации
run_test "Undo после модификаций" \
"4\n21\nAddLast\nAddFirst\n\n01.01.2001\nM\n1234\n2\n6\n10\n4444\n6\n0\n" \
    "Житель с id = 21 добавлен." "Житель изменён." "Отмена выполнена:"

echo -e "\n"
echo -e "=== Результаты ==="
echo "PASS: $pass"
echo "FAIL: $fail"
echo -e "\n"
