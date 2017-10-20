//
// Created by cdeler on 10/20/17.
//

#include <assert.h>
#include <memory.h>

#include "cmocker.h"
#include "vector.h"

void test1_smoke()
    {
    Vector *v = vector_open();

    assert(v->deleter == NULL);
    assert(v->vectorSize == 0);
    assert(v->items == 0);
    assert(v->elementsCount == 0);
    assert(!strcmp(v->eyeCatcher, VECTOR_EYECATCHER_VALUE));

    int rc = vector_close(&v);
    assert(v == NULL);
    assert(rc == 0);
    }

void test2_vector_enlarge()
    {
    size_t i;
    Vector *v = vector_open();
    vector_enlarge(v);

    assert(v->vectorSize == ENLARGE_FACTOR(0));
    assert(v->items);

    for (i = 0; i < v->vectorSize; ++i)
        {
        void *element = v->items[i];

        assert(element == NULL);
        }

    size_t oldSize = v->vectorSize;
    vector_enlarge(v);

    assert(v->vectorSize == ENLARGE_FACTOR(oldSize));
    for (i = 0; i < v->vectorSize; ++i)
        {
        void *element = v->items[i];

        assert(element == NULL);
        }

    vector_close(&v);
    }

void test3_vector_add()
    {
    Vector *v = vector_open();
    vector_set_deleter(v, free);

    char *element = strdup("foo");
    vector_add(v, element);

    assert(v->elementsCount == 1ULL);
    assert(v->vectorSize == ENLARGE_FACTOR(0));
    assert(v->items);

    char *firstElement = v->items[0];
    assert(!strcmp(firstElement, "foo"));

    assert(vector_close(&v) == 1);
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

int main(int argc __cmocker_unused, char **argv __cmocker_unused)
    {
    test1_smoke();
    test2_vector_enlarge();
    test3_vector_add();
    test4_element_remove();

    return 0;
    }