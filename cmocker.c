//
// Created by cdeler on 10/16/17.
//

#include <sys/mman.h>
#include <zconf.h>

#include "cmocker.h"

static int change_page_permissions_of_address(void *addr, int perms)
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

#define INSTR(_x) ((char)(_x))

int _mock_function_internal(void *originalFunctionAddr, void *mockFunction, void *jmpBuffer)
    {
    int rc;

    rc = change_page_permissions_of_address(originalFunctionAddr, PROT_READ | PROT_WRITE | PROT_EXEC);

    if (!rc)
        {
        int i;
        char *startAddr = (char *) originalFunctionAddr;

        char *jmpBufferAddr = (char *) jmpBuffer;

        for (i = 0; i < 8 + 4; ++i)
            {
            startAddr[i] = jmpBufferAddr[i];
            }
        }

    rc = change_page_permissions_of_address(mockFunction, PROT_READ | PROT_WRITE | PROT_EXEC);
    if (!rc)
        {
        int i;
        char *startAddr = (char *) mockFunction;

        /*   Replace function header:
         *       55                      push   %rbp
         *       48 89 e5                mov    %rsp,%rbp
         *   to
         *       48 31 c0                xor    %rax,%rax
         *       90                      nop
         */

        char jmpBufferAddr[] = {INSTR(0x48), INSTR(0x31), INSTR(0xc0), INSTR(0x90)};

        for (i = 0; i < 4; ++i)
            {
            startAddr[i] = jmpBufferAddr[i];
            }
        }

    return rc;
    }
