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

# Need to fill in GDT
gdt: 
    # (1) null descriptor -- 1st (Zeroth) entry not used

    # (2) Kernel CS
    # code descriptor

    # (3) Kernel DS
    # data descriptor

gdt_ptr:
    # size: 2 bytes
    .short 0x7FF    # length in bytes - 3 descriptors but space for 256
    
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
	
	# set up stack
	movl $stack+0x1000, %ESP # setup 4Kbyte stack
	
	# save multiboot parameter, for eventual call to C code
	pushl %EBX

	call init   # start of C code

	# In case we return from the call, we want to suspend the processor
	
	cli
	hlt
loop:
	jmp loop
