#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

extern void mem_zero(void* ptr_from, void* ptr_until);
extern void mem_sync(uint16_t* mem1, uint16_t* mem2, int offset_from, int offset_until);
extern void mem_fill_8(void* ptr_from, void* ptr_until, char fill_value);
extern void mem_fill_16(void* ptr_from, void* ptr_until, uint16_t fill_value);

#endif //MEMORY_H
