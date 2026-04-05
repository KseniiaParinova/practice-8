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