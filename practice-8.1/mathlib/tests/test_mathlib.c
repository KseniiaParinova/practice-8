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