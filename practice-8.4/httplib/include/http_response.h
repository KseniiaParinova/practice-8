#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <stddef.h>

typedef struct {
    char name[64];
    char value[256];
} HttpResponseHeader;

typedef struct {
    int status_code;
    char status_text[64];
    char version[16];
    HttpResponseHeader headers[50];
    int header_count;
    char *body;
    size_t body_length;
} HttpResponse;

// Парсинг
HttpResponse* http_response_parse(const char *raw);
void http_response_free(HttpResponse *res);

// Создание
HttpResponse* http_response_create(int status_code, const char *status_text);
void http_response_add_header(HttpResponse *res, const char *name, const char *value);
void http_response_set_body(HttpResponse *res, const char *body);
void http_response_set_body_binary(HttpResponse *res, const char *body, size_t length);

// Сериализация
char* http_response_to_string(const HttpResponse *res);

// Утилиты
const char* http_response_get_header(const HttpResponse *res, const char *name);
const char* http_status_code_to_text(int status_code);
void http_response_remove_header(HttpResponse *res, const char *name);
void http_response_clear_headers(HttpResponse *res);

// Предустановленные ответы
HttpResponse* http_response_ok();
HttpResponse* http_response_not_found();
HttpResponse* http_response_bad_request();
HttpResponse* http_response_internal_error();

#endif // HTTP_RESPONSE_H