#include "clock.h"
#include "types.h"
#include "textio.h"
#include "interrupts.h"

uint32_t clockcount = 0;

Clock::Clock(InterruptManager* manager) : InterruptHandler(0x20, manager) {}
Clock::~Clock() {}

uint32_t Clock::handleInterrupt(uint32_t esp) {
    clockcount = (clockcount + 1) % 4294967296;
    render();
    return esp;
}