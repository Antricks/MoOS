#include "interrupts.h"
#include "types.h"
#include "textio.h"
#include "port.h"


InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager) {
    this->interruptNumber = interruptNumber;
    this->interruptManager = interruptManager;
    interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler() {
    if(this->interruptManager->handlers[interruptNumber] == this)
        this->interruptManager->handlers[interruptNumber] = 0;
}

uint32_t InterruptHandler::handleInterrupt(uint32_t esp) {
    puts("Gotta handle this!");
    return esp;
}






InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager* InterruptManager::activeInterruptManager = 0;

void InterruptManager::setInterruptDescriptorTableEntry (
    uint8_t interruptNumber,
    uint16_t codeSegmentSelectorOffset,
    void (*handler)(),
    uint8_t DescriptorPrivilegeLevel,
    uint8_t DescriptorType
) {

    const uint8_t IDT_DESC_PRESENT = 0x80;

    interruptDescriptorTable[interruptNumber].handlerAdressLowBits = ((uint32_t) handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handlerAdressHighBits = (((uint32_t) handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel & 3) << 5); 
    interruptDescriptorTable[interruptNumber].reserved = 0;

}


InterruptManager::InterruptManager(GlobalDescriptorTable* gdt) :
    masterPicCommand(0x20),
    masterPicData(0x21),
    slavePicCommand(0xa0),
    slavePicData(0xa1)
{
    uint16_t CodeSegment = gdt -> CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    
    for(int i = 0; i < 256; i++) {
        handlers[i] = 0;
        setInterruptDescriptorTableEntry(i, CodeSegment, &ignoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }
    
    setInterruptDescriptorTableEntry(0x20, CodeSegment, &handleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x21, CodeSegment, &handleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x2C, CodeSegment, &handleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);


    masterPicCommand.write(0x11);
    slavePicCommand.write(0x11);

    masterPicData.write(0x20);
    slavePicData.write(0x28);

    masterPicData.write(0x04);
    slavePicData.write(0x02);

    masterPicData.write(0x01);
    slavePicData.write(0x01);

    masterPicData.write(0x00);
    slavePicData.write(0x00);

    interruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t) interruptDescriptorTable;

    asm volatile("lidt %0" : : "m" (idt));
}

InterruptManager::~InterruptManager() {}


uint32_t InterruptManager::doHandleInterrupt(uint8_t interruptNumber, uint32_t esp) {

    if(this->handlers[interruptNumber] != 0) {

        esp = this->handlers[interruptNumber]->handleInterrupt(esp);
    
    } else if (interruptNumber != 0x20) {
    
        print("No handler for interrupt: ", 0x0a);
        print("0x", 0x0b);
        print_int(interruptNumber, 0x0b, 16);
        print("\n");
    }

    //Is this a hardware interrupt
    if(0x20 <= interruptNumber && interruptNumber < 0x30) {
        //answer the interrupt
        masterPicCommand.write(0x20);
        if(0x28 <= interruptNumber)
            slavePicCommand.write(0x20);
    }

    return esp;
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    if(activeInterruptManager != 0)
        return activeInterruptManager->doHandleInterrupt(interruptNumber, esp);
    return esp;
}



void InterruptManager::activate() {
    if(activeInterruptManager != 0)
        activeInterruptManager->deactivate();
    activeInterruptManager = this;
    asm("sti");
}

void InterruptManager::deactivate() {
    if(activeInterruptManager == this) {
        activeInterruptManager = 0;
        asm("cli");
    }
}