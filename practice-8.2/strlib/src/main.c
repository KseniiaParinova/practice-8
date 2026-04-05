#include <stdio.h>
#include "strlib.h"

int main(void) {
    printf("=== Библиотека строк (strlib) ===\n\n");
    
    // Создание строк
    String *s = str_new("Hello");
    printf("Создана строка: ");
    str_println(s);
    
    // Добавление
    str_append(s, ", World!");
    printf("После str_append: ");
    str_println(s);
    
    // Длина
    printf("Длина строки: %zu\n", str_length(s));
    
    // Поиск
    int pos = str_find(s, "World");
    printf("Позиция 'World': %d\n", pos);
    
    // Замена
    str_replace(s, "World", "C Programmer");
    printf("После замены: ");
    str_println(s);
    
    // Регистр
    String *upper = str_to_upper(s);
    printf("Верхний регистр: ");
    str_println(upper);
    
    // Обрезка пробелов
    String *trimmed = str_new("   Hello   ");
    printf("До обрезки: '");
    str_print(trimmed);
    printf("'\n");
    
    String *trim_result = str_trim(trimmed);
    printf("После обрезки: '");
    str_println(trim_result);
    
    // Разделение строки
    String *csv = str_new("apple,banana,cherry,date");
    size_t count;
    String **parts = str_split(csv, ',', &count);
    printf("Разделение строки: ");
    for (size_t i = 0; i < count; i++) {
        printf("[%s]", parts[i]->data);
        if (i < count - 1) printf(", ");
    }
    printf("\n");
    
    // Объединение
    String *joined = str_join(parts, count, " | ");
    printf("Объединение: ");
    str_println(joined);
    
    // Переворот строки
    String *reversed = str_reverse(s);
    printf("Переворот строки: ");
    str_println(reversed);
    
    // Сравнение
    String *s1 = str_new("Hello");
    String *s2 = str_new("hello");
    printf("Сравнение 'Hello' и 'hello' (с учётом регистра): %d\n", str_compare(s1, s2));
    printf("Сравнение 'Hello' и 'hello' (без учёта регистра): %d\n", str_equals_ignore_case(s1, s2));
    
    // Освобождение памяти
    str_free(s);
    str_free(upper);
    str_free(trimmed);
    str_free(trim_result);
    str_free(csv);
    str_free_array(parts, count);
    str_free(joined);
    str_free(reversed);
    str_free(s1);
    str_free(s2);
    
    return 0;
}