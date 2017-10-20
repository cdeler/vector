//
// Created by cdeler on 10/20/17.
//

#include <assert.h>
#include <memory.h>

#include "vector.h"

void vector_enlarge(Vector *pVector)
    {
    assert(pVector);

    if (pVector)
        {
        if (!pVector->vectorSize)
            {
            assert(pVector->items == NULL);

            pVector->vectorSize = ENLARGE_FACTOR(pVector->vectorSize);
            pVector->items = calloc(pVector->vectorSize, sizeof(void *));
            }
        else
            {
            assert(pVector->items);

            size_t oldSize = pVector->vectorSize;
            pVector->vectorSize = ENLARGE_FACTOR(pVector->vectorSize);
            void **items = calloc(pVector->vectorSize, sizeof(void *));

            memmove(items, pVector->items, oldSize);
            free(pVector->items);

            pVector->items = items;
            }
        }

    }

Vector *vector_open(void)
    {
    Vector *result = calloc(1, sizeof(Vector));
    strcpy(result->eyeCatcher, VECTOR_EYECATCHER_VALUE);
    return result;
    }

int vector_add(Vector *pVector, void *item)
    {
    assert(pVector);
    assert(item);

    int rc = -1;

    if (pVector && item)
        {
        if (pVector->elementsCount == pVector->vectorSize)
            vector_enlarge(pVector);

        void **pLastElement = pVector->items + pVector->elementsCount;

        *pLastElement = item;

        pVector->elementsCount += 1;
        rc = 0;
        }

    return rc;
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

            for (i = 0; i < pVector->elementsCount; ++i)
                {
                void *item = pVector->items[i];

                if (pVector->deleter)
                    pVector->deleter(item);

                rc += 1;
                }

            free(pVector->items);
            free(pVector);
            }

        *ppVector = NULL;
        }

    return rc;
    }

void vector_set_deleter(Vector *pVector, VectorDeleter deleter)
    {
    assert(pVector);

    pVector->deleter = deleter;
    }

int vector_removeAt(Vector *pVector, size_t index)
    {
    assert(pVector);
    assert (index < pVector->elementsCount);
    int rc = -1;

    if (index < pVector->elementsCount)
        {
        void *element = pVector->items[index];

        if (pVector->deleter)
            pVector->deleter(element);

        size_t vectorTailSize = pVector->elementsCount - (index + 1);

        if (vectorTailSize)
            memmove(pVector->items + index, pVector->items + index + 1, vectorTailSize * sizeof(void *));

        pVector->items[pVector->elementsCount - 1] = NULL;

        pVector->elementsCount -= 1;
        rc = 0;
        }

    return rc;
    }