#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <stdio.h>

static int tests_run = 0;
static int tests_failed = 0;
static const char* current_test = NULL;

#define RUN_TEST(fn) do { \
    current_test = #fn; \
    int _before = tests_failed; \
    tests_run++; \
    fn(); \
    if (tests_failed == _before) { printf("  PASS  %s\n", #fn); } \
} while (0)

#define ASSERT(cond) do { \
    if (!(cond)) { \
        tests_failed++; \
        printf("  FAIL  %s (%s:%d): %s\n", \
               current_test, __FILE__, __LINE__, #cond); \
        return; \
    } \
} while (0)

#define ASSERT_EQ_INT(actual, expected) do { \
    int _a = (actual), _e = (expected); \
    if (_a != _e) { \
        tests_failed++; \
        printf("  FAIL  %s (%s:%d): expected %d, got %d\n", \
               current_test, __FILE__, __LINE__, _e, _a); \
        return; \
    } \
} while (0)

#define ASSERT_EQ_SIZE(actual, expected) do { \
    size_t _a = (actual), _e = (expected); \
    if (_a != _e) { \
        tests_failed++; \
        printf("  FAIL  %s (%s:%d): expected %zu, got %zu\n", \
               current_test, __FILE__, __LINE__, _e, _a); \
        return; \
    } \
} while (0)

#define ASSERT_NULL(p)     ASSERT((p) == NULL)
#define ASSERT_NOT_NULL(p) ASSERT((p) != NULL)

#define REPORT() do { \
    printf("\n%d/%d tests passed\n", tests_run - tests_failed, tests_run); \
    return tests_failed ? 1 : 0; \
} while (0)

#endif
