#include "keyboard.h"
#include "types.h"
#include "textio.h"
#include "interrupts.h"

KeyboardDriver::KeyboardDriver(InterruptManager* manager)
: InterruptHandler(0x21, manager),
dataPort(0x60),
commandPort(0x64)
{
    while(commandPort.read() & 0x1)
        dataPort.read();

    commandPort.write(0xae); //activate interrupts
    commandPort.write(0x20); //feedack current state
    uint8_t status = (dataPort.read() | 1) & ~0x10;
    commandPort.write(0x60); //set state
    dataPort.write(status);

    dataPort.write(0xf4); //activate keyboard
}

KeyboardDriver::~KeyboardDriver() {

}

uint32_t KeyboardDriver::handleInterrupt(uint32_t esp) {
    uint8_t key = dataPort.read();

    static bool shift = false;
    static bool ctrl = false;
    static bool alt = false;
    static bool altgr = false;
    
    if(ctrl) {
        switch (key) {
            case(0x26): clear(); break;
        }
    }

    switch(key) {
        case(0x10): if(!ctrl) {if(shift) print("Q"); else if(altgr) print("@"); else print("q");} break;
        case(0x11): if(!ctrl) {if(shift) print("W"); else print("w");} break;
        case(0x12): if(!ctrl) {if(shift) print("E"); else print("e");} break;
        case(0x13): if(!ctrl) {if(shift) print("R"); else print("r");} break;
        case(0x14): if(!ctrl) {if(shift) print("T"); else print("t");} break;
        case(0x15): if(!ctrl) {if(shift) print("Z"); else print("z");} break;
        case(0x16): if(!ctrl) {if(shift) print("U"); else print("u");} break;
        case(0x17): if(!ctrl) {if(shift) print("I"); else print("i");} break;
        case(0x18): if(!ctrl) {if(shift) print("O"); else print("o");} break;
        case(0x19): if(!ctrl) {if(shift) print("P"); else print("p");} break;
        case(0x1A): if(!ctrl) {if(shift) print("Ü"); else print("ü");} break;

        case(0x1E): if(!ctrl) {if(shift) print("A"); else print("a");} break;
        case(0x1F): if(!ctrl) {if(shift) print("S"); else print("s");} break;
        case(0x20): if(!ctrl) {if(shift) print("D"); else print("d");} break;
        case(0x21): if(!ctrl) {if(shift) print("F"); else print("f");} break;
        case(0x22): if(!ctrl) {if(shift) print("G"); else print("g");} break;
        case(0x23): if(!ctrl) {if(shift) print("H"); else print("h");} break;
        case(0x24): if(!ctrl) {if(shift) print("J"); else print("j");} break;
        case(0x25): if(!ctrl) {if(shift) print("K"); else print("k");} break;
        case(0x26): if(!ctrl) {if(shift) print("L"); else print("l");} break;
        case(0x27): if(!ctrl) {if(shift) print("Ö"); else print("ö");} break; 
        case(0x28): if(!ctrl) {if(shift) print("Ä"); else print("ä");} break;

        case(0x2C): if(!ctrl) {if(shift) print("Y"); else print("y");} break;
        case(0x2D): if(!ctrl) {if(shift) print("X"); else print("x");} break;
        case(0x2E): if(!ctrl) {if(shift) print("C"); else print("c");} break;
        case(0x2F): if(!ctrl) {if(shift) print("V"); else print("v");} break;
        case(0x30): if(!ctrl) {if(shift) print("B"); else print("b");} break;
        case(0x31): if(!ctrl) {if(shift) print("N"); else print("n");} break;
        case(0x32): if(!ctrl) {if(shift) print("M"); else print("m");} break;

        case(0x02): if(!ctrl) {if(shift) print("!"); else print("1");} break;
        case(0x03): if(!ctrl) {if(shift) print("\""); else print("2");} break;
        case(0x04): if(!ctrl) {if(shift) print("§"); else print("3");} break;
        case(0x05): if(!ctrl) {if(shift) print("$"); else print("4");} break;
        case(0x06): if(!ctrl) {if(shift) print("%"); else print("5");} break;
        case(0x07): if(!ctrl) {if(shift) print("&"); else print("6");} break;
        case(0x08): if(!ctrl) {if(shift) print("/"); else print("7");} break;
        case(0x09): if(!ctrl) {if(shift) print("("); else print("8");} break;
        case(0x0A): if(!ctrl) {if(shift) print(")"); else print("9");} break;
        case(0x0B): if(!ctrl) {if(shift) print("="); else print("0");} break;
        case(0x0C): if(!ctrl) {if(shift) print("?"); else print("ß");} break;

        case(0x33): if(shift) print(";"); else print(","); break;
        case(0x34): if(shift) print(":"); else print("."); break;
        case(0x35): if(shift) print("_"); else print("-"); break;

        case(0x39): print(" "); break;
        case(0x0F): print("\t"); break;
        case(0x1C): print("\n"); break;
        case(0x0E): print("\b"); break;

        case(0x2A): case(0x36): shift = true; break;
        case(0xAA): case(0xB6): shift = false; break;

        case(0x1D): ctrl = true; break;
        case(0x9D): ctrl = false; break;

        case(0x38): if(last == 0xE0) {altgr = true;} else alt = true; break;
        case(0xB8): if(last == 0xE0) {altgr = false;} else alt = false; break;

        case(0x4B): moveCursor(-1, 0); break;
        case(0x4D): moveCursor(1, 0); break;
        case(0x48): moveCursor(0, -1); break;
        case(0x50): moveCursor(0, 1); break;

        case(0x47): cursor -= cursor % 80; break; // pos1
        case(0x4F): cursor += 79 - cursor % 80; break; // end

        case(0xE0): break;



        default: 
            if(key < 0x80) {
                print_int(key, 0x0a, 16, "0x");
                print("-");
            }
            break;
    }


    

    last = key;
    return esp;
}