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