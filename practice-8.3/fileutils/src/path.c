#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "path.h"

// Объединение путей
char* path_join(const char *base, const char *name) {
    if (base == NULL || name == NULL) return NULL;
    
    size_t base_len = strlen(base);
    size_t name_len = strlen(name);
    
    // Проверяем, нужно ли добавлять разделитель
    int need_sep = 0;
    if (base_len > 0 && base[base_len - 1] != PATH_SEP && name[0] != PATH_SEP) {
        need_sep = 1;
    }
    
    size_t result_len = base_len + name_len + need_sep + 1;
    char *result = malloc(result_len);
    
    strcpy(result, base);
    if (need_sep) {
        result[base_len] = PATH_SEP;
        result[base_len + 1] = '\0';
        strcat(result, name);
    } else {
        strcat(result, name);
    }
    
    return result;
}

// Директория пути (удаляет последний компонент)
char* path_dirname(const char *path) {
    if (path == NULL) return NULL;
    
    char *result = strdup(path);
    char *last_sep = strrchr(result, PATH_SEP);
    
    if (last_sep == NULL) {
        strcpy(result, ".");
    } else {
        *last_sep = '\0';
        if (last_sep == result) {
            strcpy(result, "/");
        }
    }
    
    return result;
}

// Имя файла (последний компонент пути)
char* path_basename(const char *path) {
    if (path == NULL) return NULL;
    
    const char *last_sep = strrchr(path, PATH_SEP);
    if (last_sep == NULL) {
        return strdup(path);
    } else {
        return strdup(last_sep + 1);
    }
}

// Расширение файла
char* path_extension(const char *path) {
    if (path == NULL) return NULL;
    
    char *basename = path_basename(path);
    char *last_dot = strrchr(basename, '.');
    
    if (last_dot == NULL || last_dot == basename) {
        free(basename);
        return strdup("");
    }
    
    char *ext = strdup(last_dot + 1);
    free(basename);
    return ext;
}

// Путь без расширения
char* path_without_extension(const char *path) {
    if (path == NULL) return NULL;
    
    char *result = strdup(path);
    char *last_dot = strrchr(result, '.');
    
    if (last_dot != NULL && last_dot != result) {
        *last_dot = '\0';
    }
    
    return result;
}

// Нормализация пути (удаление лишних разделителей)
char* path_normalize(const char *path) {
    if (path == NULL) return NULL;
    
    size_t len = strlen(path);
    char *result = malloc(len + 2);
    size_t pos = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (path[i] == PATH_SEP && pos > 0 && result[pos - 1] == PATH_SEP) {
            continue;
        }
        result[pos++] = path[i];
    }
    
    if (pos > 1 && result[pos - 1] == PATH_SEP) {
        pos--;
    }
    
    result[pos] = '\0';
    return result;
}

// Проверка, является ли путь абсолютным
bool path_is_absolute(const char *path) {
    if (path == NULL) return false;
    
#ifdef _WIN32
    return (path[0] != '\0' && path[1] == ':' && path[2] == '\\') ||
           (path[0] == '\\' && path[1] == '\\');
#else
    return path[0] == '/';
#endif
}

// Проверка существования пути
bool path_exists(const char *path) {
    if (path == NULL) return false;
    
    struct stat st;
    return stat(path, &st) == 0;
}

// Проверка, является ли путь файлом
bool path_is_file(const char *path) {
    if (path == NULL) return false;
    
    struct stat st;
    if (stat(path, &st) != 0) return false;
    return S_ISREG(st.st_mode);
}

// Проверка, является ли путь директорией
bool path_is_directory(const char *path) {
    if (path == NULL) return false;
    
    struct stat st;
    if (stat(path, &st) != 0) return false;
    return S_ISDIR(st.st_mode);
}