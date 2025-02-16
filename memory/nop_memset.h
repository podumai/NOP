#ifndef NOP_MEMORY_NOP_MEMSET_H /* Begin memory nop_memset header file */
#define NOP_MEMORY_NOP_MEMSET_H 1UL

#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

void* nop_memset(void* dst, unsigned char val, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* End memory nop_memset header file */
