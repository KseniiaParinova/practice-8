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