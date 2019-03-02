//
// Created by cdeler on 10/20/17.
//

#pragma once
#ifndef CMOCKER_VECTOR_H
#define CMOCKER_VECTOR_H

#include <stddef.h>

#define VECTOR_EYECATCHER_VALUE ("EYE")

typedef void (*VectorDeleter)(void *vectorItem);

typedef struct _vector Vector;

Vector *vector_open(void);
size_t vector_close(Vector **ppVector) __attribute__((nonnull (1)));

int vector_pushBack(Vector *pVector, void *item) __attribute__((nonnull (1)));
int vector_removeAt(Vector *pVector, size_t index) __attribute__((nonnull (1)));
void *vector_elementAt(Vector *pVector, size_t index) __attribute__((nonnull (1)));
size_t vector_getLength(Vector *pVector) __attribute__((nonnull (1)));

void vector_set_deleter(Vector *pVector, VectorDeleter deleter) __attribute__((nonnull (1)));

int vector_insertAt(Vector *pVector, size_t index, void *value) __attribute__((nonnull (1)));

#endif //CMOCKER_VECTOR_H
