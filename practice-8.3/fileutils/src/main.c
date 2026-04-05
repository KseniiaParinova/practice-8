#include "../include/fileutils.h"
#include <stdio.h>

int main() {
    printf("=== Тестирование библиотеки файловых утилит ===\n\n");
    
    // Тестирование path функций
    printf("--- Тестирование path функций ---\n");
    char *joined = path_join("/home/user", "file.txt");
    printf("path_join: %s\n", joined);
    free(joined);
    
    char *dirname = path_dirname("/home/user/file.txt");
    printf("path_dirname: %s\n", dirname);
    free(dirname);
    
    char *basename = path_basename("/home/user/file.txt");
    printf("path_basename: %s\n", basename);
    free(basename);
    
    char *ext = path_extension("archive.tar.gz");
    printf("path_extension: %s\n", ext);
    free(ext);
    
    // Создание тестового файла
    printf("\n--- Тестирование io функций ---\n");
    const char *test_file = "test.txt";
    
    if (file_write_all(test_file, "Hello\nWorld\nTest") == 0) {
        printf("Файл создан успешно\n");
        
        // Чтение файла
        size_t size;
        char *content = file_read_all(test_file, &size);
        if (content) {
            printf("Содержимое файла (%zu байт):\n%s\n", size, content);
            free(content);
        }
        
        // Построчное чтение
        size_t line_count;
        char **lines = file_read_lines(test_file, &line_count);
        if (lines) {
            printf("Строк в файле: %zu\n", line_count);
            for (size_t i = 0; i < line_count; i++) {
                printf("Строка %zu: %s\n", i, lines[i]);
            }
            file_free_lines(lines, line_count);
        }
        
        // Дописывание в файл
        file_append(test_file, "\nДополнительная строка");
        
        // Информация о файле
        printf("Размер файла: %ld байт\n", file_size(test_file));
        printf("Файл существует: %s\n", file_exists(test_file) ? "да" : "нет");
        
        // Проверка типа
        printf("Это файл: %s\n", path_is_file(test_file) ? "да" : "нет");
        printf("Это директория: %s\n", path_is_directory(test_file) ? "да" : "нет");
        
        // Копирование
        if (file_copy(test_file, "test_copy.txt") == 0) {
            printf("Файл скопирован успешно\n");
            file_delete("test_copy.txt");
        }
        
        // Удаление тестового файла
        file_delete(test_file);
    }
    
    return 0;
}