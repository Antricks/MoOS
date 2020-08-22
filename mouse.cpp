#include "mouse.h"
#include "types.h"
#include "textio.h"
#include "interrupts.h"

double mouseX;
double mouseY;

MouseDriver::MouseDriver(InterruptManager* manager)
: InterruptHandler(0x2C, manager),
dataPort(0x60),
commandPort(0x64)
{
    offset = 0;
    buttons = 0;

    mouseX = 40;
    mouseY = 12;

    while(commandPort.read() & 0x1)
        dataPort.read();

    commandPort.write(0xab); //activate interrupts
    commandPort.write(0x20); //feedack current state
    uint8_t status = (dataPort.read() | 2) & ~0x10;
    commandPort.write(0x60); //set state
    dataPort.write(status);


    commandPort.write(0xd4);
    dataPort.write(0xf4);
    dataPort.read();
}

MouseDriver::~MouseDriver() {

}

uint32_t MouseDriver::handleInterrupt(uint32_t esp) {
    uint8_t status = commandPort.read();
    
    signed int xd = 0;
    signed int yd = 0;

    if(!(status & 0x20)) {
        return esp;
    } 
    
    buffer[offset] = dataPort.read();
    offset = (offset + 1) % 3;

    if(offset == 0) {
        mouseX += buffer[1]/10;
        
        if(mouseX > 79)
            mouseX = 79;
        if(mouseX < 0)
            mouseX = 0;

        mouseY -= buffer[2]/10;
        
        if(mouseY > 24)
            mouseY = 24;
        if(mouseY < 0)
            mouseY = 0;
    
        for(uint8_t i = 0; i < 3; i++) {
            if((buffer[3] & ( 1 << i)) != (buttons & ( 1 << i))) {
                print("button press");
                cursor = mouseX+mouseY*80;
            }
        }
        buttons = buffer[3];
    }
    return esp;
}