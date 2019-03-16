//
// Created by cdeler on 10/20/17.
//

#include <assert.h>
#include <memory.h>
#include <malloc.h>
#include <stdlib.h>

#include "vector.h"

#define ENLARGE_FACTOR(_x) (10ULL + (_x) * 3 / 2)

struct _vector
{
	char eyeCatcher[sizeof(VECTOR_EYECATCHER_VALUE)];
	size_t vectorSize;
	size_t elementsCount;
	void **items;

	VectorDeleter deleter;
	VectorCopier copier;
};

__attribute__((always_inline))
static void _vector_enlarge(Vector *pVector) __attribute__((nonnull (1)));

static void
_vector_enlarge(Vector *pVector)
	{
	vector_resize(pVector, ENLARGE_FACTOR(pVector->vectorSize));
	}

int
vector_resize(Vector *pVector, size_t newSize)
	{
	int rc;

	assert(pVector);

	rc = -1;

	if (pVector && pVector->vectorSize < newSize)
		{
		if (pVector->vectorSize != 0)
			{
			assert(pVector->items);

			pVector->vectorSize = newSize;
			pVector->items = realloc(pVector->items, newSize * sizeof(void *));
			}
		else
			{
			assert(pVector->items == NULL);

			pVector->vectorSize = newSize;
			pVector->items = malloc(newSize * sizeof(void *));
			}

		assert(pVector->items);

		rc = 0;
		}

	return rc;
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
			rc = pVector->elementsCount;

			if (pVector->items)
				{
				if (pVector->deleter)
					{
					for (i = 0; i < pVector->elementsCount; ++i)
						{
						pVector->deleter(pVector->items[i]);
						}
					}

				free(pVector->items);
				}

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

Vector *
vector_concat(Vector *first, Vector *second)
	{
	size_t newElementsCount, newVectorSize, i;

	if (first == NULL)
		{
		first = vector_open();

		assert(first);

		if (second)
			{
			first->copier = second->copier;
			first->deleter = second->deleter;
			}
		}

	if (second)
		{
		assert(first->copier == second->copier);

		newElementsCount = first->elementsCount + second->elementsCount;
		newVectorSize = (newElementsCount < first->vectorSize) ?
		                first->vectorSize :
		                newElementsCount;

		vector_resize(first, newVectorSize);

		if (second->copier)
			{
			const VectorCopier copier = second->copier;

			for (i = 0; i < second->elementsCount; ++i)
				{
				void *value = vector_elementAt(second, i);
				void *copy = copier(value);
				vector_pushBack(first, copy);
				}
			}
		else
			{
			void *dest = first->items + first->elementsCount;
			const void *source = second->items;

			memmove(dest, source, second->elementsCount * sizeof(void *));

			first->elementsCount = newElementsCount;
			}
		}

	return first;
	}

void
vector_sort(Vector *pVector, VectorItemComparator cmp)
	{
	assert(pVector);
	assert(cmp);

	qsort(pVector->items, pVector->elementsCount, sizeof(void *), cmp);
	}

void
vector_shuffle(Vector *pVector)
	{
	size_t i, j;
	void *buf;

	const size_t N = pVector->elementsCount;

	for (i = N - 1; i >= 1; --i)
		{
		j = vector_random_get_next(0, i);

		if (j != i)
			{
			buf = pVector->items[i];
			pVector->items[i] = pVector->items[j];
			pVector->items[j] = buf;
			}
		}
	}