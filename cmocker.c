//
// Created by cdeler on 10/16/17.
//

#include <sys/mman.h>
#include <zconf.h>
#include <stdint.h>

#include "utils/vector.h"
#include "cmocker.h"

typedef struct
    {
    void *functionAddr;
    size_t originHeader;
    size_t mockedHeader;
    } FunctionHandle;

static Vector *_functionHandles = NULL;

static void __attribute__((constructor)) __cmocker_used
init()
    {
    _functionHandles = vector_open();
    }

static void __attribute__((destructor)) __cmocker_used
finit()
    {
    vector_close(&_functionHandles);
    }

static int
change_page_permissions_of_address(void *addr, int perms)
    {
    // Move the pointer to the page boundary
    size_t page_size = (size_t) getpagesize();
    addr -= (size_t) addr % page_size;

    if (mprotect(addr, page_size, perms) == -1)
        {
        return -1;
        }

    return 0;
    }

#define JMP_OP ((uint8_t)0xE9)

int
cmocker_mock(void *originalFunction, void *mockFunction)
    {
    int rc = 0;

    size_t *mockFunctionWords = (size_t *) mockFunction;
    size_t *originalFunctionWords = (size_t *) originalFunction;

    int32_t offset = (int64_t) mockFunctionWords - ((int64_t) originalFunctionWords + 5 * sizeof(char));

    rc = change_page_permissions_of_address(originalFunction, PROT_READ | PROT_WRITE | PROT_EXEC);

    if (!rc)
        {
        size_t instruction = (size_t) (JMP_OP | offset << 8);
        *originalFunctionWords = instruction;
        }

    return rc;
    }
