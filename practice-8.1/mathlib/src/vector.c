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