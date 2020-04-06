#include "multiboot.h"

/* ISRs 0 to 31 handle CPU exceptions */
#define X86_RESERVE_EXCEPTION 32    

#define USER_DEFINE 1

// Entry of interrupt descriptor table
typedef struct {
    multiboot_uint16_t low_offset;          // [15..0] low part offset for 
    multiboot_uint16_t selector;            // [31..16] selector 
    multiboot_uint8_t unused;               // [32..39] must be set to zero
    multiboot_uint8_t P_DPL_S_Gate_Type;    // [40..47] P[47] 1 | DPL[46..45] 00 | S[44] 0 | Gate Type[43..40] 0xe
    multiboot_uint16_t high_offset;         // [63..48] high part pointer
} idt_entry;

// Interrupt Descriptor Table (IDT)
static idt_entry idt[X86_RESERVE_EXCEPTION+USER_DEFINE];

/*
    initialize idt
    This will be called in boot.s
*/
void init_idt() {
    multiboot_uint32_t interrupt_handler = (multiboot_uint32_t)divide_by_zero_exception;

    idt[0].low_offset = (multiboot_uint16_t)(interrupt_handler & 0xFFFF);
    idt[0].selector = 0x08;
    idt[0].unused = 0;
    idt[0].P_DPL_S_Gate_Type = 0x8E;
    idt[0].high_offset = (multiboot_uint16_t)(interrupt_handler >> 16);

    multiboot_uint16_t length = 8 * 33 - 1;
    multiboot_uint32_t base = (multiboot_uint32_t)(idt);
    multiboot_uint64_t IDTR = (multiboot_uint64_t)base << 16 | length;

    // load IDTR
    __asm__ __volatile__ ("lidt %0"::"m"(IDTR)); 

}


