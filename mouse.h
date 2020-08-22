#ifndef __MOUSE_H
#define __MOUSE_H

#include "types.h"
#include "textio.h"
#include "interrupts.h"
#include "port.h"

extern double mouseX;
extern double mouseY;

class MouseDriver : public InterruptHandler {
    Port8 dataPort;
    Port8 commandPort;

    int8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;

public:
    MouseDriver(InterruptManager* manager);
    ~MouseDriver();

    InterruptManager* manager;
    virtual uint32_t handleInterrupt(uint32_t esp);
};

#endif