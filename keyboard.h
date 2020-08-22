#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "types.h"
#include "textio.h"
#include "interrupts.h"
#include "port.h"

class KeyboardDriver : public InterruptHandler {
    Port8 dataPort;
    Port8 commandPort;

    uint8_t last;
public:
    KeyboardDriver(InterruptManager* manager);
    ~KeyboardDriver();

    InterruptManager* manager;
    virtual uint32_t handleInterrupt(uint32_t esp);
};

#endif