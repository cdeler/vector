//
// Created by cdeler on 10/19/17.
//

#pragma once
#ifndef CMOCKER_CMOCKER_H
#define CMOCKER_CMOCKER_H

#define DECLARE_MOCK(_mock_func_name) \
	void cmocker_ ## _mock_func_name ## _mock() \
	{asm volatile("jmpq *%0": : "r" (_mock_func_name));}

#define mock_function(_original, _mock_fname) _mock_function_internal(_original, cmocker_ ## _mock_fname ## _mock)

int _mock_function_internal(void *originalFunctionAddr, void *jmpBuffer) __attribute__((nonnull(1, 2)));

#endif //CMOCKER_CMOCKER_H
