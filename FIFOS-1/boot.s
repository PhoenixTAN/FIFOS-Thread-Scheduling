# -----------------------------------------------------------------------------------------------
# Author: 
#           Ziqi Tan & Jiaqian Sun
# Reference: 
#           memos-2 reference code
#           https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh
# -----------------------------------------------------------------------------------------------

    .globl stack

    .bss
    .align 0x1000
    .comm stack, 0x1000     # setup 4KB stack area in bss

    .data

    .text

    .globl _start

gdt: 
    # (1) null descriptor -- 1st (Zeroth) entry not used
    .long 0
    .long 0

    # (2) Kernel CS
    # code descriptor
    .long 0x0000FFFF
    .long 0x00CF9A00
    # Base: 0x00000000; Limit: 0xFFFFF
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

_start:
	jmp real_start

	# Multiboot header -- Safe to place this header in 1st page for GRUB
	.align 4            # You are working on a 32-bit environment
	.long 0x1BADB002    # Multiboot magic number
	.long 0x00000003    # Align modules to 4KB, req. mem size
			            # See 'info multiboot' for further info
	.long 0xE4524FFB    # Checksum

real_start:
    lgdt gdt_ptr    # load gtd table address into GDTR register
    ljmp $0x08, $code_segment     # go to the kernel code segment

code_segment: 
    # set up kernel data segment for these registers
    movw $0x10, %AX  
    movw %AX, %DS      
    movw %AX, %SS
    movw %AX, %ES      
    movw %AX, %FS
    movw %AX, %GS

	# set up stack
	movl $stack+0x1000, %ESP # setup 4Kbyte stack
	
	# save multiboot parameter, for eventual call to C code
	pushl %EBX

	call init   # start of C code

	# In case we return from the call, we want to suspend the processor
	
	cli
	hlt     # halts CPU until the next external interrupt is fired
loop:
	jmp loop
