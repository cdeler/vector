//
// Created by cdeler on 10/20/17.
//

#include <assert.h>

#include "vector.h"

Vector *vector_open(void)
    {
    Vector *result = calloc(sizeof(Vector), 1);

    return result;
    }

int vector_close(Vector **ppVector)
    {
    assert(ppVector);
    int rc = -1;

    if (ppVector)
        {
        Vector *pVector = *ppVector;

        if (pVector)
            {
            size_t i;
            rc = 0;
            VectorDeleter deleter = pVector->deleter ? pVector->deleter : free;

            for (i = 0; i < pVector->vectorSize; ++i)
                {
                void *item = pVector->items + i;

                deleter(item);
                rc += 1;
                }

            free(pVector->items);
            }

        *ppVector = NULL;
        }

    return rc;
    }

void set_deleter(Vector *pVector, VectorDeleter deleter)
    {
    assert(pVector);

    pVector->deleter = deleter;
    }
