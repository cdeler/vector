//
// Created by cdeler on 10/20/17.
//

#pragma once
#ifndef CMOCKER_VECTOR_H
#define CMOCKER_VECTOR_H

#include <stddef.h>

#define VECTOR_EYECATCHER_VALUE ("EYE")

typedef void (*VectorDeleter)(void *vectorItem);

typedef void *(*VectorCopier)(const void *vectorItem);

typedef int (*VectorItemComparator)(const void *item1, const void *item2);

typedef struct _vector Vector;

Vector *vector_open(void) __attribute__ ((warn_unused_result));

size_t vector_close(Vector **ppVector) __attribute__((nonnull (1)));

int vector_pushBack(Vector *pVector, void *item) __attribute__((nonnull (1)));

void *vector_removeAt(Vector *pVector, size_t index) __attribute__((nonnull (1)));

void *vector_elementAt(Vector *pVector, size_t index) __attribute__((nonnull (1)));

size_t vector_getLength(Vector *pVector) __attribute__((nonnull (1)));

void vector_set_deleter(Vector *pVector, VectorDeleter deleter) __attribute__((nonnull (1)));

int vector_insertAt(Vector *pVector, size_t index, void *value) __attribute__((nonnull (1)));

int vector_resize(Vector *pVector, size_t newSize) __attribute__((nonnull (1)));

Vector *vector_concat(Vector *first, Vector *second);

void vector_sort(Vector *pVector, VectorItemComparator cmp) __attribute__((nonnull (1, 2)));

void vector_shuffle(Vector *pVector) __attribute__((nonnull (1)));

#ifdef __cplusplus
extern "C"
{
#endif

size_t vector_random_get_next(size_t min, size_t max);

void vector_random_seed(int seed);

#ifdef __cplusplus
};
#endif
#endif //CMOCKER_VECTOR_H
