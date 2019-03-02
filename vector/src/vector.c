//
// Created by cdeler on 10/20/17.
//

#include <assert.h>
#include <memory.h>

#include "vector.h"


void
vector_enlarge(Vector *pVector)
	{
	assert(pVector);

	if (pVector)
		{
		if (!pVector->vectorSize)
			{
			assert(pVector->items == NULL);

			pVector->vectorSize = ENLARGE_FACTOR(pVector->vectorSize);
			pVector->items = malloc(pVector->vectorSize * sizeof(void *));

			assert(pVector->items);
			}
		else
			{
			assert(pVector->items);

			pVector->vectorSize = ENLARGE_FACTOR(pVector->vectorSize);
			const size_t newSize = pVector->vectorSize * sizeof(void *);
			pVector->items = realloc(pVector->items, newSize);

			assert(pVector->items);
			}
		}

	}

Vector *
vector_open(void)
	{
	Vector *result = calloc(1, sizeof(Vector));
	assert(result);

	strcpy(result->eyeCatcher, VECTOR_EYECATCHER_VALUE);
	return result;
	}

int
vector_add(Vector *pVector, void *item)
	{
	assert(pVector);

	int rc = -1;

	if (pVector)
		{
		if (pVector->elementsCount == pVector->vectorSize)
			{
			vector_enlarge(pVector);
			}

		pVector->items[pVector->elementsCount++] = item;

		rc = 0;
		}

	return rc;
	}

size_t
vector_close(Vector **ppVector)
	{
	assert(ppVector);
	size_t rc = 0;

	if (ppVector)
		{
		Vector *pVector = *ppVector;

		if (pVector)
			{
			size_t i;

			if (pVector->deleter)
				{
				for (i = 0, rc = 0; i < pVector->elementsCount; ++i, ++rc)
					{
					void *item = pVector[i];

					pVector->deleter(item);

					rc += 1;
					}

				rc = i;
				}

			free(pVector->items);
			free(pVector);
			}

		*ppVector = NULL;
		}

	return rc;
	}

void
vector_set_deleter(Vector *pVector, VectorDeleter deleter)
	{
	assert(pVector);

	pVector->deleter = deleter;
	}

void *
vector_elementAt(Vector *pVector, size_t index)
	{
	assert(pVector);

	if (pVector && index < pVector->elementsCount)
		{
		return pVector->items[index];
		}

	return NULL;
	}

size_t
vector_getLength(Vector *pVector)
	{
	assert(pVector);

	return pVector ? pVector->elementsCount : 0;
	}

int
vector_removeAt(Vector *pVector, size_t index)
	{
	assert(pVector);
	int rc = -1;

	if (pVector && index < pVector->elementsCount)
		{
		void *element = pVector[index];

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