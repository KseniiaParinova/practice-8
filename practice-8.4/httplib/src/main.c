#include "../include/http.h"
#include <stdio.h>
#include <stdlib.h>

void print_request(const HttpRequest *req) {
    printf("=== HTTP Request ===\n");
    printf("Method: %s\n", http_method_to_string(req->method));
    printf("Path: %s\n", req->path);
    printf("Version: %s\n", req->version);
    printf("Headers (%d):\n", req->header_count);
    for (int i = 0; i < req->header_count; i++) {
        printf("  %s: %s\n", req->headers[i].name, req->headers[i].value);
    }
    if (req->body && req->body_length > 0) {
        printf("Body (%zu): %s\n", req->body_length, req->body);
    }
    printf("\n");
}

void print_response(const HttpResponse *res) {
    printf("=== HTTP Response ===\n");
    printf("Status: %d %s\n", res->status_code, res->status_text);
    printf("Version: %s\n", res->version);
    printf("Headers (%d):\n", res->header_count);
    for (int i = 0; i < res->header_count; i++) {
        printf("  %s: %s\n", res->headers[i].name, res->headers[i].value);
    }
    if (res->body && res->body_length > 0) {
        printf("Body (%zu): %s\n", res->body_length, res->body);
    }
    printf("\n");
}

int main() {
    printf("=== HTTP Library Test ===\n\n");
    
    // Тестирование создания и парсинга запросов
    printf("--- Testing HTTP Requests ---\n");
    
    // Создание GET запроса
    HttpRequest *get_req = http_request_create(HTTP_GET, "/api/users?id=123");
    http_request_add_header(get_req, "Accept", "application/json");
    http_request_add_header(get_req, "Authorization", "Bearer token123");
    
    char *get_req_str = http_request_to_string(get_req);
    printf("GET Request:\n%s\n", get_req_str);
    
    // Парсинг запроса
    HttpRequest *parsed_req = http_request_parse(get_req_str);
    print_request(parsed_req);
    
    // Получение query параметра
    char *user_id = http_request_get_query_param(parsed_req, "id");
    if (user_id) {
        printf("Query param 'id': %s\n\n", user_id);
        free(user_id);
    }
    
    // Создание POST запроса с телом
    HttpRequest *post_req = http_request_create(HTTP_POST, "/api/users");
    http_request_add_header(post_req, "Content-Type", "application/json");
    http_request_set_body(post_req, "{\"name\":\"John\",\"age\":30}");
    
    char *post_req_str = http_request_to_string(post_req);
    printf("POST Request:\n%s\n", post_req_str);
    
    // Тестирование ответов
    printf("\n--- Testing HTTP Responses ---\n");
    
    // Создание успешного ответа
    HttpResponse *ok_res = http_response_create(200, "OK");
    http_response_add_header(ok_res, "Content-Type", "application/json");
    http_response_set_body(ok_res, "{\"status\":\"success\",\"data\":{\"id\":1}}");
    
    char *ok_res_str = http_response_to_string(ok_res);
    printf("OK Response:\n%s\n", ok_res_str);
    
    // Парсинг ответа
    HttpResponse *parsed_res = http_response_parse(ok_res_str);
    print_response(parsed_res);
    
    // Использование предустановленных ответов
    HttpResponse *not_found = http_response_not_found();
    char *not_found_str = http_response_to_string(not_found);
    printf("404 Response:\n%s\n", not_found_str);
    
    // Тестирование методов модификации
    printf("\n--- Testing Header Manipulation ---\n");
    HttpResponse *res = http_response_create(200, "OK");
    http_response_add_header(res, "X-Custom-Header", "value1");
    http_response_add_header(res, "X-Custom-Header", "value2");
    printf("Before removal:\n");
    for (int i = 0; i < res->header_count; i++) {
        printf("  %s: %s\n", res->headers[i].name, res->headers[i].value);
    }
    
    http_response_remove_header(res, "X-Custom-Header");
    printf("After removal:\n");
    for (int i = 0; i < res->header_count; i++) {
        printf("  %s: %s\n", res->headers[i].name, res->headers[i].value);
    }
    
    // Очистка памяти
    http_request_free(get_req);
    http_request_free(parsed_req);
    http_request_free(post_req);
    http_response_free(ok_res);
    http_response_free(parsed_res);
    http_response_free(not_found);
    http_response_free(res);
    free(get_req_str);
    free(post_req_str);
    free(ok_res_str);
    free(not_found_str);
    
    return 0;
}