//
// Created by cdeler on 10/20/17.
//

#include <assert.h>
#include <stddef.h>
#include <memory.h>
#include <stdlib.h>

#include "vector.h"

void test1_smoke()
	{
	Vector *v = vector_open();

	assert(v);
	assert(vector_getLength(v) == 0);
	assert(!strcmp((void *) v, VECTOR_EYECATCHER_VALUE));

	size_t rc = vector_close(&v);
	assert(v == NULL);
	assert(rc == 0);
	}

void test3_vector_add()
	{
	size_t rc;
	Vector *pVector = vector_open();
	vector_set_deleter(pVector, free);

	char *element = strdup("foo");
	vector_pushBack(pVector, element);

	assert(vector_getLength(pVector) == 1ULL);

	char *firstElement = vector_elementAt(pVector, 0);
	assert(!strcmp(firstElement, "foo"));

	rc = vector_close(&pVector);
	assert(rc == 1);
	}

void test4_element_remove()
	{
	Vector *pVector = vector_open();

	vector_pushBack(pVector, "first");
	assert(vector_getLength(pVector) == 1);
	assert(vector_removeAt(pVector, 0) == 0);
	assert(vector_getLength(pVector) == 0);

	vector_pushBack(pVector, "first");
	vector_pushBack(pVector, "second");
	vector_pushBack(pVector, "third");
	assert(vector_getLength(pVector) == 3);
	assert(vector_removeAt(pVector, 1) == 0);
	assert(vector_getLength(pVector) == 2);
	assert(!strcmp(vector_elementAt(pVector, 0), "first"));
	assert(!strcmp(vector_elementAt(pVector, 1), "third"));

	assert(vector_removeAt(pVector, 1) == 0);
	assert(vector_getLength(pVector) == 1);
	assert(!strcmp(vector_elementAt(pVector, 0), "first"));

	vector_close(&pVector);

	pVector = vector_open();
	vector_set_deleter(pVector, free);
	char c;
	size_t i;
	char buffer[2] = "X";
	for (c = '0'; c <= '9'; ++c)
		{
		*buffer = c;
		vector_pushBack(pVector, strdup(buffer));
		}
	assert(vector_getLength(pVector) == 10);

	assert(vector_removeAt(pVector, 9) == 0);
	assert(vector_getLength(pVector) == 9);
	for (c = '0', i = 0; c <= '8'; ++c, ++i)
		{
		*buffer = c;
		assert(!strcmp(vector_elementAt(pVector, i), buffer));
		}

	assert(vector_removeAt(pVector, 0) == 0);
	assert(vector_getLength(pVector) == 8);
	for (c = '1', i = 0; c <= '8'; ++c, ++i)
		{
		*buffer = c;
		assert(!strcmp(vector_elementAt(pVector, i), buffer));
		}

	assert(vector_removeAt(pVector, 3) == 0);
	assert(vector_getLength(pVector) == 7);
	for (c = '1', i = 0; c <= '3'; ++c, ++i)
		{
		*buffer = c;
		assert(!strcmp(vector_elementAt(pVector, i), buffer));
		}
	for (c = '5'; c <= '8'; ++c, ++i)
		{
		*buffer = c;
		assert(!strcmp(vector_elementAt(pVector, i), buffer));
		}

	assert(vector_removeAt(pVector, 5555) < 0);
	assert(vector_getLength(pVector) == 7);

	vector_close(&pVector);

	}

#define TEST5_VECTOR_SIZE (10000000ULL)
void test5_multiple_adds()
	{
	size_t i, val, N;
	Vector *pVector = vector_open();
	vector_set_deleter(pVector, NULL);

	for (i = 0; i < TEST5_VECTOR_SIZE; ++i)
		{
		vector_pushBack(pVector, (void *) i);
		}

	N = vector_getLength(pVector);
	assert(TEST5_VECTOR_SIZE == N);

	for (i = 0; i < TEST5_VECTOR_SIZE; ++i)
		{
		val = (size_t) vector_elementAt(pVector, i);
		assert(val == i);
		}

	val = vector_close(&pVector);

	assert(val == TEST5_VECTOR_SIZE);
	}

int main()
	{
	test1_smoke();
	test3_vector_add();
	test4_element_remove();
	test5_multiple_adds();

	return 0;
	}