#include <stdlib.h>
#include <string.h>
#include "strlib.h"

String* str_new(const char *init) {
    String *s = malloc(sizeof(String));
    s->length = strlen(init);
    s->capacity = s->length + 16;
    s->data = malloc(s->capacity + 1);
    strcpy(s->data, init);
    return s;
}

void str_append(String *s, const char *text) {
    size_t len = strlen(text);
    s->data = realloc(s->data, s->length + len + 1);
    strcpy(s->data + s->length, text);
    s->length += len;
}