#include "port.h"
#include "types.h"
#include "textio.h"
#include "gdt.h"

Port::Port() {
}

Port::Port(uint16_t portnumber) {
    this->portnumber = portnumber;
}

Port::~Port() {
}


Port8::Port8() : Port() {}
Port8::Port8(uint16_t portnumber) : Port(portnumber) {}

Port8::~Port8() {
}

void Port8::write(uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a" (data) , "Nd" (this->portnumber));
}

uint8_t Port8::read() {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (this->portnumber));
    return result;
}



Port8Slow::Port8Slow() : Port8() {}
Port8Slow::Port8Slow(uint16_t portnumber) : Port8(portnumber) {}

Port8Slow::~Port8Slow() {

}

void Port8Slow::write(uint8_t data) {
    __asm__ volatile("outb %0, %1\njmp 1f\njmp 1f\n1:" : : "a" (data) , "Nd" (this->portnumber));
}

Port16::Port16() : Port() {}
Port16::Port16(uint16_t portnumber) : Port(portnumber) {}

Port16::~Port16() {

}

void Port16::write(uint16_t data) {
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (this->portnumber));
}

uint16_t Port16::read() {
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (this->portnumber));
    return result;
}

Port32::Port32() : Port() {}
Port32::Port32(uint16_t portnumber) : Port(portnumber) {}

Port32::~Port32() {

}

void Port32::write(uint32_t data) {
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (this->portnumber));
}

uint32_t Port32::read() {
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (this->portnumber));
    return result;
}