#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minitest.h"

int _test_failed = 0;

void test_suite_init(TestSuite *suite, const char *name) {
    if (suite == NULL) return;
    
    suite->name = name;
    suite->tests = NULL;
    suite->test_count = 0;
    suite->passed = 0;
    suite->failed = 0;
}

void test_suite_add(TestSuite *suite, const char *name, TestFunc func) {
    if (suite == NULL || name == NULL || func == NULL) return;
    
    suite->tests = realloc(suite->tests, (suite->test_count + 1) * sizeof(TestCase));
    suite->tests[suite->test_count].name = name;
    suite->tests[suite->test_count].func = func;
    suite->test_count++;
}

void test_suite_run(TestSuite *suite) {
    if (suite == NULL) return;
    
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    ЗАПУСК ТЕСТОВ: %-30s ║\n", suite->name);
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    
    for (int i = 0; i < suite->test_count; i++) {
        printf("\n▶ Тест: %s\n", suite->tests[i].name);
        printf("  ");
        
        _test_failed = 0;
        suite->tests[i].func();
        
        if (_test_failed) {
            suite->failed++;
            printf("  ❌ НЕ ПРОЙДЕН\n");
        } else {
            suite->passed++;
            printf("  ✅ ПРОЙДЕН\n");
        }
    }
}

void test_suite_summary(const TestSuite *suite) {
    if (suite == NULL) return;
    
    int total = suite->passed + suite->failed;
    
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    РЕЗУЛЬТАТЫ ТЕСТОВ                          ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║  Набор: %-45s ║\n", suite->name);
    printf("║  ✅ Пройдено: %-38d ║\n", suite->passed);
    printf("║  ❌ Не пройдено: %-35d ║\n", suite->failed);
    printf("║  📊 Всего: %-41d ║\n", total);
    printf("║  📈 Успешность: %-36.1f%% ║\n", 
           total > 0 ? (float)suite->passed / total * 100 : 0);
    printf("╚══════════════════════════════════════════════════════════════╝\n");
}

void test_reset_failed(void) {
    _test_failed = 0;
}