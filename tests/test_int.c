#include <stdio.h>
#include "../src/dynarray.h"
#include "test_common.h"

static void test_init_empty(void) {
    DynArray a;
    da_init(&a, sizeof(int));
    ASSERT_EQ_SIZE(da_size(&a), 0);
    ASSERT_NULL(da_get(&a, 0));
    da_destroy(&a);
}

static void test_append_and_get(void) {
    DynArray a;
    da_init(&a, sizeof(int));
    int v = 42;
    da_append(&a, &v);
    ASSERT_EQ_SIZE(da_size(&a), sizeof(int));
    int* p = da_get(&a, 0);
    ASSERT_NOT_NULL(p);
    ASSERT_EQ_INT(*p, 42);
    da_destroy(&a);
}

static void test_append_growth(void) {
    DynArray a;
    da_init(&a, sizeof(int));
    for (int i = 0; i < 100; i++) {
        da_append(&a, &i);
    }
    ASSERT_EQ_SIZE(da_size(&a), 100 * sizeof(int));
    ASSERT(da_capacity(&a) >= 100 * sizeof(int));
    for (int i = 0; i < 100; i++) {
        int* p = da_get(&a, (size_t)i);
        ASSERT_NOT_NULL(p);
        ASSERT_EQ_INT(*p, i);
    }
    da_destroy(&a);
}

static void test_get_out_of_bounds(void) {
    DynArray a;
    da_init(&a, sizeof(int));
    ASSERT_NULL(da_get(&a, 0));
    int v = 1;
    da_append(&a, &v);
    ASSERT_NULL(da_get(&a, 1));
    ASSERT_NULL(da_get(&a, 999));
    da_destroy(&a);
}

static void test_modify_in_bounds(void) {
    DynArray a;
    da_init(&a, sizeof(int));
    for (int i = 0; i < 5; i++) da_append(&a, &i);
    int nv = 99;
    da_set(&a, 2, &nv);
    ASSERT_EQ_INT(*(int*)da_get(&a, 2), 99);
    ASSERT_EQ_INT(*(int*)da_get(&a, 1), 1);
    ASSERT_EQ_INT(*(int*)da_get(&a, 3), 3);
    da_destroy(&a);
}

static void test_modify_out_of_bounds_is_noop(void) {
    DynArray a;
    da_init(&a, sizeof(int));
    for (int i = 0; i < 3; i++) da_append(&a, &i);
    int v = 7;
    da_set(&a, 100, &v);
    ASSERT_EQ_SIZE(da_size(&a), 3 * sizeof(int));
    ASSERT_EQ_INT(*(int*)da_get(&a, 0), 0);
    ASSERT_EQ_INT(*(int*)da_get(&a, 1), 1);
    ASSERT_EQ_INT(*(int*)da_get(&a, 2), 2);
    da_destroy(&a);
}

static void test_remove_middle(void) {
    DynArray a;
    da_init(&a, sizeof(int));
    for (int i = 0; i < 5; i++) da_append(&a, &i);  // 0,1,2,3,4
    da_remove(&a, 2);                                // 0,1,3,4
    ASSERT_EQ_SIZE(da_size(&a), 4 * sizeof(int));
    ASSERT_EQ_INT(*(int*)da_get(&a, 0), 0);
    ASSERT_EQ_INT(*(int*)da_get(&a, 1), 1);
    ASSERT_EQ_INT(*(int*)da_get(&a, 2), 3);
    ASSERT_EQ_INT(*(int*)da_get(&a, 3), 4);
    da_destroy(&a);
}

static void test_remove_middle_multibyte_values(void) {
    // Values chosen so each int has non-zero high bytes; a one-byte-per-step
    // shift in da_remove would leave the higher bytes of shifted elements stale.
    DynArray a;
    da_init(&a, sizeof(int));
    int vals[] = {0x01020304, 0x10203040, 0x11223344, 0x55667788, 0x7FFFFFFF};
    for (int i = 0; i < 5; i++) da_append(&a, &vals[i]);
    da_remove(&a, 2);  // expect: vals[0], vals[1], vals[3], vals[4]
    ASSERT_EQ_SIZE(da_size(&a), 4 * sizeof(int));
    ASSERT_EQ_INT(*(int*)da_get(&a, 0), 0x01020304);
    ASSERT_EQ_INT(*(int*)da_get(&a, 1), 0x10203040);
    ASSERT_EQ_INT(*(int*)da_get(&a, 2), 0x55667788);
    ASSERT_EQ_INT(*(int*)da_get(&a, 3), 0x7FFFFFFF);
    da_destroy(&a);
}

static void test_remove_first(void) {
    DynArray a;
    da_init(&a, sizeof(int));
    for (int i = 0; i < 3; i++) da_append(&a, &i);
    da_remove(&a, 0);
    ASSERT_EQ_SIZE(da_size(&a), 2 * sizeof(int));
    ASSERT_EQ_INT(*(int*)da_get(&a, 0), 1);
    ASSERT_EQ_INT(*(int*)da_get(&a, 1), 2);
    da_destroy(&a);
}

static void test_remove_last(void) {
    DynArray a;
    da_init(&a, sizeof(int));
    for (int i = 0; i < 3; i++) da_append(&a, &i);
    da_remove(&a, 2);
    ASSERT_EQ_SIZE(da_size(&a), 2 * sizeof(int));
    ASSERT_EQ_INT(*(int*)da_get(&a, 0), 0);
    ASSERT_EQ_INT(*(int*)da_get(&a, 1), 1);
    da_destroy(&a);
}

static void test_remove_only(void) {
    DynArray a;
    da_init(&a, sizeof(int));
    int v = 7;
    da_append(&a, &v);
    da_remove(&a, 0);
    ASSERT_EQ_SIZE(da_size(&a), 0);
    da_destroy(&a);
}

static void test_pop(void) {
    DynArray a;
    da_init(&a, sizeof(int));
    for (int i = 0; i < 3; i++) da_append(&a, &i);  // 0,1,2
    int out = -1;
    da_pop(&a, &out);
    ASSERT_EQ_INT(out, 2);
    ASSERT_EQ_SIZE(da_size(&a), 2 * sizeof(int));
    da_pop(&a, &out);
    ASSERT_EQ_INT(out, 1);
    da_pop(&a, &out);
    ASSERT_EQ_INT(out, 0);
    ASSERT_EQ_SIZE(da_size(&a), 0);
    da_destroy(&a);
}

static void test_capacity_invariant(void) {
    DynArray a;
    da_init(&a, sizeof(int));
    for (int i = 0; i < 50; i++) {
        da_append(&a, &i);
        ASSERT(da_capacity(&a) >= da_size(&a));
    }
    da_destroy(&a);
}

int main(void) {
    printf("Running int tests...\n");
    RUN_TEST(test_init_empty);
    RUN_TEST(test_append_and_get);
    RUN_TEST(test_append_growth);
    RUN_TEST(test_get_out_of_bounds);
    RUN_TEST(test_modify_in_bounds);
    RUN_TEST(test_modify_out_of_bounds_is_noop);
    RUN_TEST(test_remove_middle);
    RUN_TEST(test_remove_middle_multibyte_values);
    RUN_TEST(test_remove_first);
    RUN_TEST(test_remove_last);
    RUN_TEST(test_remove_only);
    RUN_TEST(test_pop);
    RUN_TEST(test_capacity_invariant);
    REPORT();
}
