#include <stdio.h>
#include <string.h>
#include "../src/dynarray.h"
#include "test_common.h"

typedef struct { int x; int y; } Point;

static void test_struct_append_get(void) {
    DynArray a;
    da_init(&a, sizeof(Point));
    Point p1 = {1, 2}, p2 = {3, 4}, p3 = {5, 6};
    da_append(&a, &p1);
    da_append(&a, &p2);
    da_append(&a, &p3);
    ASSERT_EQ_SIZE(da_size(&a), 3 * sizeof(Point));
    Point* got = da_get(&a, 1);
    ASSERT_NOT_NULL(got);
    ASSERT_EQ_INT(got->x, 3);
    ASSERT_EQ_INT(got->y, 4);
    da_destroy(&a);
}

static void test_struct_modify(void) {
    DynArray a;
    da_init(&a, sizeof(Point));
    for (int i = 0; i < 5; i++) {
        Point p = {i, i * 10};
        da_append(&a, &p);
    }
    Point np = {99, 99};
    da_set(&a, 2, &np);
    Point* got = da_get(&a, 2);
    ASSERT_EQ_INT(got->x, 99);
    ASSERT_EQ_INT(got->y, 99);
    // neighbors untouched
    got = da_get(&a, 1);
    ASSERT_EQ_INT(got->x, 1);
    ASSERT_EQ_INT(got->y, 10);
    da_destroy(&a);
}

static void test_struct_remove(void) {
    DynArray a;
    da_init(&a, sizeof(Point));
    for (int i = 0; i < 4; i++) {
        Point p = {i, i};
        da_append(&a, &p);
    }
    da_remove(&a, 1);
    ASSERT_EQ_SIZE(da_size(&a), 3 * sizeof(Point));
    Point* got = da_get(&a, 1);
    ASSERT_EQ_INT(got->x, 2);
    ASSERT_EQ_INT(got->y, 2);
    got = da_get(&a, 2);
    ASSERT_EQ_INT(got->x, 3);
    ASSERT_EQ_INT(got->y, 3);
    da_destroy(&a);
}

static void test_struct_remove_large_fields(void) {
    // Use values whose high bytes are non-zero so a single-byte shift
    // in da_remove would corrupt the shifted elements.
    DynArray a;
    da_init(&a, sizeof(Point));
    Point points[] = {
        {0x11111111, 0x22222222},
        {0x33333333, 0x44444444},
        {0x55555555, 0x66666666},
        {0x77777777, (int)0x88888888},
    };
    for (int i = 0; i < 4; i++) da_append(&a, &points[i]);
    da_remove(&a, 1);
    Point* got = da_get(&a, 1);
    ASSERT_EQ_INT(got->x, 0x55555555);
    ASSERT_EQ_INT(got->y, 0x66666666);
    got = da_get(&a, 2);
    ASSERT_EQ_INT(got->x, 0x77777777);
    ASSERT_EQ_INT(got->y, (int)0x88888888);
    da_destroy(&a);
}

static void test_double(void) {
    DynArray a;
    da_init(&a, sizeof(double));
    for (int i = 0; i < 20; i++) {
        double v = i * 0.5;
        da_append(&a, &v);
    }
    ASSERT_EQ_SIZE(da_size(&a), 20 * sizeof(double));
    double* p = da_get(&a, 10);
    ASSERT_NOT_NULL(p);
    ASSERT(*p == 5.0);
    double out = 0.0;
    da_pop(&a, &out);
    ASSERT(out == 9.5);
    ASSERT_EQ_SIZE(da_size(&a), 19 * sizeof(double));
    da_destroy(&a);
}

static void test_string_pointers(void) {
    DynArray a;
    da_init(&a, sizeof(char*));
    const char* s1 = "hello";
    const char* s2 = "world";
    const char* s3 = "dyn";
    da_append(&a, &s1);
    da_append(&a, &s2);
    da_append(&a, &s3);
    ASSERT_EQ_SIZE(da_size(&a), 3 * sizeof(char*));
    char** got = da_get(&a, 1);
    ASSERT_NOT_NULL(got);
    ASSERT(strcmp(*got, "world") == 0);
    got = da_get(&a, 2);
    ASSERT(strcmp(*got, "dyn") == 0);
    da_destroy(&a);
}

static void test_char(void) {
    DynArray a;
    da_init(&a, sizeof(char));
    const char* word = "claude";
    for (size_t i = 0; i < strlen(word); i++) {
        char c = word[i];
        da_append(&a, &c);
    }
    ASSERT_EQ_SIZE(da_size(&a), 6 * sizeof(char));
    ASSERT_EQ_INT(*(char*)da_get(&a, 0), 'c');
    ASSERT_EQ_INT(*(char*)da_get(&a, 5), 'e');
    da_destroy(&a);
}

static void test_struct_growth(void) {
    DynArray a;
    da_init(&a, sizeof(Point));
    for (int i = 0; i < 200; i++) {
        Point p = {i, -i};
        da_append(&a, &p);
        ASSERT(da_capacity(&a) >= da_size(&a));
    }
    for (int i = 0; i < 200; i++) {
        Point* got = da_get(&a, (size_t)i);
        ASSERT_NOT_NULL(got);
        ASSERT_EQ_INT(got->x, i);
        ASSERT_EQ_INT(got->y, -i);
    }
    da_destroy(&a);
}

int main(void) {
    printf("Running arbitrary-type tests...\n");
    RUN_TEST(test_struct_append_get);
    RUN_TEST(test_struct_modify);
    RUN_TEST(test_struct_remove);
    RUN_TEST(test_struct_remove_large_fields);
    RUN_TEST(test_double);
    RUN_TEST(test_string_pointers);
    RUN_TEST(test_char);
    RUN_TEST(test_struct_growth);
    REPORT();
}
