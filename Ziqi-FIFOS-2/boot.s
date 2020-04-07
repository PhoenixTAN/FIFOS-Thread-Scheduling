# -----------------------------------------------------------------------------------------------
# Author: 
#           Ziqi Tan & Jiaqian Sun
# Reference: 
#           memos-2 reference code
#           https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh
# -----------------------------------------------------------------------------------------------

    .global stack
    .global idt

    .bss
    .align 0x1000
    .comm stack, 0x1000     # setup 4KB stack area in bss

    .data
    .align 0x4

gdt: 
    # (1) null descriptor -- 1st (Zeroth) entry not used
    .long 0
    .long 0

    # (2) Kernel CS
    # code descriptor
    .long 0x0000FFFF
    .long 0x00CF9A00
    # Base: 0x00000000; Limit: 0xFFFFF -> 4GB
    # Flags: 0xC(1100) -> 4KB blocks(page granularity) and 32 bit protected mode
    # Access Byte: 0x9A(10011010)
    #           Pr: 1; Privl: 00(highest); S(Descriptor type): 1; 
    #           Ex(Executable bit): 1 -> code selector

    # (3) Kernel DS
    # data descriptor
    .long 0x0000FFFF
    .long 0x00CF9200
    # Access Byte: 0x92(10010010)   
    #           Ex(Executable bit): 0 -> data selector

    # (4) Task State Segment
    # (5) Other segments if needed: user-level, LDTs, TSS

gdt_ptr:
    # Define the size of gdt
    .short 0x7FF    # length in bytes - 3 descriptors but space for 256 entries
    # Every entry is 8-byte-long. 256 entries are 0x800 bytes.

    # offset: 4 bytes
    .long gdt       # linear address of the table itself

idt:
    # ISRs 0 to 31 handle CPU exceptions
    # .long excep_div_by_zero
    .fill 256, 8, 0
    # ISRs 32 to 47 handle IRQ0 to IRQ15
    # .long irq0_handler

idt_ptr:
    .short 0x7FF
    .long idt

.text
.global _start
.global schedule_finish

_start:
	jmp real_start

	# Multiboot header -- Safe to place this header in 1st page for GRUB
	.align 4            # You are working on a 32-bit environment
	.long 0x1BADB002    # Multiboot magic number
	.long 0x00000003    # Align modules to 4KB, req. mem size
			            # See 'info multiboot' for further info
	.long 0xE4524FFB    # Checksum

real_start:
    cli                 # disable interrupts before setting up idt, PIT and PIC
    lgdt gdt_ptr        # load gdt table address into GDTR register
   
    # TODO: initialize idt
    # load effective address
    lea idt, %EDI
    lea divide_by_zero_exception, %EBX

    movl %EBX, %EAX
    movl %EBX, %EDX
    andl 0x0000FFFF, %EAX   # load EBX[15..0] into (%EDI)
    orl  0x00800000, %EAX   # load 0x08 into selector 2(%EDI)

    movl %EAX, (%EDI)
    addl $4, %EDI

    andl 0xFFFF0000, %EDX
    orl  0x00008E00, %EDX

    movl %EDX, (%EDI)

    lidt idt_ptr        # load idt table address into IDTR register
    sti
    # movw %AX, (%EDI)
    # load EBX[31..16] into 6(%EDI)
    # shrl $16, %EAX
    # movw %AX, 6(%EDI)

    # load 0x08 into selector 2(%EDI)
    # movw $0x08, 2(%EDI)

    # load 0x0 into unused 4(%EDI)
    # movb $0x0, 4(%EDI)

    # load 0x8E into P_DPL_S Gate Type into 5(%EDI)
    # movw $0x8E, 5(%EDI)	
    ljmp $0x08, $1f     # go to the kernel code segment

1: 
    # set up kernel data segment for these registers
    movw $0x10, %AX
    movw %AX, %DS
    movw %AX, %SS
    movw %AX, %ES
    movw %AX, %FS
    movw %AX, %GS

    # set up stack
	movl $stack+0x1000, %ESP # setup 4Kbyte stack
	
    call init   # start of C code

	# In case we return from the call, we want to suspend the processor
schedule_finish:	
	cli     # disable interrupt
	hlt     # halts CPU until the next external interrupt is fired


divide_by_zero_exception:
    hlt
    call isr_context_protection
    call divide_by_zero_handler
    call isr_context_retrieve
