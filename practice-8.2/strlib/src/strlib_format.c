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