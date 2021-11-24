#include "gdt.h"
#include "textio.h"
#include "types.h"
#include "port.h"
#include "memory.h"
#include "interrupts.h"
#include "keyboard.h"
#include "clock.h"
#include "mouse.h"

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void kernelMain(const void* multiboot_structure, unsigned int multiboot_magic) {    
    print("[kernel - kernelMain] initializing GDT... ");
    GlobalDescriptorTable gdt;
    puts("done");

    print("[kernel - kernelMain] instanciating interrupt manager... ");
    InterruptManager interrupts(&gdt);
    puts("done");

    print("[kernel - kernelMain] instanciating clock...");
    Clock clock(&interrupts);
    puts("done");

    print("[kernel - kernelMain] instanciating keyboard handler... ");
    KeyboardDriver keyboard(&interrupts);
    puts("done");

    print("[kernel - kernelMain] instanciating mouse handler... ");
    MouseDriver mouse(&interrupts);
    puts("done");

    print("[kernel - kernelMain] activating interrupt manager... ");
    interrupts.activate();
    puts("done");

    print("\n");
    print("WELCOME TO MoOS!\n", 0x0b);
    print("Currently, this screen is the only thing I can offer here.\nFeel free to move around your mouse or type something nice... Have fun ;D");

    while(true) {

    }
}
