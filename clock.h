#ifndef __CLOCK_H
#define __CLOCK_H

#include "types.h"
#include "textio.h"
#include "interrupts.h"

extern uint32_t clockcount;

class Clock : public InterruptHandler {
public:
    Clock(InterruptManager* manager);
    ~Clock();

    InterruptManager* manager;
    virtual uint32_t handleInterrupt(uint32_t esp);
};

#endif