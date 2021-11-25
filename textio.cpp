#include "textio.h"
#include "types.h"
#include "memory.h"
#include "clock.h"
#include "mouse.h"

extern uint16_t* VideoMemory = (unsigned short*) 0xb8000;
extern uint16_t* stdout = VideoMemory;
uint32_t cursor;

uint16_t TextMemory[2000];

void render() {
    for(int i = 0; i < 2000; i++) {
        VideoMemory[i] = TextMemory[i];
    }
    if((clockcount/4) % 2 == 1)
        VideoMemory[cursor] = VideoMemory[cursor] & 0x00ff | 0xf000 ;
    
    VideoMemory[(int)(mouseX)+(int)(mouseY)*80] = 0x0f00 | '#';
}

void moveCursor(int xd, int yd) {
    cursor = (cursor + 2000 + xd) % 2000;
    cursor = (cursor + 2000 + yd*80) % 2000;
}

void clear() {
    for(int i = 0; i < 2000; i++) {
        TextMemory[i] = 0;
    }
    cursor = 0;
}

uint32_t printAt(uint32_t position, const char* text, char style = '\x0f') {
    for(int i = 0; true ; i++) {
        if(text[i] == '\n') {
            position += 80-(position%80);
            continue;
        }
        if(text[i] == '\t') {
            mem_fill_16((TextMemory+position),(TextMemory+position+(3-(position%4))), style << 8 & 0xF0FF | 0x0800 | '-');
            position += 4-(position%4);
            continue;
        }
        if(text[i] == '\b') {
            position--;
            TextMemory[position] = 0;
            continue;
        }
        if(text[i] == '\x00') {
            break;
        }
        TextMemory[position] = (uint16_t) style << 8 | text[i];
        position = (position + 1) % (80*25);
    }
    render();

    return position;
}

void print(const char* text, char style = '\x0f') {
    cursor = printAt(cursor, text, style);
}

void print_raw(const char* text, char style) {
    for(int i = 0; true ; i++) {
        if(text[i] == '\n') {
            cursor += 80-(cursor%80);
            continue;
        }
        if(text[i] == '\t') {
            mem_fill_16((VideoMemory+cursor),(VideoMemory+cursor+(3-(cursor%4))), style << 8 & 0xF0FF | 0x0800 | '-');
            cursor += 4-(cursor%4);
            continue;
        }
        if(text[i] == '\b') {
            cursor--;
            VideoMemory[cursor] = 0;
            continue;
        }
        if(text[i] == '\x00') {
            break;
        }
        VideoMemory[cursor] = (uint16_t) style << 8 | text[i];
        cursor = (cursor + 1) % (80*25);
    }
}

void print_int(uint32_t number, char style, uint8_t base, const char* prefix, uint8_t stretch_to){
    print(prefix, style);
    uint16_t digits = 0;
    uint32_t tmp = number;
    if(number == 0) {
        TextMemory[cursor] = style << 8 | 0x30;
        cursor++;
        digits = 1;
    }
    while(tmp){
        tmp = tmp / base;
        digits++;
    }
    if(digits < stretch_to) {
        for(int i = 0; i < stretch_to - digits; ++i) {
            TextMemory[cursor] = style << 8 | 0x30;
            cursor++;
        }
    }
    tmp = number;
    for(uint16_t i = digits-1; tmp; i--){
        if(tmp%base < 10) {
            TextMemory[cursor+i] = style << 8 | (uint8_t)((tmp%base)+0x30);
        } else {
            TextMemory[cursor+i] = style << 8 | (uint8_t)((tmp%base)+0x30+7);
        }
        tmp /= base;
    }
    if(number) {
        cursor+=digits;
    }
    render();
}

void print_int_raw(uint32_t number, char style, uint8_t base, const char* prefix, uint8_t stretch_to){
    print(prefix, style);
    uint16_t digits = 0;
    uint32_t tmp = number;
    if(number == 0) {
        VideoMemory[cursor] = style << 8 | 0x30;
        cursor++;
        digits = 1;
    }
    while(tmp){
        tmp = tmp / base;
        digits++;
    }
    if(digits < stretch_to) {
        for(int i = 0; i < stretch_to - digits; ++i) {
            VideoMemory[cursor] = style << 8 | 0x30;
            cursor++;
        }
    }
    tmp = number;
    for(uint16_t i = digits-1; tmp; i--){
        if(tmp%base < 10) {
            VideoMemory[cursor+i] = style << 8 | (uint8_t)((tmp%base)+0x30);
        } else {
            VideoMemory[cursor+i] = style << 8 | (uint8_t)((tmp%base)+0x30+7);
        }
        tmp /= base;
    }
    if(number) {
        cursor+=digits;
    }
}

void color_test() {
    uint16_t style = 0x0000;
    for(int i = 0; i < 16*16 ; ++i) {
        style = (i*0x0100) % 0x10000;
        if(i==128) {
            print("\n", 0x00);
        }
        TextMemory[cursor] = style | 0x30+(i%8);
        cursor++;
    }
    render();
}

void puts(const char* text) {
    print(text, 0x0f);
    print("\n", 0x00);
}

void memory_dump_dw(void* pointer, unsigned int words, int steps) {
    uint32_t* int_pointer = (uint32_t*) pointer;

    print("\n", 0x00);
    for(int i = 0; i < words; i++) {
        if(i % 4 == 0) {
            print("\n", 0x00);
            print_int((uint32_t) (int_pointer + steps * i), '\x0e', 16, "", 8);
            print("|", '\x0e');
        }
        print_int(*(int_pointer + steps * i), 0x0f, 16, "", 8);
        if(i % 4 != 3) {
            print(" ", 0x00);
        }
    }
    print("\n", 0x00);
}

void memory_dump_w(void* pointer, unsigned int words, int steps) {
    uint16_t* int_pointer = (uint16_t*) pointer;
    for(int i = 0; i < words; i++) {
        if(i % 8 == 0) {
            print("\n", 0x00);
            print_int((uint32_t) (int_pointer + steps * i), '\x0e', 16, "", 8);
            print("|", '\x0e');
        }
        print_int(*(int_pointer + steps * i), 0x0f, 16, "", 4);
        if(i % 8 != 7) {
            print(" ", 0x00);
        }
    }
    print("\n", 0x00);
    print("\n", 0x00);
}

void memory_dump_b(void* pointer, unsigned int words, int steps, int base) {
    uint8_t* int_pointer = (uint8_t*) pointer;
    int stretch = 0;
    switch(base) {
        case(2): stretch = 8; break;
        case(8): stretch = 4; break;
        case(10): stretch = 3; break;
        case(16): stretch = 2; break;
    }
    print("\n", 0x00);
    for(int i = 0; i < words; i++) {
        if(i % 8 == 0) {
            if(base > 2)
                print("\n", 0x00);
            print_int((uint32_t) (int_pointer + steps * i), '\x0e', 16, "", 8);
            print("|", '\x0e');
        }
        print_int(*(int_pointer + steps * i), 0x0f, base, "", stretch);
        if(i% 8 != 7) {
            print(" ", 0x00);
        }
    }
    print("\n", 0x00);
    print("\n", 0x00);
}

void mouseDebug(bool leftDown, bool rightDown, bool wheelDown) {
    if(leftDown) printAt(80*25-3, "L");
    else printAt(80*25-3, "-");

    if(rightDown) printAt(80*25-2, "R");
    else printAt(80*25-2, "-");

    if(wheelDown) printAt(80*25-1, "W");
    else printAt(80*25-1, "-");
}