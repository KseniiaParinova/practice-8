#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <stddef.h>

typedef enum {
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_PATCH,
    HTTP_UNKNOWN
} HttpMethod;

typedef struct {
    char name[64];
    char value[256];
} HttpHeader;

typedef struct {
    HttpMethod method;
    char path[256];
    char version[16];
    HttpHeader headers[50];
    int header_count;
    char *body;
    size_t body_length;
    char *query_string;  // Параметры запроса
} HttpRequest;

// Парсинг
HttpRequest* http_request_parse(const char *raw);
void http_request_free(HttpRequest *req);

// Создание
HttpRequest* http_request_create(HttpMethod method, const char *path);
void http_request_add_header(HttpRequest *req, const char *name, const char *value);
void http_request_set_body(HttpRequest *req, const char *body);
void http_request_set_body_binary(HttpRequest *req, const char *body, size_t length);

// Сериализация
char* http_request_to_string(const HttpRequest *req);

// Утилиты
const char* http_method_to_string(HttpMethod method);
HttpMethod http_method_from_string(const char *str);
const char* http_request_get_header(const HttpRequest *req, const char *name);

// Дополнительные утилиты
void http_request_remove_header(HttpRequest *req, const char *name);
void http_request_clear_headers(HttpRequest *req);
char* http_request_get_query_param(const HttpRequest *req, const char *key);

#endif // HTTP_REQUEST_H