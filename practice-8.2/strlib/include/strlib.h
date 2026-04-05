#ifndef STRLIB_H
#define STRLIB_H

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} String;

String* str_new(const char *init);
void str_append(String *s, const char *text);
void str_replace(String *s, const char *old_str, const char *new_str);
String** str_split(const String *s, char delimiter, size_t *count);
String* str_join(String **parts, size_t count, const char *separator);
String* str_reverse(const String *s);

#endif