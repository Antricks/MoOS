#ifndef TEXTIO_H
#define TEXTIO_H

#include "types.h"
#include "memory.h"

extern uint16_t* VideoMemory;
extern uint16_t* stdout;
extern uint32_t cursor;
extern uint16_t TextMemory[2000];

void moveCursor(int xd, int yd);
void clear();
void render();
void print(const char* text, char style, const uint32_t _position);
void print_raw(const char* text, char style = '\x0f');
void print_int(uint32_t number, char style = '\x0f', uint8_t base = 10, const char* prefix = "", uint8_t stretch_to = 0);
void print_int_raw(uint32_t number, char style = '\x0f', uint8_t base = 10, const char* prefix = "", uint8_t stretch_to = 0);
extern void color_test();
extern void puts(const char* text);
extern void memory_dump_dw(void* pointer, unsigned int words, int steps = -1);
extern void memory_dump_w(void* pointer, unsigned int words, int steps = -1);
extern void memory_dump_b(void* pointer, unsigned int words, int steps = -1, int base = 16);
extern void mouseDebug(bool leftDown, bool rightDown, bool wheelDown);

#endif //TEXTIO_H
