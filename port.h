#ifndef PORT_H
#define PORT_H

#include "types.h"
#include "textio.h"
#include "gdt.h"

class Port {
public:
    Port();
    Port(uint16_t portnumber);
    ~Port();
    uint16_t portnumber;
};

class Port8 : public Port {
public:
    Port8();
    Port8(uint16_t portnumber);
    ~Port8();
    void write(uint8_t data);
    uint8_t read();
};

class Port8Slow : public Port8 {
public:
    Port8Slow();
    Port8Slow(uint16_t portnumber);
    ~Port8Slow();
    void write(uint8_t data);
    uint8_t read();
};

class Port16 : public Port {
public:
    Port16();
    Port16(uint16_t portnumber);
    ~Port16();
    void write(uint16_t data);
    uint16_t read();
};

class Port32 : public Port {
public:
    Port32();
    Port32(uint16_t portnumber);
    ~Port32();
    void write(uint32_t data);
    uint32_t read();
};

#endif