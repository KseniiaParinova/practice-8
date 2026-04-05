# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -I include
CFLAGS_DEBUG = -g -O0
CFLAGS_RELEASE = -O2 -DNDEBUG
LDFLAGS = -lm

# Директории
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Файлы
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LIB_OBJS = $(filter-out $(OBJ_DIR)/main.o,$(OBJS))
TARGET = $(BIN_DIR)/mathlib_demo
LIB = $(BIN_DIR)/libmathlib.a

# Цели по умолчанию
.PHONY: all clean lib demo test

all: lib demo

lib: $(LIB)

demo: $(TARGET)

# Статическая библиотека
$(LIB): $(LIB_OBJS) | $(BIN_DIR)
	ar rcs $@ $^

# Исполняемый файл демо
$(TARGET): $(OBJ_DIR)/main.o $(LIB) | $(BIN_DIR)
	$(CC) $< -L$(BIN_DIR) -lmathlib $(LDFLAGS) -o $@

# Компиляция объектных файлов
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -c $< -o $@

# Создание директорий
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Тесты
test: $(BIN_DIR)/test_mathlib
	./$(BIN_DIR)/test_mathlib

$(BIN_DIR)/test_mathlib: tests/test_mathlib.c $(LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $< -L$(BIN_DIR) -lmathlib $(LDFLAGS) -o $@

# Очистка
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)