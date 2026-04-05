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