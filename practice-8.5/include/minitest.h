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