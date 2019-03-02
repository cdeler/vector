//
// Created by cdeler on 10/20/17.
//

#include <assert.h>
#include <stddef.h>
#include <memory.h>

#include "vector.h"

void test1_smoke()
	{
	Vector *v = vector_open();

	assert(v->deleter == NULL);
	assert(v->vectorSize == 0);
	assert(v->items == 0);
	assert(v->elementsCount == 0);
	assert(!strcmp(v->eyeCatcher, VECTOR_EYECATCHER_VALUE));

	size_t rc = vector_close(&v);
	assert(v == NULL);
	assert(rc == 0);
	}

void test2_vector_enlarge()
	{
	Vector *v = vector_open();
	vector_enlarge(v);

	assert(v->vectorSize == ENLARGE_FACTOR(0));
	assert(v->items);
	assert(vector_getLength(v) == 0);

	size_t oldSize = v->vectorSize;
	vector_enlarge(v);

	assert(v->vectorSize == ENLARGE_FACTOR(oldSize));
	assert(v->items);
	assert(vector_getLength(v) == 0);

	vector_close(&v);
	}

void test3_vector_add()
	{
	int rc;
	Vector *v = vector_open();
	vector_set_deleter(v, free);

	char *element = strdup("foo");
	vector_add(v, element);

	assert(v->elementsCount == 1ULL);
	assert(v->vectorSize == ENLARGE_FACTOR(0));
	assert(v->items);

	char *firstElement = v->items[0];
	assert(!strcmp(firstElement, "foo"));

	rc = vector_close(&v);
	assert(rc == 1);
	}

void test4_element_remove()
	{
	Vector *v = vector_open();

	vector_add(v, "first");
	assert(v->elementsCount == 1);
	assert(vector_removeAt(v, 0) == 0);
	assert(v->elementsCount == 0);

	vector_add(v, "first");
	vector_add(v, "second");
	vector_add(v, "third");
	assert(v->elementsCount == 3);
	assert(vector_removeAt(v, 1) == 0);
	assert(v->elementsCount == 2);
	assert(!strcmp(v->items[0], "first"));
	assert(!strcmp(v->items[1], "third"));

	assert(vector_removeAt(v, 1) == 0);
	assert(v->elementsCount == 1);
	assert(!strcmp(v->items[0], "first"));

	vector_close(&v);

	v = vector_open();
	vector_set_deleter(v, free);
	char c;
	int i;
	char buffer[2] = "X";
	for (c = '0'; c <= '9'; ++c)
		{
		*buffer = c;
		vector_add(v, strdup(buffer));
		}
	assert(v->elementsCount == 10);
	assert(v->vectorSize == 10);

	assert(vector_removeAt(v, 9) == 0);
	assert(v->elementsCount == 9);
	for (c = '0', i = 0; c <= '8'; ++c, ++i)
		{
		*buffer = c;
		char *element = v->items[i];

		assert(!strcmp(buffer, element));
		}

	assert(vector_removeAt(v, 0) == 0);
	assert(v->elementsCount == 8);
	for (c = '1', i = 0; c <= '8'; ++c, ++i)
		{
		*buffer = c;
		char *element = v->items[i];

		assert(!strcmp(buffer, element));
		}

	assert(vector_removeAt(v, 3) == 0);
	assert(v->elementsCount == 7);
	for (c = '1', i = 0; c <= '3'; ++c, ++i)
		{
		*buffer = c;
		char *element = v->items[i];

		assert(!strcmp(buffer, element));
		}
	for (c = '5'; c <= '8'; ++c, ++i)
		{
		*buffer = c;
		char *element = v->items[i];

		assert(!strcmp(buffer, element));
		}

	assert(vector_removeAt(v, 5555) < 0);
	assert(v->elementsCount == 7);

	vector_close(&v);

	}


void test5_multiple_adds()
	{
	size_t i, val;
	Vector *pVector = vector_open();
	vector_set_deleter(pVector, NULL);

	for (i = 0; i < 10000000; ++i)
		{
		vector_add(pVector, (void *) i);
		}

	for (i = 0; i < 10000000; ++i)
		{
		val = (size_t) vector_elementAt(pVector, i);
		assert(val == i);
		}

	vector_close(&pVector);
	}

int main()
	{
	test1_smoke();
	test2_vector_enlarge();
	test3_vector_add();
	test4_element_remove();
	test5_multiple_adds();

	return 0;
	}