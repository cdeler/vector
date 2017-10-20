//
// Created by cdeler on 10/19/17.
//

#pragma once
#ifndef CMOCKER_CMOCKER_H
#define CMOCKER_CMOCKER_H

#define __cmocker_unused __attribute__((unused))
#define __cmocker_used __attribute__((used))

int cmocker_mock(void *originalFunction, void *mockFunction);

#endif //CMOCKER_CMOCKER_H
