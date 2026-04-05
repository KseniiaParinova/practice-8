#include <stdio.h>
#include "strlib.h"

static int _test_failed = 0;

#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            printf("  FAILED: %s:%d\n", __FILE__, __LINE__); \
            _test_failed = 1; \
        } \
    } while(0)

#define ASSERT_STR_EQ(a, b) \
    do { \
        if (strcmp((a), (b)) != 0) { \
            printf("  FAILED: %s:%d: \"%s\" != \"%s\"\n", __FILE__, __LINE__, (a), (b)); \
            _test_failed = 1; \
        } \
    } while(0)

void test_create(void) {
    printf("  test_create: ");
    String *s = str_new("Hello");
    ASSERT_STR_EQ(s->data, "Hello");
    ASSERT_EQ(s->length, 5);
    str_free(s);
    printf("PASSED\n");
}

void test_append(void) {
    printf("  test_append: ");
    String *s = str_new("Hello");
    str_append(s, ", World!");
    ASSERT_STR_EQ(s->data, "Hello, World!");
    str_free(s);
    printf("PASSED\n");
}

void test_find(void) {
    printf("  test_find: ");
    String *s = str_new("Hello, World!");
    int pos = str_find(s, "World");
    ASSERT_EQ(pos, 7);
    str_free(s);
    printf("PASSED\n");
}

void test_replace(void) {
    printf("  test_replace: ");
    String *s = str_new("Hello, World!");
    str_replace(s, "World", "C Programmer");
    ASSERT_STR_EQ(s->data, "Hello, C Programmer!");
    str_free(s);
    printf("PASSED\n");
}

void test_trim(void) {
    printf("  test_trim: ");
    String *s = str_new("   Hello   ");
    String *t = str_trim(s);
    ASSERT_STR_EQ(t->data, "Hello");
    str_free(s);
    str_free(t);
    printf("PASSED\n");
}

void test_split_join(void) {
    printf("  test_split_join: ");
    String *s = str_new("a,b,c,d");
    size_t count;
    String **parts = str_split(s, ',', &count);
    ASSERT_EQ(count, 4);
    ASSERT_STR_EQ(parts[0]->data, "a");
    ASSERT_STR_EQ(parts[1]->data, "b");
    ASSERT_STR_EQ(parts[2]->data, "c");
    ASSERT_STR_EQ(parts[3]->data, "d");
    
    String *joined = str_join(parts, count, "-");
    ASSERT_STR_EQ(joined->data, "a-b-c-d");
    
    str_free(s);
    str_free_array(parts, count);
    str_free(joined);
    printf("PASSED\n");
}

int main(void) {
    printf("\n=== Тестирование strlib ===\n\n");
    
    test_create();
    test_append();
    test_find();
    test_replace();
    test_trim();
    test_split_join();
    
    printf("\n=== Результат ===\n");
    if (_test_failed) {
        printf("❌ Некоторые тесты не пройдены!\n");
        return 1;
    } else {
        printf("✅ Все тесты пройдены успешно!\n");
        return 0;
    }
}