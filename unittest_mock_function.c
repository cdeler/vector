//
// Created by cdeler on 10/19/17.
//
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdarg.h>

#include "cmocker.h"

static char *__globalTestState = NULL;

static void
setUpTearDown()
    {
    if (__globalTestState)
        free(__globalTestState);

    __globalTestState = NULL;
    }

void
test1_original(void)
    {
    __globalTestState = strdup("test1_original");
    }

int
test2_original(void)
    {
    __globalTestState = strdup("test2_original");
    return 5;
    }

int
test3_original(int *pOutput, int input)
    {
    __globalTestState = strdup("test3_original");
    *pOutput = 5;
    return input + 5;
    }

int
test4_original(int argc, ...)
    {
    int i, result = 0;

    __globalTestState = strdup("test4_original");

    va_list va;

    va_start(va, argc);

    for (i = 0; i < argc; ++i)
        {
        int arg = va_arg(va, int);

        result += arg;
        }

    va_end(va);
    return result;
    }

void
test5_original()
    {
    __globalTestState = strdup("test5_original");
    }

void
test1_mock(void)
    {
    __globalTestState = strdup("test1_mock");
    }

int
test2_mock(void)
    {
    __globalTestState = strdup("test2_mock");
    return 10;
    }

int
test3_mock(int *pOutput, int input)
    {
    __globalTestState = strdup("test3_mock");
    *pOutput = 10;
    return input + 10;
    }

int
test4_mock(int argc, ...)
    {
    int i, result = 0;

    __globalTestState = strdup("test4_mock");

    va_list va;

    va_start(va, argc);

    for (i = 0; i < argc; ++i)
        {
        int arg = va_arg(va, int);

        result += arg;
        }

    va_end(va);

    return result + 10;
    }

void
test5_mock()
    {
    __globalTestState = strdup("test5_mock");
    }

void
test1_smoke()
    {
    setUpTearDown();

    cmocker_mock(test1_original, test1_mock);
    test1_original();

    assert(__globalTestState != NULL);
    assert(!strcmp("test1_mock", __globalTestState));

    setUpTearDown();
    }

void
test2()
    {
    setUpTearDown();

    cmocker_mock(test2_original, test2_mock);
    int actual = test2_original();

    assert(__globalTestState != NULL);
    assert(!strcmp("test2_mock", __globalTestState));
    assert(actual == 10);

    setUpTearDown();
    }

void
test3()
    {
    setUpTearDown();

    cmocker_mock(test3_original, test3_mock);

    int var1 = 0;
    int actual = test3_original(&var1, 0);

    assert(__globalTestState != NULL);
    assert(!strcmp("test3_mock", __globalTestState));
    assert(actual == 10);
    assert(var1 == 10);

    setUpTearDown();
    }

void
test4()
    {
    setUpTearDown();

    cmocker_mock(test4_original, test4_mock);

    int actual = test4_original(5, 1, 1, 1, 1, 1);
    assert(__globalTestState != NULL);
    assert(!strcmp("test4_mock", __globalTestState));
    assert(actual == 15);

    setUpTearDown();
    }

void
test5_mock_twice()
    {
    setUpTearDown();

    cmocker_mock(test5_original, test5_mock);
    int rc = cmocker_mock(test5_original, test5_mock);

    assert(rc < 0);

    test5_original();

    assert(__globalTestState != NULL);
    assert(!strcmp("test5_mock", __globalTestState));

    setUpTearDown();
    }

int
main(int argc __cmocker_unused, char **argv __cmocker_unused)
    {
    test1_smoke();
    test2();
    test3();
    test4();
    test5_mock_twice();

    return 0;
    }