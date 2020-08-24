# MoOS
> The MoOS is a simple operating system written in C++ and GNU Assembler based on the tutorial on https://wyoos.org/

To create a VM bootable image file use `make moos.iso` and use the file as boot image in your VM of choice. In this case VirtualBox was used for development.  
Actually booting this OS on a live mashine is very experimental and it should not be expected to work on a modern mashine. 
To try booting MoOS this way, use `make moos.bin` and copy the resulting file into */boot/*, then create a menu entry in the grub config (*/boot/grub/grub.cfg*) the following way:

```
menuentry "MoOS" {
    multiboot /boot/moos.bin 1
    boot
    systemd.unit=rescue.target
}
``` 

I recommend being careful editing the grub config as it has the potential of breaking the boot process. If you don't know exactly what you're doing there, better just let it be or don't blame me if something goes wrong. You are better of using a VM anyway.
