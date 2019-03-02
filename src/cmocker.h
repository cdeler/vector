//
// Created by cdeler on 10/19/17.
//

#pragma once
#ifndef CMOCKER_CMOCKER_H
#define CMOCKER_CMOCKER_H

#define __cmocker_unused __attribute__((unused))
#define __cmocker_used __attribute__((used))

int cmocker_mock(void *originalFunction, void *mockFunction) __attribute__((nonnull (1, 2)));
int cmocker_restore_origin(void *originalFunction)  __attribute__((nonnull (1)));

#endif //CMOCKER_CMOCKER_H
