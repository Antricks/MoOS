# sudo apt-get install g++ binutils libc6-dev-i386

GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o kernel.o gdt.o memory.o textio.o port.o interrupts.o interruptstubs.o keyboard.o clock.o mouse.o



%.o: %.cpp
	gcc $(GCCPARAMS) -c -o $@ $<

%.o: %.s
	as $(ASPARAMS) -o $@ $<

moos.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: moos.bin
	sudo cp $< /boot/moos.bin

moos.iso: moos.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp moos.bin iso/boot/moos.bin
	cp grub.cfg iso/boot/grub
	grub-mkrescue --output=moos.iso iso
	rm -rf iso

run: moos.iso
	VirtualBox --startvm 'My OS'

clean:
	rm *.o
	rm moos.bin
	rm moos.iso
	rm -r iso
