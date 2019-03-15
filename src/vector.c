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
vector_resize(Vector *this, size_t newSize)
	{
	int rc;

	assert(this);

	rc = -1;

	if (this && this->vectorSize < newSize)
		{
		if (this->vectorSize != 0)
			{
			assert(this->items);

			this->vectorSize = newSize;
			this->items = realloc(this->items, newSize * sizeof(void *));

			assert(this->items);
			}
		else
			{
			assert(this->items == NULL);

			this->vectorSize = newSize;
			this->items = malloc(newSize * sizeof(void *));

			assert(this->items);
			}

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
vector_concat(Vector *this, Vector *other)
	{
	size_t newElementsCount, newVectorSize, i;

	if (this == NULL)
		{
		this = vector_open();

		assert(this);

		if (other)
			{
			this->copier = other->copier;
			this->deleter = other->deleter;
			}
		}

	if (other)
		{
		assert(this->copier == other->copier);

		newElementsCount = this->elementsCount + other->elementsCount;
		newVectorSize = (newElementsCount < this->vectorSize) ?
		                this->vectorSize :
		                newElementsCount;

		vector_resize(this, newVectorSize);

		if (other->copier)
			{
			const VectorCopier copier = other->copier;

			for (i = 0; i < other->elementsCount; ++i)
				{
				void *value = vector_elementAt(other, i);
				void *copy = copier(value);
				vector_pushBack(this, copy);
				}
			}
		else
			{
			void *dest = this->items + this->elementsCount;
			const void *source = other->items;

			memmove(dest, source, other->elementsCount * sizeof(void *));

			this->elementsCount = newElementsCount;
			}
		}

	return this;
	}