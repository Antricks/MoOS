#include "memory.h"
#include "types.h"

extern void mem_zero(void* ptr_from, void* ptr_until) {
    for(char* i = (char*) ptr_from; i < (char*) ptr_until; i++) {
        *i = 0x00;
    }
}

extern void mem_sync(uint16_t* mem1, uint16_t* mem2, int offset_from, int offset_until) {
    for(int i = (int) offset_from; i < (int) offset_until; i++) {
        mem1[i] = mem2[i];
    }
}

extern void mem_fill_8(void* ptr_from, void* ptr_until, char fill_value) {
    for(char* i = (char*) ptr_from; i <= (char*) ptr_until; i++) {
        *i = fill_value;
    }
}

extern void mem_fill_16(void* ptr_from, void* ptr_until, uint16_t fill_value) {
    for(uint16_t* i = (uint16_t*) ptr_from; i <= (uint16_t*) ptr_until; i++) {
        *i = fill_value;
    }
}
