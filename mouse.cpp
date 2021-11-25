#include "mouse.h"
#include "types.h"
#include "textio.h"
#include "interrupts.h"

double mouseX;
double mouseY;

bool leftDown;
bool rightDown;
bool wheelDown;

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
    
    if(!(status & 0x20)) {
        return esp;
    } 
    
    buffer[offset] = dataPort.read();
    offset = (offset + 1) % 3;

    if(offset == 0) {
        mouseX += buffer[1]/16;
        
        if(mouseX > 79)
            mouseX = 79;
        if(mouseX < 0)
            mouseX = 0;

        mouseY -= buffer[2]/16;
        
        if(mouseY > 24)
            mouseY = 24;
        if(mouseY < 0)
            mouseY = 0;


        buttons = buffer[0] & 0x07;
    
        if(buttons) { // left down
            leftDown = buttons & 0x01;
            rightDown = buttons & 0x02;
            wheelDown = buttons & 0x04;
        } else { // release
            if(leftDown)
                cursor = mouseX+mouseY*80;

            leftDown = false;
            rightDown = false;
            wheelDown = false;
        }

        mouseDebug(leftDown, rightDown, wheelDown);
    }
    return esp;
}