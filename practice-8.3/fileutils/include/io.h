#ifndef IO_H
#define IO_H

#include <stddef.h>

// Чтение файла целиком
char* file_read_all(const char *filename, size_t *size);

// Запись в файл
int file_write_all(const char *filename, const char *content);
int file_append(const char *filename, const char *content);

// Построчное чтение
char** file_read_lines(const char *filename, size_t *count);
void file_free_lines(char **lines, size_t count);

// Копирование и перемещение
int file_copy(const char *src, const char *dst);
int file_move(const char *src, const char *dst);
int file_delete(const char *path);

// Информация о файле
long file_size(const char *filename);
int file_exists(const char *filename);

#endif // IO_H