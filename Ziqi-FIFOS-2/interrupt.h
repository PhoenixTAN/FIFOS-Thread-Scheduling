#include "multiboot.h"

static void outb (multiboot_uint8_t uch, multiboot_uint16_t usPort) {
    __asm__ __volatile__("outb %0, %1"::"a"(uch), "Nd"(usPort));
}

void send_eoi() {
    outb(0x20, 0x20);
}

void init_pic() {
    outb(0x11, 0x20);
    outb(0x20, 0x21);
    outb(0x0D, 0x21);
}

void init_pit() {
    outb(0x34, 0x43);
    outb(0, 0X40);
    outb(0, 0x40);
}

void interrupt_handler() {
    outb(0x20 , 0x20);
    print("int ");
    // scheduler
    fifo_scheduler();
}