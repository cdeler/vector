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

#if 0
static const char commands[] =
        {
                //0x48, 0x8d, 0x05, 0x08, 0x00, 0x00, 0x00, // lea    0x8(%rip),%rax
                //0x50,                                     // push   %rax
                0xb8, 0xc4, 0x08, 0x40, 0x00,             // mov    $0x400854,%eax
                0xff, 0xe0                                // jmpq   *%rax
        };

    //asm volatile("leaq 8(%rip), %rax");
    //asm volatile("pushq %rax");
    //asm volatile("jmp *%0": : "r" (function2_mock));
    //asm volatile("goback_label:");

#endif

#if 0
static void fill_mov_buffer(char *buffer, uint32_t addr)
    {
    int i;
    for (i = 0; i < 4; ++i)
        {
        buffer[i] = (char)(addr & 0xFF);
        addr >>= 8;
        }
    }

    ...

    uint32_t ptrVal = (uint32_t)le32toh(mockFunctionAddr);

    char commandBuffer[] =
            {
                    0xb8, 0x00, 0x00, 0x00, 0x00, // mov    (0000000),%eax
                    0xff, 0xe0                    // jmpq   *%rax
            };

    fill_mov_buffer(commandBuffer + 1, ptrVal);
#endif

int _mock_function_internal(void *originalFunctionAddr, void *jmpBuffer)
    {
    int rc;

    rc = change_page_permissions_of_address(originalFunctionAddr, PROT_READ | PROT_WRITE | PROT_EXEC);

    if (!rc)
        {
        int i;
        char *startAddr = (char *) originalFunctionAddr;

        /*   Skip function header:
         *      55                      push   %rbp
         *      48 89 e5                mov    %rsp,%rbp
         */

        char *jmpBufferAddr = (char *) jmpBuffer + 4;

        for (i = 0; i < 8; ++i)
            {
            startAddr[i] = jmpBufferAddr[i];
            }
        }

    return rc;
    }
