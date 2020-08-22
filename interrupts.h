#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "types.h"
#include "textio.h"
#include "port.h"
#include "gdt.h"

class InterruptManager;





class InterruptHandler {
protected:
    uint8_t interruptNumber;
    InterruptManager* interruptManager;   

    InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager);
    ~InterruptHandler();

public:
    virtual uint32_t handleInterrupt(uint32_t esp);
};




class InterruptManager {

friend class InterruptHandler;
protected:

    static InterruptManager* activeInterruptManager;

    InterruptHandler* handlers[256];

    struct GateDescriptor {
        uint16_t handlerAdressLowBits;
        uint16_t gdt_codeSegmentSelector;
        uint8_t reserved;
        uint8_t access;
        uint16_t handlerAdressHighBits;

    } __attribute__((packed));

    static GateDescriptor interruptDescriptorTable[256];

    struct interruptDescriptorTablePointer {
        uint16_t size;
        uint32_t base;
    } __attribute__((packed));

    static void setInterruptDescriptorTableEntry(
        uint8_t interruptNumber,
        uint16_t codeSegmentSelectorOffset,
        void (*handler)(),
        uint8_t DescriptorPrivilegeLevel,
        uint8_t DescriptorType
    );

    Port8Slow masterPicCommand;
    Port8Slow masterPicData;
    Port8Slow slavePicCommand;
    Port8Slow slavePicData;

public:
    InterruptManager(GlobalDescriptorTable* gdt);
    ~InterruptManager();

    void activate();
    void deactivate();

    static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
    uint32_t doHandleInterrupt(uint8_t interruptNumber, uint32_t esp);

    static void ignoreInterruptRequest();
    static void handleInterruptRequest0x00();
    static void handleInterruptRequest0x01();
    static void handleInterruptRequest0x0C();
};

#endif