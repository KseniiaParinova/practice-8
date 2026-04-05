# Практическая работа 8: Многофайловые проекты
**Студент:** Паринова Ксения Николаевна
**Группа:** 1зб_ИВТ(ускор.)/25
**Дата:** 04 апреля 2026 г.
---
## Задание 8.1: Математическая библиотека
### Постановка задачи
Создайте библиотеку математических функций.
### Структура проекта
mathlib/
├── include/
│   ├── mathlib.h      # Основной заголовок
│   ├── vector.h       # Операции с векторами
│   └── matrix.h       # Операции с матрицами
├── src/
│   ├── basic.c        # Базовые функции
│   ├── vector.c       # Реализация векторов
│   ├── matrix.c       # Реализация матриц
│   └── main.c         # Демонстрация
├── tests/
│   └── test_mathlib.c # Тесты
├── Makefile
└── meson.build
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| Vector | struct | Динамический вектор |
| Matrix | struct | Матрица |
| ml_sqrt | double (*)(double) | Квадратный корень |
| ml_factorial | long long (*)(int) | Факториал |
| ml_gcd | long long (*)(long long, long long) | НОД |
### Код программы
include/mathlib.h:
```c
#ifndef MATHLIB_H
#define MATHLIB_H

#define MATHLIB_PI 3.14159265358979323846
#define MATHLIB_E  2.71828182845904523536

double ml_abs(double x);
double ml_pow(double base, int exp);
double ml_sqrt(double x);
long long ml_factorial(int n);
long long ml_gcd(long long a, long long b);
double ml_sin(double x);
double ml_cos(double x);

#endif
```
include/vector.h:
```c
#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct {
    double *data;
    size_t size;
} Vector;

// Создание и уничтожение
Vector* vec_create(size_t size);
Vector* vec_from_array(const double *arr, size_t size);
void vec_free(Vector *v);
Vector* vec_copy(const Vector *v);

// Базовые операции
void vec_add(Vector *result, const Vector *a, const Vector *b);
void vec_sub(Vector *result, const Vector *a, const Vector *b);
void vec_scale(Vector *v, double scalar);
double vec_dot(const Vector *a, const Vector *b);

// Вывод
void vec_print(const Vector *v);

#endif // VECTOR_H
```
include/matrix.h:
```c
#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

typedef struct {
    double *data;
    size_t rows;
    size_t cols;
} Matrix;

// Создание и уничтожение
Matrix* mat_create(size_t rows, size_t cols);
Matrix* mat_identity(size_t n);
void mat_free(Matrix *m);
Matrix* mat_copy(const Matrix *m);

// Доступ к элементам
double mat_get(const Matrix *m, size_t row, size_t col);
void mat_set(Matrix *m, size_t row, size_t col, double value);

// Операции
void mat_add(Matrix *result, const Matrix *a, const Matrix *b);
void mat_sub(Matrix *result, const Matrix *a, const Matrix *b);
void mat_mul(Matrix *result, const Matrix *a, const Matrix *b);
void mat_scale(Matrix *m, double scalar);
Matrix* mat_transpose(const Matrix *m);
double mat_determinant(const Matrix *m);
Matrix* mat_inverse(const Matrix *m);

// Вывод
void mat_print(const Matrix *m);

#endif // MATRIX_H
```
src/basic.c:
```c
#include <math.h>
#include "mathlib.h"

double ml_sqrt(double x) {
    double guess = x / 2.0;
    double prev_guess;
    do {
        prev_guess = guess;
        guess = (guess + x / guess) / 2.0;
    } while (fabs(guess - prev_guess) > 1e-12);
    return guess;
}

long long ml_factorial(int n) {
    if (n < 0) return 0;
    if (n <= 1) return 1;
    long long result = 1;
    for (int i = 2; i <= n; i++) result *= i;
    return result;
}
```
src/vector.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

Vector* vec_create(size_t size) {
    Vector *v = malloc(sizeof(Vector));
    if (v == NULL) return NULL;
    
    v->data = calloc(size, sizeof(double));
    if (v->data == NULL) {
        free(v);
        return NULL;
    }
    
    v->size = size;
    return v;
}

Vector* vec_from_array(const double *arr, size_t size) {
    Vector *v = vec_create(size);
    if (v == NULL) return NULL;
    
    memcpy(v->data, arr, size * sizeof(double));
    return v;
}

void vec_free(Vector *v) {
    if (v == NULL) return;
    free(v->data);
    free(v);
}

Vector* vec_copy(const Vector *v) {
    if (v == NULL) return NULL;
    return vec_from_array(v->data, v->size);
}

void vec_add(Vector *result, const Vector *a, const Vector *b) {
    if (result == NULL || a == NULL || b == NULL) return;
    if (a->size != b->size || result->size != a->size) return;
    
    for (size_t i = 0; i < a->size; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }
}

void vec_sub(Vector *result, const Vector *a, const Vector *b) {
    if (result == NULL || a == NULL || b == NULL) return;
    if (a->size != b->size || result->size != a->size) return;
    
    for (size_t i = 0; i < a->size; i++) {
        result->data[i] = a->data[i] - b->data[i];
    }
}

void vec_scale(Vector *v, double scalar) {
    if (v == NULL) return;
    
    for (size_t i = 0; i < v->size; i++) {
        v->data[i] *= scalar;
    }
}

double vec_dot(const Vector *a, const Vector *b) {
    if (a == NULL || b == NULL || a->size != b->size) return 0.0;
    
    double result = 0.0;
    for (size_t i = 0; i < a->size; i++) {
        result += a->data[i] * b->data[i];
    }
    return result;
}

void vec_print(const Vector *v) {
    if (v == NULL) return;
    
    printf("[");
    for (size_t i = 0; i < v->size; i++) {
        printf("%.2f", v->data[i]);
        if (i < v->size - 1) printf(", ");
    }
    printf("]\n");
}
```
src/matrix.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrix.h"

Matrix* mat_create(size_t rows, size_t cols) {
    Matrix *m = malloc(sizeof(Matrix));
    if (m == NULL) return NULL;
    
    m->data = calloc(rows * cols, sizeof(double));
    if (m->data == NULL) {
        free(m);
        return NULL;
    }
    
    m->rows = rows;
    m->cols = cols;
    return m;
}

Matrix* mat_identity(size_t n) {
    Matrix *m = mat_create(n, n);
    if (m == NULL) return NULL;
    
    for (size_t i = 0; i < n; i++) {
        m->data[i * n + i] = 1.0;
    }
    
    return m;
}

void mat_free(Matrix *m) {
    if (m == NULL) return;
    free(m->data);
    free(m);
}

Matrix* mat_copy(const Matrix *m) {
    if (m == NULL) return NULL;
    
    Matrix *copy = mat_create(m->rows, m->cols);
    if (copy == NULL) return NULL;
    
    memcpy(copy->data, m->data, m->rows * m->cols * sizeof(double));
    return copy;
}

double mat_get(const Matrix *m, size_t row, size_t col) {
    if (m == NULL || row >= m->rows || col >= m->cols) return 0.0;
    return m->data[row * m->cols + col];
}

void mat_set(Matrix *m, size_t row, size_t col, double value) {
    if (m == NULL || row >= m->rows || col >= m->cols) return;
    m->data[row * m->cols + col] = value;
}

void mat_add(Matrix *result, const Matrix *a, const Matrix *b) {
    if (result == NULL || a == NULL || b == NULL) return;
    if (a->rows != b->rows || a->cols != b->cols) return;
    if (result->rows != a->rows || result->cols != a->cols) return;
    
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }
}

void mat_sub(Matrix *result, const Matrix *a, const Matrix *b) {
    if (result == NULL || a == NULL || b == NULL) return;
    if (a->rows != b->rows || a->cols != b->cols) return;
    if (result->rows != a->rows || result->cols != a->cols) return;
    
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] - b->data[i];
    }
}

void mat_mul(Matrix *result, const Matrix *a, const Matrix *b) {
    if (result == NULL || a == NULL || b == NULL) return;
    if (a->cols != b->rows) return;
    if (result->rows != a->rows || result->cols != b->cols) return;
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result->data[i * b->cols + j] = sum;
        }
    }
}

void mat_scale(Matrix *m, double scalar) {
    if (m == NULL) return;
    
    for (size_t i = 0; i < m->rows * m->cols; i++) {
        m->data[i] *= scalar;
    }
}

Matrix* mat_transpose(const Matrix *m) {
    if (m == NULL) return NULL;
    
    Matrix *t = mat_create(m->cols, m->rows);
    if (t == NULL) return NULL;
    
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            t->data[j * m->rows + i] = m->data[i * m->cols + j];
        }
    }
    
    return t;
}

void mat_print(const Matrix *m) {
    if (m == NULL) return;
    
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%8.2f ", mat_get(m, i, j));
        }
        printf("\n");
    }
}
```
src/main.c:
```c
#include <stdio.h>
#include "mathlib.h"

int main(void) {
    printf("=== Математическая библиотека ===\n\n");
    
    // Базовые функции
    printf("Базовые функции:\n");
    printf("ml_abs(-5) = %.0f\n", ml_abs(-5));
    printf("ml_pow(2, 10) = %.0f\n", ml_pow(2, 10));
    printf("ml_sqrt(16) = %.2f\n", ml_sqrt(16));
    printf("ml_factorial(5) = %lld\n", ml_factorial(5));
    printf("ml_gcd(48, 18) = %lld\n", ml_gcd(48, 18));
    printf("ml_lcm(12, 18) = %lld\n\n", ml_lcm(12, 18));
    
    // Тригонометрия
    printf("Тригонометрия:\n");
    printf("ml_sin(π/2) = %.4f\n", ml_sin(MATHLIB_PI / 2));
    printf("ml_cos(0) = %.4f\n", ml_cos(0));
    printf("ml_deg_to_rad(180) = %.4f\n\n", ml_deg_to_rad(180));
    
    // Векторы
    printf("Векторы:\n");
    double arr1[] = {1.0, 2.0, 3.0};
    double arr2[] = {4.0, 5.0, 6.0};
    
    Vector *v1 = vec_from_array(arr1, 3);
    Vector *v2 = vec_from_array(arr2, 3);
    Vector *v3 = vec_create(3);
    
    printf("v1 = "); vec_print(v1);
    printf("v2 = "); vec_print(v2);
    
    vec_add(v3, v1, v2);
    printf("v1 + v2 = "); vec_print(v3);
    
    printf("v1 · v2 = %.2f\n\n", vec_dot(v1, v2));
    
    // Матрицы
    printf("Матрицы:\n");
    Matrix *m1 = mat_create(2, 3);
    Matrix *m2 = mat_create(3, 2);
    
    mat_set(m1, 0, 0, 1); mat_set(m1, 0, 1, 2); mat_set(m1, 0, 2, 3);
    mat_set(m1, 1, 0, 4); mat_set(m1, 1, 1, 5); mat_set(m1, 1, 2, 6);
    
    mat_set(m2, 0, 0, 7); mat_set(m2, 0, 1, 8);
    mat_set(m2, 1, 0, 9); mat_set(m2, 1, 1, 10);
    mat_set(m2, 2, 0, 11); mat_set(m2, 2, 1, 12);
    
    printf("Матрица A (2x3):\n");
    mat_print(m1);
    printf("\nМатрица B (3x2):\n");
    mat_print(m2);
    
    Matrix *m3 = mat_create(2, 2);
    mat_mul(m3, m1, m2);
    printf("\nA × B (2x2):\n");
    mat_print(m3);
    
    // Освобождение памяти
    vec_free(v1);
    vec_free(v2);
    vec_free(v3);
    mat_free(m1);
    mat_free(m2);
    mat_free(m3);
    
    return 0;
}
```
Makefile:
```c
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
```
meson.build:
```c
project('mathlib', 'c',
    version: '1.0.0',
    default_options: [
        'warning_level=3',
        'c_std=c11'
    ]
)

inc = include_directories('include')

cc = meson.get_compiler('c')
math_dep = cc.find_library('m', required: false)

lib_src = files(
    'src/basic.c',
    'src/vector.c',
    'src/matrix.c'
)

mathlib = static_library('mathlib', lib_src,
    include_directories: inc,
    dependencies: [math_dep]
)

executable('mathlib_demo', 'src/main.c',
    include_directories: inc,
    link_with: mathlib,
    dependencies: [math_dep]
)

test_exe = executable('test_mathlib', 'tests/test_mathlib.c',
    include_directories: inc,
    link_with: mathlib,
    dependencies: [math_dep]
)

test('mathlib tests', test_exe)
```
tests/test_mathlib.c:
```c
#include <stdio.h>
#include <math.h>
#include "mathlib.h"

int _test_failed = 0;

#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            printf("FAILED: %s:%d: %lld != %lld\n", __FILE__, __LINE__, (long long)(a), (long long)(b)); \
            _test_failed = 1; \
        } \
    } while(0)

#define ASSERT_DOUBLE_EQ(a, b, eps) \
    do { \
        double diff = (a) - (b); \
        if (diff < 0) diff = -diff; \
        if (diff > (eps)) { \
            printf("FAILED: %s:%d: %f != %f\n", __FILE__, __LINE__, (double)(a), (double)(b)); \
            _test_failed = 1; \
        } \
    } while(0)

void test_factorial(void) {
    ASSERT_EQ(ml_factorial(0), 1);
    ASSERT_EQ(ml_factorial(1), 1);
    ASSERT_EQ(ml_factorial(5), 120);
    ASSERT_EQ(ml_factorial(10), 3628800);
    printf("  test_factorial: PASSED\n");
}

void test_sqrt(void) {
    ASSERT_DOUBLE_EQ(ml_sqrt(4.0), 2.0, 0.0001);
    ASSERT_DOUBLE_EQ(ml_sqrt(9.0), 3.0, 0.0001);
    ASSERT_DOUBLE_EQ(ml_sqrt(2.0), 1.41421356, 0.0001);
    printf("  test_sqrt: PASSED\n");
}

void test_gcd_lcm(void) {
    ASSERT_EQ(ml_gcd(48, 18), 6);
    ASSERT_EQ(ml_gcd(17, 19), 1);
    ASSERT_EQ(ml_lcm(12, 18), 36);
    printf("  test_gcd_lcm: PASSED\n");
}

void test_trigonometry(void) {
    ASSERT_DOUBLE_EQ(ml_sin(0), 0.0, 0.0001);
    ASSERT_DOUBLE_EQ(ml_cos(0), 1.0, 0.0001);
    ASSERT_DOUBLE_EQ(ml_sin(MATHLIB_PI / 2), 1.0, 0.0001);
    printf("  test_trigonometry: PASSED\n");
}

int main(void) {
    printf("\n=== Тестирование MathLib ===\n\n");
    
    test_factorial();
    test_sqrt();
    test_gcd_lcm();
    test_trigonometry();
    
    printf("\n=== Результат ===\n");
    if (_test_failed) {
        printf("Некоторые тесты не пройдены!\n");
        return 1;
    } else {
        printf("Все тесты пройдены успешно!\n");
        return 0;
    }
}
```
---
## Задание 8.2: Библиотека строк
### Постановка задачи
Создайте библиотеку для работы со строками.
### Структура проекта
strlib/
├── include/
│   └── strlib.h
├── src/
│   ├── strlib_core.c   # Базовые операции
│   ├── strlib_search.c # Поиск
│   ├── strlib_format.c # Форматирование
│   └── main.c
├── tests/
│   └── test_strlib.c
├── Makefile
└── meson.build
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| String | struct | Динамическая строка |
| str_new | String* (*)(const char*) | Создание строки |
| str_append | void (*)(String*, const char*) | Добавление текста |
| str_split | String** (*)(const String*, char, size_t*) | Разделение строки |
### Код программы
include/strlib.h:
```c
#ifndef STRLIB_H
#define STRLIB_H

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} String;

String* str_new(const char *init);
void str_append(String *s, const char *text);
void str_replace(String *s, const char *old_str, const char *new_str);
String** str_split(const String *s, char delimiter, size_t *count);
String* str_join(String **parts, size_t count, const char *separator);
String* str_reverse(const String *s);

#endif
```
src/strlib_core.c:
```c
#include <stdlib.h>
#include <string.h>
#include "strlib.h"

String* str_new(const char *init) {
    String *s = malloc(sizeof(String));
    s->length = strlen(init);
    s->capacity = s->length + 16;
    s->data = malloc(s->capacity + 1);
    strcpy(s->data, init);
    return s;
}

void str_append(String *s, const char *text) {
    size_t len = strlen(text);
    s->data = realloc(s->data, s->length + len + 1);
    strcpy(s->data + s->length, text);
    s->length += len;
}
```
src/strlib_search.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strlib.h"

// Поиск подстроки (первое вхождение)
int str_find(const String *s, const char *substr) {
    if (s == NULL || substr == NULL) return -1;
    
    char *pos = strstr(s->data, substr);
    if (pos == NULL) return -1;
    
    return pos - s->data;
}

// Поиск подстроки (последнее вхождение)
int str_find_last(const String *s, const char *substr) {
    if (s == NULL || substr == NULL) return -1;
    
    int last_pos = -1;
    char *pos = s->data;
    size_t sub_len = strlen(substr);
    
    while ((pos = strstr(pos, substr)) != NULL) {
        last_pos = pos - s->data;
        pos += sub_len;
    }
    
    return last_pos;
}

// Поиск символа
int str_find_char(const String *s, char c) {
    if (s == NULL) return -1;
    
    char *pos = strchr(s->data, c);
    if (pos == NULL) return -1;
    
    return pos - s->data;
}

// Проверка наличия подстроки
bool str_contains(const String *s, const char *substr) {
    return str_find(s, substr) != -1;
}

// Проверка начала строки
bool str_starts_with(const String *s, const char *prefix) {
    if (s == NULL || prefix == NULL) return false;
    
    size_t prefix_len = strlen(prefix);
    if (prefix_len > s->length) return false;
    
    return strncmp(s->data, prefix, prefix_len) == 0;
}

// Проверка окончания строки
bool str_ends_with(const String *s, const char *suffix) {
    if (s == NULL || suffix == NULL) return false;
    
    size_t suffix_len = strlen(suffix);
    if (suffix_len > s->length) return false;
    
    return strcmp(s->data + s->length - suffix_len, suffix) == 0;
}

// Количество вхождений подстроки
size_t str_count(const String *s, const char *substr) {
    if (s == NULL || substr == NULL) return 0;
    
    size_t count = 0;
    char *pos = s->data;
    size_t sub_len = strlen(substr);
    
    while ((pos = strstr(pos, substr)) != NULL) {
        count++;
        pos += sub_len;
    }
    
    return count;
}

// Замена подстроки
void str_replace(String *s, const char *old_str, const char *new_str) {
    if (s == NULL || old_str == NULL || new_str == NULL) return;
    
    String *result = str_new("");
    if (result == NULL) return;
    
    char *pos = s->data;
    char *found;
    size_t old_len = strlen(old_str);
    
    while ((found = strstr(pos, old_str)) != NULL) {
        // Добавляем текст до найденной подстроки
        str_append(result, pos);
        result->length -= strlen(pos);
        str_append(result, new_str);
        pos = found + old_len;
    }
    
    str_append(result, pos);
    
    // Заменяем содержимое
    str_clear(s);
    str_append(s, result->data);
    str_free(result);
}
```
src/strlib_format.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "strlib.h"

// Преобразование в верхний регистр
String* str_to_upper(const String *s) {
    if (s == NULL) return NULL;
    
    String *result = str_copy(s);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < result->length; i++) {
        result->data[i] = toupper(result->data[i]);
    }
    
    return result;
}

// Преобразование в нижний регистр
String* str_to_lower(const String *s) {
    if (s == NULL) return NULL;
    
    String *result = str_copy(s);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < result->length; i++) {
        result->data[i] = tolower(result->data[i]);
    }
    
    return result;
}

// Удаление пробелов с обоих концов
String* str_trim(const String *s) {
    if (s == NULL) return NULL;
    
    size_t start = 0;
    size_t end = s->length;
    
    while (start < end && isspace(s->data[start])) start++;
    while (end > start && isspace(s->data[end - 1])) end--;
    
    if (start == 0 && end == s->length) {
        return str_copy(s);
    }
    
    char *sub = malloc(end - start + 1);
    memcpy(sub, s->data + start, end - start);
    sub[end - start] = '\0';
    
    String *result = str_new(sub);
    free(sub);
    
    return result;
}

// Удаление пробелов слева
String* str_trim_left(const String *s) {
    if (s == NULL) return NULL;
    
    size_t start = 0;
    while (start < s->length && isspace(s->data[start])) start++;
    
    return str_substring(s, start, s->length - start);
}

// Удаление пробелов справа
String* str_trim_right(const String *s) {
    if (s == NULL) return NULL;
    
    size_t end = s->length;
    while (end > 0 && isspace(s->data[end - 1])) end--;
    
    return str_substring(s, 0, end);
}

// Разделение строки на части
String** str_split(const String *s, char delimiter, size_t *count) {
    if (s == NULL || count == NULL) return NULL;
    
    *count = 0;
    String **parts = NULL;
    size_t start = 0;
    
    for (size_t i = 0; i <= s->length; i++) {
        if (i == s->length || s->data[i] == delimiter) {
            if (i > start) {
                parts = realloc(parts, (*count + 1) * sizeof(String*));
                char *sub = malloc(i - start + 1);
                memcpy(sub, s->data + start, i - start);
                sub[i - start] = '\0';
                parts[*count] = str_new(sub);
                free(sub);
                (*count)++;
            }
            start = i + 1;
        }
    }
    
    return parts;
}

// Объединение частей в строку
String* str_join(String **parts, size_t count, const char *separator) {
    if (parts == NULL || count == 0) return str_new("");
    
    String *result = str_copy(parts[0]);
    
    for (size_t i = 1; i < count; i++) {
        str_append(result, separator);
        str_append(result, parts[i]->data);
    }
    
    return result;
}

// Извлечение подстроки
String* str_substring(const String *s, size_t start, size_t len) {
    if (s == NULL) return NULL;
    if (start >= s->length) return str_new("");
    
    if (start + len > s->length) {
        len = s->length - start;
    }
    
    char *sub = malloc(len + 1);
    memcpy(sub, s->data + start, len);
    sub[len] = '\0';
    
    String *result = str_new(sub);
    free(sub);
    
    return result;
}

// Повторение текста
String* str_repeat(const char *text, size_t times) {
    if (text == NULL || times == 0) return str_new("");
    
    size_t text_len = strlen(text);
    size_t total_len = text_len * times;
    
    char *result_str = malloc(total_len + 1);
    result_str[0] = '\0';
    
    for (size_t i = 0; i < times; i++) {
        strcat(result_str, text);
    }
    
    String *result = str_new(result_str);
    free(result_str);
    
    return result;
}

// Переворот строки
String* str_reverse(const String *s) {
    if (s == NULL) return NULL;
    
    String *result = str_new_capacity(s->length);
    
    for (int i = s->length - 1; i >= 0; i--) {
        str_append_char(result, s->data[i]);
    }
    
    return result;
}

// Сравнение строк
int str_compare(const String *a, const String *b) {
    if (a == NULL && b == NULL) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;
    return strcmp(a->data, b->data);
}

// Проверка на равенство
bool str_equals(const String *a, const String *b) {
    if (a == NULL && b == NULL) return true;
    if (a == NULL || b == NULL) return false;
    return strcmp(a->data, b->data) == 0;
}

// Проверка на равенство (без учёта регистра)
bool str_equals_ignore_case(const String *a, const String *b) {
    if (a == NULL && b == NULL) return true;
    if (a == NULL || b == NULL) return false;
    return strcasecmp(a->data, b->data) == 0;
}

// Освобождение массива строк
void str_free_array(String **arr, size_t count) {
    if (arr == NULL) return;
    
    for (size_t i = 0; i < count; i++) {
        str_free(arr[i]);
    }
    free(arr);
}
```
src/main.c:
```c
#include <stdio.h>
#include "strlib.h"

int main(void) {
    printf("=== Библиотека строк (strlib) ===\n\n");
    
    // Создание строк
    String *s = str_new("Hello");
    printf("Создана строка: ");
    str_println(s);
    
    // Добавление
    str_append(s, ", World!");
    printf("После str_append: ");
    str_println(s);
    
    // Длина
    printf("Длина строки: %zu\n", str_length(s));
    
    // Поиск
    int pos = str_find(s, "World");
    printf("Позиция 'World': %d\n", pos);
    
    // Замена
    str_replace(s, "World", "C Programmer");
    printf("После замены: ");
    str_println(s);
    
    // Регистр
    String *upper = str_to_upper(s);
    printf("Верхний регистр: ");
    str_println(upper);
    
    // Обрезка пробелов
    String *trimmed = str_new("   Hello   ");
    printf("До обрезки: '");
    str_print(trimmed);
    printf("'\n");
    
    String *trim_result = str_trim(trimmed);
    printf("После обрезки: '");
    str_println(trim_result);
    
    // Разделение строки
    String *csv = str_new("apple,banana,cherry,date");
    size_t count;
    String **parts = str_split(csv, ',', &count);
    printf("Разделение строки: ");
    for (size_t i = 0; i < count; i++) {
        printf("[%s]", parts[i]->data);
        if (i < count - 1) printf(", ");
    }
    printf("\n");
    
    // Объединение
    String *joined = str_join(parts, count, " | ");
    printf("Объединение: ");
    str_println(joined);
    
    // Переворот строки
    String *reversed = str_reverse(s);
    printf("Переворот строки: ");
    str_println(reversed);
    
    // Сравнение
    String *s1 = str_new("Hello");
    String *s2 = str_new("hello");
    printf("Сравнение 'Hello' и 'hello' (с учётом регистра): %d\n", str_compare(s1, s2));
    printf("Сравнение 'Hello' и 'hello' (без учёта регистра): %d\n", str_equals_ignore_case(s1, s2));
    
    // Освобождение памяти
    str_free(s);
    str_free(upper);
    str_free(trimmed);
    str_free(trim_result);
    str_free(csv);
    str_free_array(parts, count);
    str_free(joined);
    str_free(reversed);
    str_free(s1);
    str_free(s2);
    
    return 0;
}
```
Makefile:
```c
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -I include
CFLAGS_DEBUG = -g -O0
CFLAGS_RELEASE = -O2 -DNDEBUG

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LIB_OBJS = $(filter-out $(OBJ_DIR)/main.o,$(OBJS))
TARGET = $(BIN_DIR)/strlib_demo
LIB = $(BIN_DIR)/libstrlib.a

.PHONY: all clean lib demo test

all: lib demo

lib: $(LIB)

demo: $(TARGET)

$(LIB): $(LIB_OBJS) | $(BIN_DIR)
	ar rcs $@ $^

$(TARGET): $(OBJ_DIR)/main.o $(LIB) | $(BIN_DIR)
	$(CC) $< -L$(BIN_DIR) -lstrlib -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

test: $(BIN_DIR)/test_strlib
	./$(BIN_DIR)/test_strlib

$(BIN_DIR)/test_strlib: tests/test_strlib.c $(LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $< -L$(BIN_DIR) -lstrlib -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
```
tests/test_strlib.c:
```c
#include <stdio.h>
#include "strlib.h"

static int _test_failed = 0;

#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            printf("  FAILED: %s:%d\n", __FILE__, __LINE__); \
            _test_failed = 1; \
        } \
    } while(0)

#define ASSERT_STR_EQ(a, b) \
    do { \
        if (strcmp((a), (b)) != 0) { \
            printf("  FAILED: %s:%d: \"%s\" != \"%s\"\n", __FILE__, __LINE__, (a), (b)); \
            _test_failed = 1; \
        } \
    } while(0)

void test_create(void) {
    printf("  test_create: ");
    String *s = str_new("Hello");
    ASSERT_STR_EQ(s->data, "Hello");
    ASSERT_EQ(s->length, 5);
    str_free(s);
    printf("PASSED\n");
}

void test_append(void) {
    printf("  test_append: ");
    String *s = str_new("Hello");
    str_append(s, ", World!");
    ASSERT_STR_EQ(s->data, "Hello, World!");
    str_free(s);
    printf("PASSED\n");
}

void test_find(void) {
    printf("  test_find: ");
    String *s = str_new("Hello, World!");
    int pos = str_find(s, "World");
    ASSERT_EQ(pos, 7);
    str_free(s);
    printf("PASSED\n");
}

void test_replace(void) {
    printf("  test_replace: ");
    String *s = str_new("Hello, World!");
    str_replace(s, "World", "C Programmer");
    ASSERT_STR_EQ(s->data, "Hello, C Programmer!");
    str_free(s);
    printf("PASSED\n");
}

void test_trim(void) {
    printf("  test_trim: ");
    String *s = str_new("   Hello   ");
    String *t = str_trim(s);
    ASSERT_STR_EQ(t->data, "Hello");
    str_free(s);
    str_free(t);
    printf("PASSED\n");
}

void test_split_join(void) {
    printf("  test_split_join: ");
    String *s = str_new("a,b,c,d");
    size_t count;
    String **parts = str_split(s, ',', &count);
    ASSERT_EQ(count, 4);
    ASSERT_STR_EQ(parts[0]->data, "a");
    ASSERT_STR_EQ(parts[1]->data, "b");
    ASSERT_STR_EQ(parts[2]->data, "c");
    ASSERT_STR_EQ(parts[3]->data, "d");
    
    String *joined = str_join(parts, count, "-");
    ASSERT_STR_EQ(joined->data, "a-b-c-d");
    
    str_free(s);
    str_free_array(parts, count);
    str_free(joined);
    printf("PASSED\n");
}

int main(void) {
    printf("\n=== Тестирование strlib ===\n\n");
    
    test_create();
    test_append();
    test_find();
    test_replace();
    test_trim();
    test_split_join();
    
    printf("\n=== Результат ===\n");
    if (_test_failed) {
        printf("❌ Некоторые тесты не пройдены!\n");
        return 1;
    } else {
        printf("✅ Все тесты пройдены успешно!\n");
        return 0;
    }
}
```
---
## Задание 8.3: Утилиты для работы с файлами
### Постановка задачи
Создайте библиотеку файловых утилит.
### Структура проекта
fileutils/
├── include/
│   ├── fileutils.h
│   ├── path.h          # Работа с путями
│   └── io.h            # Ввод-вывод
├── src/
│   ├── path.c
│   ├── io.c
│   └── main.c
├── Makefile
└── meson.build
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| path_join | char* (*)(const char*, const char*) | Объединение путей |
| file_read_all | char* (*)(const char*, size_t*) | Чтение файла |
| file_copy | int (*)(const char*, const char*) | Копирование файла |
### Код программы
include/path.h:
```c
#ifndef PATH_H
#define PATH_H

#include <stdbool.h>
#include <stddef.h>

// Разделитель пути для разных ОС
#ifdef _WIN32
    #define PATH_SEP '\\'
#else
    #define PATH_SEP '/'
#endif

// Функции для работы с путями
char* path_join(const char *base, const char *name);
char* path_dirname(const char *path);
char* path_basename(const char *path);
char* path_extension(const char *path);
char* path_without_extension(const char *path);
char* path_normalize(const char *path);
bool path_is_absolute(const char *path);
bool path_exists(const char *path);
bool path_is_file(const char *path);
bool path_is_directory(const char *path);

#endif // PATH_H
```
include/io.h:
```c
#ifndef IO_H
#define IO_H

#include <stddef.h>

// Чтение файла целиком
char* file_read_all(const char *filename, size_t *size);

// Запись в файл
int file_write_all(const char *filename, const char *content);
int file_append(const char *filename, const char *content);

// Построчное чтение
char** file_read_lines(const char *filename, size_t *count);
void file_free_lines(char **lines, size_t count);

// Копирование и перемещение
int file_copy(const char *src, const char *dst);
int file_move(const char *src, const char *dst);
int file_delete(const char *path);

// Информация о файле
long file_size(const char *filename);
int file_exists(const char *filename);

#endif // IO_H
```
include/fileutils.h:
```c
#ifndef FILEUTILS_H
#define FILEUTILS_H

#include "path.h"
#include "io.h"

#endif // FILEUTILS_H
```
src/path.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "path.h"

// Объединение путей
char* path_join(const char *base, const char *name) {
    if (base == NULL || name == NULL) return NULL;
    
    size_t base_len = strlen(base);
    size_t name_len = strlen(name);
    
    // Проверяем, нужно ли добавлять разделитель
    int need_sep = 0;
    if (base_len > 0 && base[base_len - 1] != PATH_SEP && name[0] != PATH_SEP) {
        need_sep = 1;
    }
    
    size_t result_len = base_len + name_len + need_sep + 1;
    char *result = malloc(result_len);
    
    strcpy(result, base);
    if (need_sep) {
        result[base_len] = PATH_SEP;
        result[base_len + 1] = '\0';
        strcat(result, name);
    } else {
        strcat(result, name);
    }
    
    return result;
}

// Директория пути (удаляет последний компонент)
char* path_dirname(const char *path) {
    if (path == NULL) return NULL;
    
    char *result = strdup(path);
    char *last_sep = strrchr(result, PATH_SEP);
    
    if (last_sep == NULL) {
        strcpy(result, ".");
    } else {
        *last_sep = '\0';
        if (last_sep == result) {
            strcpy(result, "/");
        }
    }
    
    return result;
}

// Имя файла (последний компонент пути)
char* path_basename(const char *path) {
    if (path == NULL) return NULL;
    
    const char *last_sep = strrchr(path, PATH_SEP);
    if (last_sep == NULL) {
        return strdup(path);
    } else {
        return strdup(last_sep + 1);
    }
}

// Расширение файла
char* path_extension(const char *path) {
    if (path == NULL) return NULL;
    
    char *basename = path_basename(path);
    char *last_dot = strrchr(basename, '.');
    
    if (last_dot == NULL || last_dot == basename) {
        free(basename);
        return strdup("");
    }
    
    char *ext = strdup(last_dot + 1);
    free(basename);
    return ext;
}

// Путь без расширения
char* path_without_extension(const char *path) {
    if (path == NULL) return NULL;
    
    char *result = strdup(path);
    char *last_dot = strrchr(result, '.');
    
    if (last_dot != NULL && last_dot != result) {
        *last_dot = '\0';
    }
    
    return result;
}

// Нормализация пути (удаление лишних разделителей)
char* path_normalize(const char *path) {
    if (path == NULL) return NULL;
    
    size_t len = strlen(path);
    char *result = malloc(len + 2);
    size_t pos = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (path[i] == PATH_SEP && pos > 0 && result[pos - 1] == PATH_SEP) {
            continue;
        }
        result[pos++] = path[i];
    }
    
    if (pos > 1 && result[pos - 1] == PATH_SEP) {
        pos--;
    }
    
    result[pos] = '\0';
    return result;
}

// Проверка, является ли путь абсолютным
bool path_is_absolute(const char *path) {
    if (path == NULL) return false;
    
#ifdef _WIN32
    return (path[0] != '\0' && path[1] == ':' && path[2] == '\\') ||
           (path[0] == '\\' && path[1] == '\\');
#else
    return path[0] == '/';
#endif
}

// Проверка существования пути
bool path_exists(const char *path) {
    if (path == NULL) return false;
    
    struct stat st;
    return stat(path, &st) == 0;
}

// Проверка, является ли путь файлом
bool path_is_file(const char *path) {
    if (path == NULL) return false;
    
    struct stat st;
    if (stat(path, &st) != 0) return false;
    return S_ISREG(st.st_mode);
}

// Проверка, является ли путь директорией
bool path_is_directory(const char *path) {
    if (path == NULL) return false;
    
    struct stat st;
    if (stat(path, &st) != 0) return false;
    return S_ISDIR(st.st_mode);
}
```
src/io.c:
```c
#include "../include/io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif

char* file_read_all(const char *filename, size_t *size) {
    if (!filename) return NULL;
    
    FILE *file = fopen(filename, "rb");
    if (!file) return NULL;
    
    // Получаем размер файла
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (file_size < 0) {
        fclose(file);
        return NULL;
    }
    
    char *buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }
    
    size_t read_size = fread(buffer, 1, file_size, file);
    fclose(file);
    
    if (read_size != (size_t)file_size) {
        free(buffer);
        return NULL;
    }
    
    buffer[file_size] = '\0';
    
    if (size) *size = file_size;
    return buffer;
}

int file_write_all(const char *filename, const char *content) {
    if (!filename || !content) return -1;
    
    FILE *file = fopen(filename, "w");
    if (!file) return -1;
    
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, file);
    fclose(file);
    
    return (written == len) ? 0 : -1;
}

int file_append(const char *filename, const char *content) {
    if (!filename || !content) return -1;
    
    FILE *file = fopen(filename, "a");
    if (!file) return -1;
    
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, file);
    fclose(file);
    
    return (written == len) ? 0 : -1;
}

char** file_read_lines(const char *filename, size_t *count) {
    if (!filename || !count) return NULL;
    
    char *content = file_read_all(filename, NULL);
    if (!content) return NULL;
    
    // Подсчитываем количество строк
    size_t line_count = 0;
    for (char *p = content; *p; p++) {
        if (*p == '\n') line_count++;
    }
    if (line_count == 0 && content[0]) line_count = 1;
    
    char **lines = (char**)malloc(sizeof(char*) * (line_count + 1));
    if (!lines) {
        free(content);
        return NULL;
    }
    
    size_t idx = 0;
    char *line = strtok(content, "\n");
    while (line && idx < line_count) {
        lines[idx] = strdup(line);
        if (!lines[idx]) {
            for (size_t i = 0; i < idx; i++) free(lines[i]);
            free(lines);
            free(content);
            return NULL;
        }
        idx++;
        line = strtok(NULL, "\n");
    }
    
    lines[idx] = NULL;
    *count = idx;
    free(content);
    
    return lines;
}

void file_free_lines(char **lines, size_t count) {
    if (!lines) return;
    
    for (size_t i = 0; i < count; i++) {
        free(lines[i]);
    }
    free(lines);
}

int file_copy(const char *src, const char *dst) {
    if (!src || !dst) return -1;
    
    FILE *src_file = fopen(src, "rb");
    if (!src_file) return -1;
    
    FILE *dst_file = fopen(dst, "wb");
    if (!dst_file) {
        fclose(src_file);
        return -1;
    }
    
    char buffer[8192];
    size_t bytes;
    int result = 0;
    
    while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
        if (fwrite(buffer, 1, bytes, dst_file) != bytes) {
            result = -1;
            break;
        }
    }
    
    fclose(src_file);
    fclose(dst_file);
    
    return result;
}

int file_move(const char *src, const char *dst) {
    if (!src || !dst) return -1;
    
#ifdef _WIN32
    if (MoveFile(src, dst)) return 0;
#else
    if (rename(src, dst) == 0) return 0;
#endif
    
    // Если rename не сработал, пробуем копировать и удалить
    if (file_copy(src, dst) == 0) {
        if (file_delete(src) == 0) return 0;
    }
    
    return -1;
}

int file_delete(const char *path) {
    if (!path) return -1;
    
#ifdef _WIN32
    return (DeleteFile(path) != 0) ? 0 : -1;
#else
    return (unlink(path) == 0) ? 0 : -1;
#endif
}

long file_size(const char *filename) {
    if (!filename) return -1;
    
    FILE *file = fopen(filename, "rb");
    if (!file) return -1;
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    
    return size;
}

int file_exists(const char *filename) {
    if (!filename) return 0;
    
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}
```
src/main.c:
```c
#include "../include/fileutils.h"
#include <stdio.h>

int main() {
    printf("=== Тестирование библиотеки файловых утилит ===\n\n");
    
    // Тестирование path функций
    printf("--- Тестирование path функций ---\n");
    char *joined = path_join("/home/user", "file.txt");
    printf("path_join: %s\n", joined);
    free(joined);
    
    char *dirname = path_dirname("/home/user/file.txt");
    printf("path_dirname: %s\n", dirname);
    free(dirname);
    
    char *basename = path_basename("/home/user/file.txt");
    printf("path_basename: %s\n", basename);
    free(basename);
    
    char *ext = path_extension("archive.tar.gz");
    printf("path_extension: %s\n", ext);
    free(ext);
    
    // Создание тестового файла
    printf("\n--- Тестирование io функций ---\n");
    const char *test_file = "test.txt";
    
    if (file_write_all(test_file, "Hello\nWorld\nTest") == 0) {
        printf("Файл создан успешно\n");
        
        // Чтение файла
        size_t size;
        char *content = file_read_all(test_file, &size);
        if (content) {
            printf("Содержимое файла (%zu байт):\n%s\n", size, content);
            free(content);
        }
        
        // Построчное чтение
        size_t line_count;
        char **lines = file_read_lines(test_file, &line_count);
        if (lines) {
            printf("Строк в файле: %zu\n", line_count);
            for (size_t i = 0; i < line_count; i++) {
                printf("Строка %zu: %s\n", i, lines[i]);
            }
            file_free_lines(lines, line_count);
        }
        
        // Дописывание в файл
        file_append(test_file, "\nДополнительная строка");
        
        // Информация о файле
        printf("Размер файла: %ld байт\n", file_size(test_file));
        printf("Файл существует: %s\n", file_exists(test_file) ? "да" : "нет");
        
        // Проверка типа
        printf("Это файл: %s\n", path_is_file(test_file) ? "да" : "нет");
        printf("Это директория: %s\n", path_is_directory(test_file) ? "да" : "нет");
        
        // Копирование
        if (file_copy(test_file, "test_copy.txt") == 0) {
            printf("Файл скопирован успешно\n");
            file_delete("test_copy.txt");
        }
        
        // Удаление тестового файла
        file_delete(test_file);
    }
    
    return 0;
}
```
Makefile:
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS = 

SRCS = src/main.c src/path.c src/io.c
OBJS = $(SRCS:.c=.o)
TARGET = fileutils

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) test.txt test_copy.txt

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

test: $(TARGET)
	./$(TARGET)
```
meson.build:
```c
project('fileutils', 'c',
    version: '1.0.0',
    default_options: ['c_std=c11'])

inc = include_directories('include')

sources = [
    'src/main.c',
    'src/path.c',
    'src/io.c'
]

executable('fileutils',
    sources,
    include_directories: inc,
    install: true)

# Для использования как библиотеки
lib_sources = [
    'src/path.c',
    'src/io.c'
]

fileutils_lib = static_library('fileutils',
    lib_sources,
    include_directories: inc,
    install: true)

fileutils_dep = declare_dependency(
    include_directories: inc,
    link_with: fileutils_lib)
```

---
## Задание 8.4: Простой HTTP-парсер
### Постановка задачи
Создайте библиотеку для парсинга HTTP запросов/ответов.
### Структура проекта
httplib/
├── include/
│   ├── http_request.h
│   └── http_response.h
│   └── http.h
├── src/
│   ├── http_request.c
│   ├── http_response.c
│   └── main.c
├── Makefile
└── meson.build
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| HttpMethod | enum | HTTP методы |
| HttpRequest | struct | HTTP запрос |
| HttpResponse | struct | HTTP ответ |
| http_request_parse | HttpRequest* (*)(const char*) | Парсинг запроса |
### Код программы
include/http_request.h:
```c
#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <stddef.h>

typedef enum {
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_PATCH,
    HTTP_UNKNOWN
} HttpMethod;

typedef struct {
    char name[64];
    char value[256];
} HttpHeader;

typedef struct {
    HttpMethod method;
    char path[256];
    char version[16];
    HttpHeader headers[50];
    int header_count;
    char *body;
    size_t body_length;
    char *query_string;  // Параметры запроса
} HttpRequest;

// Парсинг
HttpRequest* http_request_parse(const char *raw);
void http_request_free(HttpRequest *req);

// Создание
HttpRequest* http_request_create(HttpMethod method, const char *path);
void http_request_add_header(HttpRequest *req, const char *name, const char *value);
void http_request_set_body(HttpRequest *req, const char *body);
void http_request_set_body_binary(HttpRequest *req, const char *body, size_t length);

// Сериализация
char* http_request_to_string(const HttpRequest *req);

// Утилиты
const char* http_method_to_string(HttpMethod method);
HttpMethod http_method_from_string(const char *str);
const char* http_request_get_header(const HttpRequest *req, const char *name);

// Дополнительные утилиты
void http_request_remove_header(HttpRequest *req, const char *name);
void http_request_clear_headers(HttpRequest *req);
char* http_request_get_query_param(const HttpRequest *req, const char *key);

#endif // HTTP_REQUEST_H
```
src/http_request.c:
```c
#include "../include/http_request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const char* method_strings[] = {
    "GET", "POST", "PUT", "DELETE", "HEAD", "OPTIONS", "PATCH", NULL
};

const char* http_method_to_string(HttpMethod method) {
    if (method >= HTTP_GET && method <= HTTP_PATCH) {
        return method_strings[method];
    }
    return "UNKNOWN";
}

HttpMethod http_method_from_string(const char *str) {
    for (int i = 0; method_strings[i] != NULL; i++) {
        if (strcasecmp(str, method_strings[i]) == 0) {
            return (HttpMethod)i;
        }
    }
    return HTTP_UNKNOWN;
}

HttpRequest* http_request_create(HttpMethod method, const char *path) {
    HttpRequest *req = (HttpRequest*)calloc(1, sizeof(HttpRequest));
    if (!req) return NULL;
    
    req->method = method;
    strncpy(req->path, path, 255);
    req->path[255] = '\0';
    strcpy(req->version, "HTTP/1.1");
    req->header_count = 0;
    req->body = NULL;
    req->body_length = 0;
    req->query_string = NULL;
    
    // Добавляем стандартные заголовки
    http_request_add_header(req, "Host", "localhost");
    http_request_add_header(req, "Connection", "close");
    http_request_add_header(req, "User-Agent", "HTTPLib/1.0");
    
    return req;
}

void http_request_add_header(HttpRequest *req, const char *name, const char *value) {
    if (!req || req->header_count >= 50) return;
    
    strncpy(req->headers[req->header_count].name, name, 63);
    req->headers[req->header_count].name[63] = '\0';
    strncpy(req->headers[req->header_count].value, value, 255);
    req->headers[req->header_count].value[255] = '\0';
    req->header_count++;
}

void http_request_set_body(HttpRequest *req, const char *body) {
    if (!req) return;
    
    if (req->body) {
        free(req->body);
    }
    
    if (body) {
        req->body_length = strlen(body);
        req->body = (char*)malloc(req->body_length + 1);
        if (req->body) {
            strcpy(req->body, body);
            // Обновляем Content-Length
            char content_length[32];
            snprintf(content_length, sizeof(content_length), "%zu", req->body_length);
            http_request_add_header(req, "Content-Length", content_length);
        }
    } else {
        req->body = NULL;
        req->body_length = 0;
    }
}

void http_request_set_body_binary(HttpRequest *req, const char *body, size_t length) {
    if (!req) return;
    
    if (req->body) {
        free(req->body);
    }
    
    if (body && length > 0) {
        req->body = (char*)malloc(length);
        if (req->body) {
            memcpy(req->body, body, length);
            req->body_length = length;
            char content_length[32];
            snprintf(content_length, sizeof(content_length), "%zu", req->body_length);
            http_request_add_header(req, "Content-Length", content_length);
        }
    } else {
        req->body = NULL;
        req->body_length = 0;
    }
}

char* http_request_to_string(const HttpRequest *req) {
    if (!req) return NULL;
    
    // Начальный размер буфера
    size_t buffer_size = 4096;
    char *buffer = (char*)malloc(buffer_size);
    if (!buffer) return NULL;
    
    int offset = 0;
    
    // Записываем стартовую строку
    offset += snprintf(buffer + offset, buffer_size - offset, 
                      "%s %s %s\r\n",
                      http_method_to_string(req->method),
                      req->path,
                      req->version);
    
    // Записываем заголовки
    for (int i = 0; i < req->header_count; i++) {
        offset += snprintf(buffer + offset, buffer_size - offset,
                          "%s: %s\r\n",
                          req->headers[i].name,
                          req->headers[i].value);
    }
    
    // Завершаем заголовки
    offset += snprintf(buffer + offset, buffer_size - offset, "\r\n");
    
    // Записываем тело
    if (req->body && req->body_length > 0) {
        if (offset + req->body_length >= buffer_size) {
            buffer_size = offset + req->body_length + 1024;
            buffer = (char*)realloc(buffer, buffer_size);
            if (!buffer) return NULL;
        }
        memcpy(buffer + offset, req->body, req->body_length);
        offset += req->body_length;
    }
    
    buffer[offset] = '\0';
    return buffer;
}

HttpRequest* http_request_parse(const char *raw) {
    if (!raw) return NULL;
    
    HttpRequest *req = (HttpRequest*)calloc(1, sizeof(HttpRequest));
    if (!req) return NULL;
    
    char *raw_copy = strdup(raw);
    if (!raw_copy) {
        free(req);
        return NULL;
    }
    
    // Парсим стартовую строку
    char *line = strtok(raw_copy, "\r\n");
    if (!line) {
        free(raw_copy);
        free(req);
        return NULL;
    }
    
    char method_str[32], path[256], version[16];
    if (sscanf(line, "%s %s %s", method_str, path, version) != 3) {
        free(raw_copy);
        free(req);
        return NULL;
    }
    
    req->method = http_method_from_string(method_str);
    strncpy(req->path, path, 255);
    strncpy(req->version, version, 15);
    
    // Извлекаем query string
    char *query_start = strchr(req->path, '?');
    if (query_start) {
        *query_start = '\0';
        req->query_string = strdup(query_start + 1);
    }
    
    // Парсим заголовки
    req->header_count = 0;
    char *header_line;
    while ((header_line = strtok(NULL, "\r\n")) != NULL && strlen(header_line) > 0) {
        char *colon = strchr(header_line, ':');
        if (colon && req->header_count < 50) {
            *colon = '\0';
            char *name = header_line;
            char *value = colon + 1;
            
            // Пропускаем пробелы в начале значения
            while (isspace(*value)) value++;
            // Удаляем пробелы в конце значения
            char *end = value + strlen(value) - 1;
            while (end > value && isspace(*end)) end--;
            *(end + 1) = '\0';
            
            http_request_add_header(req, name, value);
        }
    }
    
    // Парсим тело
    char *body_start = strstr(raw, "\r\n\r\n");
    if (body_start) {
        body_start += 4;
        size_t body_len = strlen(body_start);
        
        // Проверяем Content-Length
        const char *content_length = http_request_get_header(req, "Content-Length");
        if (content_length) {
            req->body_length = atoi(content_length);
            if (req->body_length > 0 && req->body_length <= body_len) {
                req->body = (char*)malloc(req->body_length + 1);
                if (req->body) {
                    memcpy(req->body, body_start, req->body_length);
                    req->body[req->body_length] = '\0';
                }
            }
        } else if (strstr(header_line, "Transfer-Encoding: chunked")) {
            // Упрощенная обработка chunked encoding (для демонстрации)
            req->body = strdup(body_start);
            req->body_length = body_len;
        } else if (body_len > 0) {
            req->body = strdup(body_start);
            req->body_length = body_len;
        }
    }
    
    free(raw_copy);
    return req;
}

void http_request_free(HttpRequest *req) {
    if (!req) return;
    
    if (req->body) {
        free(req->body);
    }
    if (req->query_string) {
        free(req->query_string);
    }
    free(req);
}

const char* http_request_get_header(const HttpRequest *req, const char *name) {
    if (!req || !name) return NULL;
    
    for (int i = 0; i < req->header_count; i++) {
        if (strcasecmp(req->headers[i].name, name) == 0) {
            return req->headers[i].value;
        }
    }
    return NULL;
}

void http_request_remove_header(HttpRequest *req, const char *name) {
    if (!req || !name) return;
    
    for (int i = 0; i < req->header_count; i++) {
        if (strcasecmp(req->headers[i].name, name) == 0) {
            for (int j = i; j < req->header_count - 1; j++) {
                req->headers[j] = req->headers[j + 1];
            }
            req->header_count--;
            break;
        }
    }
}

void http_request_clear_headers(HttpRequest *req) {
    if (!req) return;
    req->header_count = 0;
}

char* http_request_get_query_param(const HttpRequest *req, const char *key) {
    if (!req || !req->query_string || !key) return NULL;
    
    char *query = strdup(req->query_string);
    if (!query) return NULL;
    
    char *token = strtok(query, "&");
    while (token) {
        char *eq = strchr(token, '=');
        if (eq) {
            *eq = '\0';
            if (strcmp(token, key) == 0) {
                char *value = strdup(eq + 1);
                free(query);
                return value;
            }
        }
        token = strtok(NULL, "&");
    }
    
    free(query);
    return NULL;
}
```
src/http_response.c:
```c
#include "../include/http_response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int code;
    const char *text;
} StatusCodeMap;

static const StatusCodeMap status_codes[] = {
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {204, "No Content"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {303, "See Other"},
    {304, "Not Modified"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {502, "Bad Gateway"},
    {503, "Service Unavailable"},
    {0, NULL}
};

const char* http_status_code_to_text(int status_code) {
    for (int i = 0; status_codes[i].code != 0; i++) {
        if (status_codes[i].code == status_code) {
            return status_codes[i].text;
        }
    }
    return "Unknown";
}

HttpResponse* http_response_create(int status_code, const char *status_text) {
    HttpResponse *res = (HttpResponse*)calloc(1, sizeof(HttpResponse));
    if (!res) return NULL;
    
    res->status_code = status_code;
    if (status_text) {
        strncpy(res->status_text, status_text, 63);
    } else {
        strncpy(res->status_text, http_status_code_to_text(status_code), 63);
    }
    res->status_text[63] = '\0';
    strcpy(res->version, "HTTP/1.1");
    res->header_count = 0;
    res->body = NULL;
    res->body_length = 0;
    
    // Добавляем стандартные заголовки
    http_response_add_header(res, "Content-Type", "text/plain");
    http_response_add_header(res, "Connection", "close");
    
    return res;
}

void http_response_add_header(HttpResponse *res, const char *name, const char *value) {
    if (!res || res->header_count >= 50) return;
    
    strncpy(res->headers[res->header_count].name, name, 63);
    res->headers[res->header_count].name[63] = '\0';
    strncpy(res->headers[res->header_count].value, value, 255);
    res->headers[res->header_count].value[255] = '\0';
    res->header_count++;
}

void http_response_set_body(HttpResponse *res, const char *body) {
    if (!res) return;
    
    if (res->body) {
        free(res->body);
    }
    
    if (body) {
        res->body_length = strlen(body);
        res->body = (char*)malloc(res->body_length + 1);
        if (res->body) {
            strcpy(res->body, body);
            char content_length[32];
            snprintf(content_length, sizeof(content_length), "%zu", res->body_length);
            http_response_add_header(res, "Content-Length", content_length);
        }
    } else {
        res->body = NULL;
        res->body_length = 0;
    }
}

void http_response_set_body_binary(HttpResponse *res, const char *body, size_t length) {
    if (!res) return;
    
    if (res->body) {
        free(res->body);
    }
    
    if (body && length > 0) {
        res->body = (char*)malloc(length);
        if (res->body) {
            memcpy(res->body, body, length);
            res->body_length = length;
            char content_length[32];
            snprintf(content_length, sizeof(content_length), "%zu", res->body_length);
            http_response_add_header(res, "Content-Length", content_length);
        }
    } else {
        res->body = NULL;
        res->body_length = 0;
    }
}

char* http_response_to_string(const HttpResponse *res) {
    if (!res) return NULL;
    
    size_t buffer_size = 4096;
    char *buffer = (char*)malloc(buffer_size);
    if (!buffer) return NULL;
    
    int offset = 0;
    
    // Стартовая строка
    offset += snprintf(buffer + offset, buffer_size - offset,
                      "%s %d %s\r\n",
                      res->version,
                      res->status_code,
                      res->status_text);
    
    // Заголовки
    for (int i = 0; i < res->header_count; i++) {
        offset += snprintf(buffer + offset, buffer_size - offset,
                          "%s: %s\r\n",
                          res->headers[i].name,
                          res->headers[i].value);
    }
    
    // Разделитель
    offset += snprintf(buffer + offset, buffer_size - offset, "\r\n");
    
    // Тело
    if (res->body && res->body_length > 0) {
        if (offset + res->body_length >= buffer_size) {
            buffer_size = offset + res->body_length + 1024;
            buffer = (char*)realloc(buffer, buffer_size);
            if (!buffer) return NULL;
        }
        memcpy(buffer + offset, res->body, res->body_length);
        offset += res->body_length;
    }
    
    buffer[offset] = '\0';
    return buffer;
}

HttpResponse* http_response_parse(const char *raw) {
    if (!raw) return NULL;
    
    HttpResponse *res = (HttpResponse*)calloc(1, sizeof(HttpResponse));
    if (!res) return NULL;
    
    char *raw_copy = strdup(raw);
    if (!raw_copy) {
        free(res);
        return NULL;
    }
    
    // Парсим стартовую строку
    char *line = strtok(raw_copy, "\r\n");
    if (!line) {
        free(raw_copy);
        free(res);
        return NULL;
    }
    
    char version[16];
    int status_code;
    char status_text[64];
    
    if (sscanf(line, "%s %d %63[^\r\n]", version, &status_code, status_text) != 3) {
        free(raw_copy);
        free(res);
        return NULL;
    }
    
    strncpy(res->version, version, 15);
    res->status_code = status_code;
    strncpy(res->status_text, status_text, 63);
    
    // Парсим заголовки
    res->header_count = 0;
    char *header_line;
    while ((header_line = strtok(NULL, "\r\n")) != NULL && strlen(header_line) > 0) {
        char *colon = strchr(header_line, ':');
        if (colon && res->header_count < 50) {
            *colon = '\0';
            char *name = header_line;
            char *value = colon + 1;
            
            while (isspace(*value)) value++;
            char *end = value + strlen(value) - 1;
            while (end > value && isspace(*end)) end--;
            *(end + 1) = '\0';
            
            http_response_add_header(res, name, value);
        }
    }
    
    // Парсим тело
    char *body_start = strstr(raw, "\r\n\r\n");
    if (body_start) {
        body_start += 4;
        size_t body_len = strlen(body_start);
        
        const char *content_length = http_response_get_header(res, "Content-Length");
        if (content_length) {
            res->body_length = atoi(content_length);
            if (res->body_length > 0 && res->body_length <= body_len) {
                res->body = (char*)malloc(res->body_length + 1);
                if (res->body) {
                    memcpy(res->body, body_start, res->body_length);
                    res->body[res->body_length] = '\0';
                }
            }
        } else if (body_len > 0) {
            res->body = strdup(body_start);
            res->body_length = body_len;
        }
    }
    
    free(raw_copy);
    return res;
}

void http_response_free(HttpResponse *res) {
    if (!res) return;
    
    if (res->body) {
        free(res->body);
    }
    free(res);
}

const char* http_response_get_header(const HttpResponse *res, const char *name) {
    if (!res || !name) return NULL;
    
    for (int i = 0; i < res->header_count; i++) {
        if (strcasecmp(res->headers[i].name, name) == 0) {
            return res->headers[i].value;
        }
    }
    return NULL;
}

void http_response_remove_header(HttpResponse *res, const char *name) {
    if (!res || !name) return;
    
    for (int i = 0; i < res->header_count; i++) {
        if (strcasecmp(res->headers[i].name, name) == 0) {
            for (int j = i; j < res->header_count - 1; j++) {
                res->headers[j] = res->headers[j + 1];
            }
            res->header_count--;
            break;
        }
    }
}

void http_response_clear_headers(HttpResponse *res) {
    if (!res) return;
    res->header_count = 0;
}

HttpResponse* http_response_ok() {
    HttpResponse *res = http_response_create(200, "OK");
    http_response_set_body(res, "OK");
    return res;
}

HttpResponse* http_response_not_found() {
    HttpResponse *res = http_response_create(404, "Not Found");
    http_response_set_body(res, "404 Not Found");
    return res;
}

HttpResponse* http_response_bad_request() {
    HttpResponse *res = http_response_create(400, "Bad Request");
    http_response_set_body(res, "400 Bad Request");
    return res;
}

HttpResponse* http_response_internal_error() {
    HttpResponse *res = http_response_create(500, "Internal Server Error");
    http_response_set_body(res, "500 Internal Server Error");
    return res;
}
```
include/http_responce.h:
```c
#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <stddef.h>

typedef struct {
    char name[64];
    char value[256];
} HttpResponseHeader;

typedef struct {
    int status_code;
    char status_text[64];
    char version[16];
    HttpResponseHeader headers[50];
    int header_count;
    char *body;
    size_t body_length;
} HttpResponse;

// Парсинг
HttpResponse* http_response_parse(const char *raw);
void http_response_free(HttpResponse *res);

// Создание
HttpResponse* http_response_create(int status_code, const char *status_text);
void http_response_add_header(HttpResponse *res, const char *name, const char *value);
void http_response_set_body(HttpResponse *res, const char *body);
void http_response_set_body_binary(HttpResponse *res, const char *body, size_t length);

// Сериализация
char* http_response_to_string(const HttpResponse *res);

// Утилиты
const char* http_response_get_header(const HttpResponse *res, const char *name);
const char* http_status_code_to_text(int status_code);
void http_response_remove_header(HttpResponse *res, const char *name);
void http_response_clear_headers(HttpResponse *res);

// Предустановленные ответы
HttpResponse* http_response_ok();
HttpResponse* http_response_not_found();
HttpResponse* http_response_bad_request();
HttpResponse* http_response_internal_error();

#endif // HTTP_RESPONSE_H
```
include/http.h:
```c
#ifndef HTTP_H
#define HTTP_H

// Главный заголовочный файл библиотеки
#include "http_request.h"
#include "http_response.h"

// Общие HTTP константы
#define HTTP_VERSION "HTTP/1.1"
#define MAX_HEADER_COUNT 50
#define MAX_HEADER_NAME_LEN 64
#define MAX_HEADER_VALUE_LEN 256

// Коды состояния HTTP
#define HTTP_STATUS_OK 200
#define HTTP_STATUS_CREATED 201
#define HTTP_STATUS_ACCEPTED 202
#define HTTP_STATUS_NO_CONTENT 204
#define HTTP_STATUS_MOVED_PERMANENTLY 301
#define HTTP_STATUS_FOUND 302
#define HTTP_STATUS_SEE_OTHER 303
#define HTTP_STATUS_NOT_MODIFIED 304
#define HTTP_STATUS_BAD_REQUEST 400
#define HTTP_STATUS_UNAUTHORIZED 401
#define HTTP_STATUS_FORBIDDEN 403
#define HTTP_STATUS_NOT_FOUND 404
#define HTTP_STATUS_METHOD_NOT_ALLOWED 405
#define HTTP_STATUS_INTERNAL_SERVER_ERROR 500
#define HTTP_STATUS_NOT_IMPLEMENTED 501
#define HTTP_STATUS_BAD_GATEWAY 502
#define HTTP_STATUS_SERVICE_UNAVAILABLE 503

#endif // HTTP_H
```

src/main.c:
```c
#include "../include/http.h"
#include <stdio.h>
#include <stdlib.h>

void print_request(const HttpRequest *req) {
    printf("=== HTTP Request ===\n");
    printf("Method: %s\n", http_method_to_string(req->method));
    printf("Path: %s\n", req->path);
    printf("Version: %s\n", req->version);
    printf("Headers (%d):\n", req->header_count);
    for (int i = 0; i < req->header_count; i++) {
        printf("  %s: %s\n", req->headers[i].name, req->headers[i].value);
    }
    if (req->body && req->body_length > 0) {
        printf("Body (%zu): %s\n", req->body_length, req->body);
    }
    printf("\n");
}

void print_response(const HttpResponse *res) {
    printf("=== HTTP Response ===\n");
    printf("Status: %d %s\n", res->status_code, res->status_text);
    printf("Version: %s\n", res->version);
    printf("Headers (%d):\n", res->header_count);
    for (int i = 0; i < res->header_count; i++) {
        printf("  %s: %s\n", res->headers[i].name, res->headers[i].value);
    }
    if (res->body && res->body_length > 0) {
        printf("Body (%zu): %s\n", res->body_length, res->body);
    }
    printf("\n");
}

int main() {
    printf("=== HTTP Library Test ===\n\n");
    
    // Тестирование создания и парсинга запросов
    printf("--- Testing HTTP Requests ---\n");
    
    // Создание GET запроса
    HttpRequest *get_req = http_request_create(HTTP_GET, "/api/users?id=123");
    http_request_add_header(get_req, "Accept", "application/json");
    http_request_add_header(get_req, "Authorization", "Bearer token123");
    
    char *get_req_str = http_request_to_string(get_req);
    printf("GET Request:\n%s\n", get_req_str);
    
    // Парсинг запроса
    HttpRequest *parsed_req = http_request_parse(get_req_str);
    print_request(parsed_req);
    
    // Получение query параметра
    char *user_id = http_request_get_query_param(parsed_req, "id");
    if (user_id) {
        printf("Query param 'id': %s\n\n", user_id);
        free(user_id);
    }
    
    // Создание POST запроса с телом
    HttpRequest *post_req = http_request_create(HTTP_POST, "/api/users");
    http_request_add_header(post_req, "Content-Type", "application/json");
    http_request_set_body(post_req, "{\"name\":\"John\",\"age\":30}");
    
    char *post_req_str = http_request_to_string(post_req);
    printf("POST Request:\n%s\n", post_req_str);
    
    // Тестирование ответов
    printf("\n--- Testing HTTP Responses ---\n");
    
    // Создание успешного ответа
    HttpResponse *ok_res = http_response_create(200, "OK");
    http_response_add_header(ok_res, "Content-Type", "application/json");
    http_response_set_body(ok_res, "{\"status\":\"success\",\"data\":{\"id\":1}}");
    
    char *ok_res_str = http_response_to_string(ok_res);
    printf("OK Response:\n%s\n", ok_res_str);
    
    // Парсинг ответа
    HttpResponse *parsed_res = http_response_parse(ok_res_str);
    print_response(parsed_res);
    
    // Использование предустановленных ответов
    HttpResponse *not_found = http_response_not_found();
    char *not_found_str = http_response_to_string(not_found);
    printf("404 Response:\n%s\n", not_found_str);
    
    // Тестирование методов модификации
    printf("\n--- Testing Header Manipulation ---\n");
    HttpResponse *res = http_response_create(200, "OK");
    http_response_add_header(res, "X-Custom-Header", "value1");
    http_response_add_header(res, "X-Custom-Header", "value2");
    printf("Before removal:\n");
    for (int i = 0; i < res->header_count; i++) {
        printf("  %s: %s\n", res->headers[i].name, res->headers[i].value);
    }
    
    http_response_remove_header(res, "X-Custom-Header");
    printf("After removal:\n");
    for (int i = 0; i < res->header_count; i++) {
        printf("  %s: %s\n", res->headers[i].name, res->headers[i].value);
    }
    
    // Очистка памяти
    http_request_free(get_req);
    http_request_free(parsed_req);
    http_request_free(post_req);
    http_response_free(ok_res);
    http_response_free(parsed_res);
    http_response_free(not_found);
    http_response_free(res);
    free(get_req_str);
    free(post_req_str);
    free(ok_res_str);
    free(not_found_str);
    
    return 0;
}
```
Makefile:
```c
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS = 

SRCS = src/main.c src/http_request.c src/http_response.c
OBJS = $(SRCS:.c=.o)
TARGET = httplib

.PHONY: all clean install test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

test: $(TARGET)
	./$(TARGET)

# Для использования как статической библиотеки
lib: $(OBJS)
	ar rcs libhttp.a $(filter-out src/main.o, $(OBJS))

# Для использования как динамической библиотеки
shared: CFLAGS += -fPIC
shared: $(filter-out src/main.o, $(SRCS:.c=.o))
	$(CC) -shared -o libhttp.so $^
```
meson.build:
```c
project('httplib', 'c',
    version: '1.0.0',
    default_options: ['c_std=c11'])

inc = include_directories('include')

sources = [
    'src/main.c',
    'src/http_request.c',
    'src/http_response.c'
]

executable('httplib',
    sources,
    include_directories: inc,
    install: true)

# Статическая библиотека
lib_sources = [
    'src/http_request.c',
    'src/http_response.c'
]

http_lib = static_library('http',
    lib_sources,
    include_directories: inc,
    install: true)

http_dep = declare_dependency(
    include_directories: inc,
    link_with: http_lib)

# Для тестов
test('httplib_test', executable('httplib_test', sources, include_directories: inc))
```

---
## Задание 8.5: Мини-тестовый фреймворк
### Постановка задачи
Создайте простой фреймворк для модульного тестирования.
### Структура проекта
minitest/
├── include/
│   └── minitest.h
├── src/
│   ├── minitest.c
│   └── main.c
├── tests/
│   └── test_example.c
├── Makefile
└── meson.build
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| TestSuite | struct | Набор тестов |
| TestCase | struct | Отдельный тест |
| ASSERT_EQ | macro | Проверка равенства |
| ASSERT_STR_EQ | macro | Проверка строк |
### Код программы
include/minitest.h:
```c
#ifndef MINITEST_H
#define MINITEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ========== СТРУКТУРЫ ДЛЯ ТЕСТОВ ==========

typedef void (*TestFunc)(void);

typedef struct {
    const char *name;
    TestFunc func;
} TestCase;

typedef struct {
    const char *name;
    TestCase *tests;
    int test_count;
    int passed;
    int failed;
} TestSuite;

// ========== МАКРОСЫ ДЛЯ УТВЕРЖДЕНИЙ ==========

// Глобальная переменная для отслеживания неудач
extern int _test_failed;

// Базовое утверждение
#define ASSERT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            printf("  ❌ FAILED: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

#define ASSERT_FALSE(cond) ASSERT_TRUE(!(cond))

// Сравнение целых чисел
#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            printf("  ❌ FAILED: %s:%d: %lld != %lld\n", __FILE__, __LINE__, \
                   (long long)(a), (long long)(b)); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

#define ASSERT_NEQ(a, b) \
    do { \
        if ((a) == (b)) { \
            printf("  ❌ FAILED: %s:%d: %lld == %lld\n", __FILE__, __LINE__, \
                   (long long)(a), (long long)(b)); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

// Сравнение строк
#define ASSERT_STR_EQ(a, b) \
    do { \
        if (strcmp((a), (b)) != 0) { \
            printf("  ❌ FAILED: %s:%d: \"%s\" != \"%s\"\n", __FILE__, __LINE__, (a), (b)); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

#define ASSERT_STR_NEQ(a, b) \
    do { \
        if (strcmp((a), (b)) == 0) { \
            printf("  ❌ FAILED: %s:%d: \"%s\" == \"%s\"\n", __FILE__, __LINE__, (a), (b)); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

// Сравнение указателей
#define ASSERT_NULL(ptr) ASSERT_TRUE((ptr) == NULL)
#define ASSERT_NOT_NULL(ptr) ASSERT_TRUE((ptr) != NULL)

// Сравнение чисел с плавающей точкой
#define ASSERT_DOUBLE_EQ(a, b, eps) \
    do { \
        double _diff = (a) - (b); \
        if (_diff < 0) _diff = -_diff; \
        if (_diff > (eps)) { \
            printf("  ❌ FAILED: %s:%d: %f != %f (eps=%f)\n", __FILE__, __LINE__, \
                   (double)(a), (double)(b), (eps)); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

#define ASSERT_FLOAT_EQ(a, b, eps) ASSERT_DOUBLE_EQ(a, b, eps)

// Сравнение массивов
#define ASSERT_ARRAY_EQ(arr1, arr2, len, type) \
    do { \
        for (size_t _i = 0; _i < (len); _i++) { \
            if ((arr1)[_i] != (arr2)[_i]) { \
                printf("  ❌ FAILED: %s:%d: array mismatch at index %zu\n", \
                       __FILE__, __LINE__, _i); \
                _test_failed = 1; \
                return; \
            } \
        } \
    } while(0)

// Проверка на выброс исключения (для C можно использовать флаги)
#define ASSERT_THROWS(expr, expected) \
    do { \
        /* В C нет исключений, этот макрос для совместимости */ \
        (void)(expected); \
        expr; \
    } while(0)

// ========== ФУНКЦИИ ДЛЯ РАБОТЫ С ТЕСТОВЫМИ НАБОРАМИ ==========

// Инициализация тестового набора
void test_suite_init(TestSuite *suite, const char *name);

// Добавление теста в набор
void test_suite_add(TestSuite *suite, const char *name, TestFunc func);

// Запуск всех тестов
void test_suite_run(TestSuite *suite);

// Вывод итоговой статистики
void test_suite_summary(const TestSuite *suite);

// Сброс флага ошибки
void test_reset_failed(void);

// ========== УДОБНЫЕ МАКРОСЫ ==========

// Макрос для добавления теста
#define RUN_TEST(suite, test_func) \
    test_suite_add(&suite, #test_func, test_func)

// Макрос для создания простого теста без набора
#define TEST(name) \
    void name(void); \
    void name(void)

// Макрос для запуска одного теста
#define RUN_SINGLE_TEST(test_func) \
    do { \
        printf("\n=== Запуск теста: %s ===\n", #test_func); \
        _test_failed = 0; \
        test_func(); \
        if (_test_failed) { \
            printf("❌ Тест %s НЕ ПРОЙДЕН\n", #test_func); \
        } else { \
            printf("✅ Тест %s пройден\n", #test_func); \
        } \
    } while(0)

#endif // MINITEST_H
```
src/minitest.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minitest.h"

int _test_failed = 0;

void test_suite_init(TestSuite *suite, const char *name) {
    if (suite == NULL) return;
    
    suite->name = name;
    suite->tests = NULL;
    suite->test_count = 0;
    suite->passed = 0;
    suite->failed = 0;
}

void test_suite_add(TestSuite *suite, const char *name, TestFunc func) {
    if (suite == NULL || name == NULL || func == NULL) return;
    
    suite->tests = realloc(suite->tests, (suite->test_count + 1) * sizeof(TestCase));
    suite->tests[suite->test_count].name = name;
    suite->tests[suite->test_count].func = func;
    suite->test_count++;
}

void test_suite_run(TestSuite *suite) {
    if (suite == NULL) return;
    
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    ЗАПУСК ТЕСТОВ: %-30s ║\n", suite->name);
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    
    for (int i = 0; i < suite->test_count; i++) {
        printf("\n▶ Тест: %s\n", suite->tests[i].name);
        printf("  ");
        
        _test_failed = 0;
        suite->tests[i].func();
        
        if (_test_failed) {
            suite->failed++;
            printf("  ❌ НЕ ПРОЙДЕН\n");
        } else {
            suite->passed++;
            printf("  ✅ ПРОЙДЕН\n");
        }
    }
}

void test_suite_summary(const TestSuite *suite) {
    if (suite == NULL) return;
    
    int total = suite->passed + suite->failed;
    
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    РЕЗУЛЬТАТЫ ТЕСТОВ                          ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║  Набор: %-45s ║\n", suite->name);
    printf("║  ✅ Пройдено: %-38d ║\n", suite->passed);
    printf("║  ❌ Не пройдено: %-35d ║\n", suite->failed);
    printf("║  📊 Всего: %-41d ║\n", total);
    printf("║  📈 Успешность: %-36.1f%% ║\n", 
           total > 0 ? (float)suite->passed / total * 100 : 0);
    printf("╚══════════════════════════════════════════════════════════════╝\n");
}

void test_reset_failed(void) {
    _test_failed = 0;
}
```
src/main.c:
```c
#include <stdio.h>
#include <math.h>
#include "minitest.h"

// ========== ФУНКЦИИ ДЛЯ ТЕСТИРОВАНИЯ ==========

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
double divide(int a, int b) { return b != 0 ? (double)a / b : 0; }
int factorial(int n) {
    if (n < 0) return 0;
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}
int is_even(int n) { return n % 2 == 0; }

// ========== ТЕСТЫ ==========

// Тест для функции сложения
void test_add(void) {
    ASSERT_EQ(add(2, 3), 5);
    ASSERT_EQ(add(-1, 1), 0);
    ASSERT_EQ(add(0, 0), 0);
    ASSERT_EQ(add(-5, -3), -8);
}

// Тест для функции вычитания
void test_subtract(void) {
    ASSERT_EQ(subtract(5, 3), 2);
    ASSERT_EQ(subtract(0, 5), -5);
    ASSERT_EQ(subtract(-2, -3), 1);
}

// Тест для функции умножения
void test_multiply(void) {
    ASSERT_EQ(multiply(2, 3), 6);
    ASSERT_EQ(multiply(-2, 3), -6);
    ASSERT_EQ(multiply(0, 100), 0);
}

// Тест для функции деления
void test_divide(void) {
    ASSERT_DOUBLE_EQ(divide(10, 2), 5.0, 0.0001);
    ASSERT_DOUBLE_EQ(divide(7, 2), 3.5, 0.0001);
    ASSERT_DOUBLE_EQ(divide(5, 0), 0.0, 0.0001);
}

// Тест для факториала
void test_factorial(void) {
    ASSERT_EQ(factorial(0), 1);
    ASSERT_EQ(factorial(1), 1);
    ASSERT_EQ(factorial(5), 120);
    ASSERT_EQ(factorial(10), 3628800);
    ASSERT_EQ(factorial(-5), 0);
}

// Тест для проверки чётности
void test_is_even(void) {
    ASSERT_TRUE(is_even(2));
    ASSERT_TRUE(is_even(0));
    ASSERT_FALSE(is_even(3));
    ASSERT_FALSE(is_even(-1));
}

// Тест со строками
void test_strings(void) {
    ASSERT_STR_EQ("hello", "hello");
    ASSERT_STR_NEQ("hello", "world");
    ASSERT_STR_EQ("", "");
}

// Тест с указателями
void test_pointers(void) {
    int x = 42;
    int *p = &x;
    int *q = NULL;
    
    ASSERT_NOT_NULL(p);
    ASSERT_NULL(q);
}

// ========== ЗАПУСК ТЕСТОВ ==========

int main(void) {
    // Создание тестового набора
    TestSuite suite;
    test_suite_init(&suite, "Моя тестовая библиотека");
    
    // Добавление тестов
    RUN_TEST(suite, test_add);
    RUN_TEST(suite, test_subtract);
    RUN_TEST(suite, test_multiply);
    RUN_TEST(suite, test_divide);
    RUN_TEST(suite, test_factorial);
    RUN_TEST(suite, test_is_even);
    RUN_TEST(suite, test_strings);
    RUN_TEST(suite, test_pointers);
    
    // Запуск тестов
    test_suite_run(&suite);
    
    // Вывод результатов
    test_suite_summary(&suite);
    
    // Запуск одного теста (демонстрация RUN_SINGLE_TEST)
    printf("\n=== Демонстрация RUN_SINGLE_TEST ===\n");
    RUN_SINGLE_TEST(test_add);
    
    return suite.failed > 0 ? 1 : 0;
}
```
Makefile:
```c
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -I include
CFLAGS_DEBUG = -g -O0
CFLAGS_RELEASE = -O2 -DNDEBUG

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LIB_OBJS = $(filter-out $(OBJ_DIR)/main.o,$(OBJS))
TARGET = $(BIN_DIR)/minitest_demo
LIB = $(BIN_DIR)/libminitest.a

.PHONY: all clean lib demo test

all: lib demo

lib: $(LIB)

demo: $(TARGET)

$(LIB): $(LIB_OBJS) | $(BIN_DIR)
	ar rcs $@ $^

$(TARGET): $(OBJ_DIR)/main.o $(LIB) | $(BIN_DIR)
	$(CC) $< -L$(BIN_DIR) -lminitest -lm -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Пример тестов с использованием библиотеки
test: $(BIN_DIR)/test_example
	./$(BIN_DIR)/test_example

$(BIN_DIR)/test_example: tests/test_example.c $(LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $< -L$(BIN_DIR) -lminitest -lm -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
```
tests/test_example.c:
```c
#include <stdio.h>
#include <math.h>
#include "minitest.h"

// ========== ТЕСТИРУЕМЫЕ ФУНКЦИИ ==========

double my_sqrt(double x) {
    if (x < 0) return -1;
    if (x == 0) return 0;
    
    double guess = x / 2;
    double prev;
    do {
        prev = guess;
        guess = (guess + x / guess) / 2;
    } while (fabs(guess - prev) > 1e-12);
    
    return guess;
}

int my_gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int my_lcm(int a, int b) {
    return a / my_gcd(a, b) * b;
}

// ========== ТЕСТЫ ==========

void test_sqrt(void) {
    ASSERT_DOUBLE_EQ(my_sqrt(4.0), 2.0, 0.0001);
    ASSERT_DOUBLE_EQ(my_sqrt(9.0), 3.0, 0.0001);
    ASSERT_DOUBLE_EQ(my_sqrt(2.0), 1.41421356, 0.0001);
    ASSERT_DOUBLE_EQ(my_sqrt(0.0), 0.0, 0.0001);
    ASSERT_TRUE(my_sqrt(-1.0) < 0);
}

void test_gcd(void) {
    ASSERT_EQ(my_gcd(48, 18), 6);
    ASSERT_EQ(my_gcd(17, 19), 1);
    ASSERT_EQ(my_gcd(100, 25), 25);
    ASSERT_EQ(my_gcd(0, 5), 5);
    ASSERT_EQ(my_gcd(-48, 18), 6);
}

void test_lcm(void) {
    ASSERT_EQ(my_lcm(12, 18), 36);
    ASSERT_EQ(my_lcm(3, 5), 15);
    ASSERT_EQ(my_lcm(6, 8), 24);
}

void test_failing_test(void) {
    // Этот тест специально падает для демонстрации
    ASSERT_EQ(1, 2);
}

// ========== ЗАПУСК ==========

int main(void) {
    TestSuite suite;
    test_suite_init(&suite, "Math Functions Tests");
    
    RUN_TEST(suite, test_sqrt);
    RUN_TEST(suite, test_gcd);
    RUN_TEST(suite, test_lcm);
    // RUN_TEST(suite, test_failing_test);  // Раскомментировать для проверки падающего теста
    
    test_suite_run(&suite);
    test_suite_summary(&suite);
    
    return suite.failed > 0 ? 1 : 0;
}
```
meson.build:
```c
project('minitest', 'c',
    version: '1.0.0',
    default_options: ['warning_level=3', 'c_std=c11']
)

inc = include_directories('include')

lib_src = files('src/minitest.c')

minitest = static_library('minitest', lib_src,
    include_directories: inc
)

executable('minitest_demo', 'src/main.c',
    include_directories: inc,
    link_with: minitest,
    dependencies: meson.get_compiler('c').find_library('m', required: false)
)

test_exe = executable('test_example', 'tests/test_example.c',
    include_directories: inc,
    link_with: minitest,
    dependencies: meson.get_compiler('c').find_library('m', required: false)
)

test('minitest example', test_exe)
```
