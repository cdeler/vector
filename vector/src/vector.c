//
// Created by cdeler on 10/20/17.
//

#include <assert.h>
#include <memory.h>
#include <malloc.h>

#include "vector.h"

#define ENLARGE_FACTOR(_x) (10ULL + (_x) * 3 / 2)

struct _vector
{
	char eyeCatcher[sizeof(VECTOR_EYECATCHER_VALUE)];
	size_t vectorSize;
	size_t elementsCount;
	void **items;

	VectorDeleter deleter;
};

static void _vector_enlarge(Vector *pVector) __attribute__((nonnull (1)));

static void
_vector_enlarge(Vector *pVector)
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
vector_pushBack(Vector *pVector, void *item)
	{
	assert(pVector);

	int rc = -1;

	if (pVector)
		{
		if (pVector->elementsCount == pVector->vectorSize)
			{
			_vector_enlarge(pVector);
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
	size_t rc = 0, i;

	if (ppVector)
		{
		Vector *pVector = *ppVector;

		if (pVector)
			{
			if (pVector->deleter)
				{
				for (i = 0; i < pVector->elementsCount; ++i)
					{
					pVector->deleter(pVector->items[i]);
					}
				}

			rc = pVector->elementsCount;

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


void *
vector_removeAt(Vector *pVector, size_t index)
	{
	assert(pVector);
	void *result = NULL;

	if (pVector && index < pVector->elementsCount)
		{
		result = pVector->items[index];

		size_t vectorTailSize = pVector->elementsCount - (index + 1);

		if (vectorTailSize)
			{
			memmove(pVector->items + index, pVector->items + index + 1, vectorTailSize * sizeof(void *));
			}

		pVector->elementsCount -= 1;
		}

	return result;
	}

int
vector_insertAt(Vector *pVector, size_t index, void *value)
	{
	assert(pVector);
	int rc = -1;
	size_t tailSize;

	if (pVector)
		{
		if (index == pVector->elementsCount)
			{
			rc = vector_pushBack(pVector, value);
			}
		else if (index < pVector->elementsCount)
			{
			if (pVector->elementsCount == pVector->vectorSize)
				{
				_vector_enlarge(pVector);
				}

			tailSize = pVector->elementsCount - index;

			memmove(pVector->items + (index + 1), pVector->items + index, tailSize * sizeof(void *));
			pVector->items[index] = value;

			++pVector->elementsCount;

			rc = 0;
			}
		}

	return rc;
	}