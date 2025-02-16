#ifndef NOP_MEMORY_NOP_MEMCPY_H /* Begin memory nop_memcpy header file */
#define NOP_MEMORY_NOP_MEMCPY_H 1UL

#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

void* nop_memcpy(void* __restrict__ dst, const void* __restrict__ src, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* End memory nop_memcpy header file */
