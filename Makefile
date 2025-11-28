CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wextra
CLIBS = -lm

TASKS = 1 2 3 45 6 7
EXECUTABLES = $(foreach t,$(TASKS),task$(t).exe)

VECTOR_SRC = 2/src/vector.c
INT_TEST = 2/tests/test1.c
DOUBLE_TEST = 2/tests/test2.c
CHAR_TEST = 2/tests/test3.c

INT_OUT = task2_int.exe
DOUBLE_OUT = task2_double.exe
CHAR_OUT = task2_char.exe

.PHONY: all clean test int double char

all: $(EXECUTABLES) int double char

task%.exe:
	$(CC) $(CFLAGS) $*/src/main.c $(filter-out $*/src/main.c,$(wildcard $*/src/*.c)) -I$*/include $(CLIBS) -o $@

int:
	$(CC) $(CFLAGS) -DVECTOR_TYPE=int $(INT_TEST) $(VECTOR_SRC) -I2/include $(CLIBS) -o $(INT_OUT)

double:
	$(CC) $(CFLAGS) -DVECTOR_TYPE=double -DVECTOR_TYPE_IS_FLOAT $(DOUBLE_TEST) $(VECTOR_SRC) -I2/include $(CLIBS) -o $(DOUBLE_OUT)

char:
	$(CC) $(CFLAGS) -DVECTOR_TYPE="char*" -DVECTOR_TYPE_IS_CSTRING $(CHAR_TEST) $(VECTOR_SRC) -I2/include $(CLIBS) -o $(CHAR_OUT)

clean:
	rm -f $(EXECUTABLES) $(INT_OUT) $(DOUBLE_OUT) $(CHAR_OUT)
test: all
	@for t in $(TASKS); do \
	    EXE="task$$t.exe"; \
	    if [ -f "$$t/tests/test.sh" ]; then \
	        echo "=== Тестирование задачи $$t ==="; \
	        (cd $$t/tests && bash test.sh) || exit 1; \
	    fi \
	done
	@echo "\n\nТест 1. Тип int"
	./$(INT_OUT)
	@echo "\n\nТест 2. Тип double"
	./$(DOUBLE_OUT)
	@echo "\n\nТест 3. Тип char*"
	./$(CHAR_OUT)
	@echo "\nВсе тесты завершены."
