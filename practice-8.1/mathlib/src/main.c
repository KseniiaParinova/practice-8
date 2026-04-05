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