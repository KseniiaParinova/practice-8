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