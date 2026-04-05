#include "../include/http_response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int code;
    const char *text;
} StatusCodeMap;

static const StatusCodeMap status_codes[] = {
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {204, "No Content"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {303, "See Other"},
    {304, "Not Modified"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {502, "Bad Gateway"},
    {503, "Service Unavailable"},
    {0, NULL}
};

const char* http_status_code_to_text(int status_code) {
    for (int i = 0; status_codes[i].code != 0; i++) {
        if (status_codes[i].code == status_code) {
            return status_codes[i].text;
        }
    }
    return "Unknown";
}

HttpResponse* http_response_create(int status_code, const char *status_text) {
    HttpResponse *res = (HttpResponse*)calloc(1, sizeof(HttpResponse));
    if (!res) return NULL;
    
    res->status_code = status_code;
    if (status_text) {
        strncpy(res->status_text, status_text, 63);
    } else {
        strncpy(res->status_text, http_status_code_to_text(status_code), 63);
    }
    res->status_text[63] = '\0';
    strcpy(res->version, "HTTP/1.1");
    res->header_count = 0;
    res->body = NULL;
    res->body_length = 0;
    
    // Добавляем стандартные заголовки
    http_response_add_header(res, "Content-Type", "text/plain");
    http_response_add_header(res, "Connection", "close");
    
    return res;
}

void http_response_add_header(HttpResponse *res, const char *name, const char *value) {
    if (!res || res->header_count >= 50) return;
    
    strncpy(res->headers[res->header_count].name, name, 63);
    res->headers[res->header_count].name[63] = '\0';
    strncpy(res->headers[res->header_count].value, value, 255);
    res->headers[res->header_count].value[255] = '\0';
    res->header_count++;
}

void http_response_set_body(HttpResponse *res, const char *body) {
    if (!res) return;
    
    if (res->body) {
        free(res->body);
    }
    
    if (body) {
        res->body_length = strlen(body);
        res->body = (char*)malloc(res->body_length + 1);
        if (res->body) {
            strcpy(res->body, body);
            char content_length[32];
            snprintf(content_length, sizeof(content_length), "%zu", res->body_length);
            http_response_add_header(res, "Content-Length", content_length);
        }
    } else {
        res->body = NULL;
        res->body_length = 0;
    }
}

void http_response_set_body_binary(HttpResponse *res, const char *body, size_t length) {
    if (!res) return;
    
    if (res->body) {
        free(res->body);
    }
    
    if (body && length > 0) {
        res->body = (char*)malloc(length);
        if (res->body) {
            memcpy(res->body, body, length);
            res->body_length = length;
            char content_length[32];
            snprintf(content_length, sizeof(content_length), "%zu", res->body_length);
            http_response_add_header(res, "Content-Length", content_length);
        }
    } else {
        res->body = NULL;
        res->body_length = 0;
    }
}

char* http_response_to_string(const HttpResponse *res) {
    if (!res) return NULL;
    
    size_t buffer_size = 4096;
    char *buffer = (char*)malloc(buffer_size);
    if (!buffer) return NULL;
    
    int offset = 0;
    
    // Стартовая строка
    offset += snprintf(buffer + offset, buffer_size - offset,
                      "%s %d %s\r\n",
                      res->version,
                      res->status_code,
                      res->status_text);
    
    // Заголовки
    for (int i = 0; i < res->header_count; i++) {
        offset += snprintf(buffer + offset, buffer_size - offset,
                          "%s: %s\r\n",
                          res->headers[i].name,
                          res->headers[i].value);
    }
    
    // Разделитель
    offset += snprintf(buffer + offset, buffer_size - offset, "\r\n");
    
    // Тело
    if (res->body && res->body_length > 0) {
        if (offset + res->body_length >= buffer_size) {
            buffer_size = offset + res->body_length + 1024;
            buffer = (char*)realloc(buffer, buffer_size);
            if (!buffer) return NULL;
        }
        memcpy(buffer + offset, res->body, res->body_length);
        offset += res->body_length;
    }
    
    buffer[offset] = '\0';
    return buffer;
}

HttpResponse* http_response_parse(const char *raw) {
    if (!raw) return NULL;
    
    HttpResponse *res = (HttpResponse*)calloc(1, sizeof(HttpResponse));
    if (!res) return NULL;
    
    char *raw_copy = strdup(raw);
    if (!raw_copy) {
        free(res);
        return NULL;
    }
    
    // Парсим стартовую строку
    char *line = strtok(raw_copy, "\r\n");
    if (!line) {
        free(raw_copy);
        free(res);
        return NULL;
    }
    
    char version[16];
    int status_code;
    char status_text[64];
    
    if (sscanf(line, "%s %d %63[^\r\n]", version, &status_code, status_text) != 3) {
        free(raw_copy);
        free(res);
        return NULL;
    }
    
    strncpy(res->version, version, 15);
    res->status_code = status_code;
    strncpy(res->status_text, status_text, 63);
    
    // Парсим заголовки
    res->header_count = 0;
    char *header_line;
    while ((header_line = strtok(NULL, "\r\n")) != NULL && strlen(header_line) > 0) {
        char *colon = strchr(header_line, ':');
        if (colon && res->header_count < 50) {
            *colon = '\0';
            char *name = header_line;
            char *value = colon + 1;
            
            while (isspace(*value)) value++;
            char *end = value + strlen(value) - 1;
            while (end > value && isspace(*end)) end--;
            *(end + 1) = '\0';
            
            http_response_add_header(res, name, value);
        }
    }
    
    // Парсим тело
    char *body_start = strstr(raw, "\r\n\r\n");
    if (body_start) {
        body_start += 4;
        size_t body_len = strlen(body_start);
        
        const char *content_length = http_response_get_header(res, "Content-Length");
        if (content_length) {
            res->body_length = atoi(content_length);
            if (res->body_length > 0 && res->body_length <= body_len) {
                res->body = (char*)malloc(res->body_length + 1);
                if (res->body) {
                    memcpy(res->body, body_start, res->body_length);
                    res->body[res->body_length] = '\0';
                }
            }
        } else if (body_len > 0) {
            res->body = strdup(body_start);
            res->body_length = body_len;
        }
    }
    
    free(raw_copy);
    return res;
}

void http_response_free(HttpResponse *res) {
    if (!res) return;
    
    if (res->body) {
        free(res->body);
    }
    free(res);
}

const char* http_response_get_header(const HttpResponse *res, const char *name) {
    if (!res || !name) return NULL;
    
    for (int i = 0; i < res->header_count; i++) {
        if (strcasecmp(res->headers[i].name, name) == 0) {
            return res->headers[i].value;
        }
    }
    return NULL;
}

void http_response_remove_header(HttpResponse *res, const char *name) {
    if (!res || !name) return;
    
    for (int i = 0; i < res->header_count; i++) {
        if (strcasecmp(res->headers[i].name, name) == 0) {
            for (int j = i; j < res->header_count - 1; j++) {
                res->headers[j] = res->headers[j + 1];
            }
            res->header_count--;
            break;
        }
    }
}

void http_response_clear_headers(HttpResponse *res) {
    if (!res) return;
    res->header_count = 0;
}

HttpResponse* http_response_ok() {
    HttpResponse *res = http_response_create(200, "OK");
    http_response_set_body(res, "OK");
    return res;
}

HttpResponse* http_response_not_found() {
    HttpResponse *res = http_response_create(404, "Not Found");
    http_response_set_body(res, "404 Not Found");
    return res;
}

HttpResponse* http_response_bad_request() {
    HttpResponse *res = http_response_create(400, "Bad Request");
    http_response_set_body(res, "400 Bad Request");
    return res;
}

HttpResponse* http_response_internal_error() {
    HttpResponse *res = http_response_create(500, "Internal Server Error");
    http_response_set_body(res, "500 Internal Server Error");
    return res;
}