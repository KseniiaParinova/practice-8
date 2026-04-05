#include "../include/http_request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const char* method_strings[] = {
    "GET", "POST", "PUT", "DELETE", "HEAD", "OPTIONS", "PATCH", NULL
};

const char* http_method_to_string(HttpMethod method) {
    if (method >= HTTP_GET && method <= HTTP_PATCH) {
        return method_strings[method];
    }
    return "UNKNOWN";
}

HttpMethod http_method_from_string(const char *str) {
    for (int i = 0; method_strings[i] != NULL; i++) {
        if (strcasecmp(str, method_strings[i]) == 0) {
            return (HttpMethod)i;
        }
    }
    return HTTP_UNKNOWN;
}

HttpRequest* http_request_create(HttpMethod method, const char *path) {
    HttpRequest *req = (HttpRequest*)calloc(1, sizeof(HttpRequest));
    if (!req) return NULL;
    
    req->method = method;
    strncpy(req->path, path, 255);
    req->path[255] = '\0';
    strcpy(req->version, "HTTP/1.1");
    req->header_count = 0;
    req->body = NULL;
    req->body_length = 0;
    req->query_string = NULL;
    
    // Добавляем стандартные заголовки
    http_request_add_header(req, "Host", "localhost");
    http_request_add_header(req, "Connection", "close");
    http_request_add_header(req, "User-Agent", "HTTPLib/1.0");
    
    return req;
}

void http_request_add_header(HttpRequest *req, const char *name, const char *value) {
    if (!req || req->header_count >= 50) return;
    
    strncpy(req->headers[req->header_count].name, name, 63);
    req->headers[req->header_count].name[63] = '\0';
    strncpy(req->headers[req->header_count].value, value, 255);
    req->headers[req->header_count].value[255] = '\0';
    req->header_count++;
}

void http_request_set_body(HttpRequest *req, const char *body) {
    if (!req) return;
    
    if (req->body) {
        free(req->body);
    }
    
    if (body) {
        req->body_length = strlen(body);
        req->body = (char*)malloc(req->body_length + 1);
        if (req->body) {
            strcpy(req->body, body);
            // Обновляем Content-Length
            char content_length[32];
            snprintf(content_length, sizeof(content_length), "%zu", req->body_length);
            http_request_add_header(req, "Content-Length", content_length);
        }
    } else {
        req->body = NULL;
        req->body_length = 0;
    }
}

void http_request_set_body_binary(HttpRequest *req, const char *body, size_t length) {
    if (!req) return;
    
    if (req->body) {
        free(req->body);
    }
    
    if (body && length > 0) {
        req->body = (char*)malloc(length);
        if (req->body) {
            memcpy(req->body, body, length);
            req->body_length = length;
            char content_length[32];
            snprintf(content_length, sizeof(content_length), "%zu", req->body_length);
            http_request_add_header(req, "Content-Length", content_length);
        }
    } else {
        req->body = NULL;
        req->body_length = 0;
    }
}

char* http_request_to_string(const HttpRequest *req) {
    if (!req) return NULL;
    
    // Начальный размер буфера
    size_t buffer_size = 4096;
    char *buffer = (char*)malloc(buffer_size);
    if (!buffer) return NULL;
    
    int offset = 0;
    
    // Записываем стартовую строку
    offset += snprintf(buffer + offset, buffer_size - offset, 
                      "%s %s %s\r\n",
                      http_method_to_string(req->method),
                      req->path,
                      req->version);
    
    // Записываем заголовки
    for (int i = 0; i < req->header_count; i++) {
        offset += snprintf(buffer + offset, buffer_size - offset,
                          "%s: %s\r\n",
                          req->headers[i].name,
                          req->headers[i].value);
    }
    
    // Завершаем заголовки
    offset += snprintf(buffer + offset, buffer_size - offset, "\r\n");
    
    // Записываем тело
    if (req->body && req->body_length > 0) {
        if (offset + req->body_length >= buffer_size) {
            buffer_size = offset + req->body_length + 1024;
            buffer = (char*)realloc(buffer, buffer_size);
            if (!buffer) return NULL;
        }
        memcpy(buffer + offset, req->body, req->body_length);
        offset += req->body_length;
    }
    
    buffer[offset] = '\0';
    return buffer;
}

HttpRequest* http_request_parse(const char *raw) {
    if (!raw) return NULL;
    
    HttpRequest *req = (HttpRequest*)calloc(1, sizeof(HttpRequest));
    if (!req) return NULL;
    
    char *raw_copy = strdup(raw);
    if (!raw_copy) {
        free(req);
        return NULL;
    }
    
    // Парсим стартовую строку
    char *line = strtok(raw_copy, "\r\n");
    if (!line) {
        free(raw_copy);
        free(req);
        return NULL;
    }
    
    char method_str[32], path[256], version[16];
    if (sscanf(line, "%s %s %s", method_str, path, version) != 3) {
        free(raw_copy);
        free(req);
        return NULL;
    }
    
    req->method = http_method_from_string(method_str);
    strncpy(req->path, path, 255);
    strncpy(req->version, version, 15);
    
    // Извлекаем query string
    char *query_start = strchr(req->path, '?');
    if (query_start) {
        *query_start = '\0';
        req->query_string = strdup(query_start + 1);
    }
    
    // Парсим заголовки
    req->header_count = 0;
    char *header_line;
    while ((header_line = strtok(NULL, "\r\n")) != NULL && strlen(header_line) > 0) {
        char *colon = strchr(header_line, ':');
        if (colon && req->header_count < 50) {
            *colon = '\0';
            char *name = header_line;
            char *value = colon + 1;
            
            // Пропускаем пробелы в начале значения
            while (isspace(*value)) value++;
            // Удаляем пробелы в конце значения
            char *end = value + strlen(value) - 1;
            while (end > value && isspace(*end)) end--;
            *(end + 1) = '\0';
            
            http_request_add_header(req, name, value);
        }
    }
    
    // Парсим тело
    char *body_start = strstr(raw, "\r\n\r\n");
    if (body_start) {
        body_start += 4;
        size_t body_len = strlen(body_start);
        
        // Проверяем Content-Length
        const char *content_length = http_request_get_header(req, "Content-Length");
        if (content_length) {
            req->body_length = atoi(content_length);
            if (req->body_length > 0 && req->body_length <= body_len) {
                req->body = (char*)malloc(req->body_length + 1);
                if (req->body) {
                    memcpy(req->body, body_start, req->body_length);
                    req->body[req->body_length] = '\0';
                }
            }
        } else if (strstr(header_line, "Transfer-Encoding: chunked")) {
            // Упрощенная обработка chunked encoding (для демонстрации)
            req->body = strdup(body_start);
            req->body_length = body_len;
        } else if (body_len > 0) {
            req->body = strdup(body_start);
            req->body_length = body_len;
        }
    }
    
    free(raw_copy);
    return req;
}

void http_request_free(HttpRequest *req) {
    if (!req) return;
    
    if (req->body) {
        free(req->body);
    }
    if (req->query_string) {
        free(req->query_string);
    }
    free(req);
}

const char* http_request_get_header(const HttpRequest *req, const char *name) {
    if (!req || !name) return NULL;
    
    for (int i = 0; i < req->header_count; i++) {
        if (strcasecmp(req->headers[i].name, name) == 0) {
            return req->headers[i].value;
        }
    }
    return NULL;
}

void http_request_remove_header(HttpRequest *req, const char *name) {
    if (!req || !name) return;
    
    for (int i = 0; i < req->header_count; i++) {
        if (strcasecmp(req->headers[i].name, name) == 0) {
            for (int j = i; j < req->header_count - 1; j++) {
                req->headers[j] = req->headers[j + 1];
            }
            req->header_count--;
            break;
        }
    }
}

void http_request_clear_headers(HttpRequest *req) {
    if (!req) return;
    req->header_count = 0;
}

char* http_request_get_query_param(const HttpRequest *req, const char *key) {
    if (!req || !req->query_string || !key) return NULL;
    
    char *query = strdup(req->query_string);
    if (!query) return NULL;
    
    char *token = strtok(query, "&");
    while (token) {
        char *eq = strchr(token, '=');
        if (eq) {
            *eq = '\0';
            if (strcmp(token, key) == 0) {
                char *value = strdup(eq + 1);
                free(query);
                return value;
            }
        }
        token = strtok(NULL, "&");
    }
    
    free(query);
    return NULL;
}