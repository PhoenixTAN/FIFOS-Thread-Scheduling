/**
 * Author: Ziqi Tan, Jiaqian Sun
 * 
 * Reference: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
*/

#include "multiboot.h"

/* Macros. */

/* Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

/* Some screen stuff. */
/* The number of columns. */
#define COLUMNS                 80
/* The number of lines. */
#define LINES                   24
/* The attribute of an character. */
#define ATTRIBUTE               7
/* The video memory address. */
#define VIDEO                   0xB8000

/* Variables. */
/* Save the X position (column). */
static int xpos;
/* Save the Y position (row). */
static int ypos;
/* Point to the video memory. */
static volatile unsigned char *video;

/* Forward declarations. */
void cmain (unsigned long magic, unsigned long addr);
static void cls (void);
void print_memory_size(multiboot_uint64_t memory_size);
void print_memory_range(multiboot_uint64_t base_addr, multiboot_uint64_t len, multiboot_uint32_t type );
void print_hex_string(multiboot_uint64_t data);
void put_char(char ch);
void put_string(char* string);
void newline(void);


void cmain(unsigned long magic, unsigned long addr) {
    /* Clear the screen. */
    cls ();

    multiboot_info_t *mbi;

    /* Am I booted by a Multiboot-compliant boot loader? */
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        put_string("Invalid magic number:");
        print_hex_string(magic);
        put_char('\n');
        return;
    }

    /* Set MBI to the address of the Multiboot information structure. */
    mbi = (multiboot_info_t *) addr;

    // TODO: a list of condition check

    // init data
    multiboot_memory_map_t *mmap;
    multiboot_uint64_t memory_size = 0;
    xpos = 0;
    ypos = 0;

    // iterate the memory map
    for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr; 
          (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length; 
            mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size))) {       
        // 64 bits base address, 64 bits length and 32 bits type.
        // add up free memory
        if (mmap->type == 1) {
            memory_size += mmap->len;   // the size of the memory region in bytes
        }
        print_memory_range(mmap->addr, mmap->len, mmap->type);
    }

    print_memory_size(memory_size);
}

/* Clear the screen and initialize VIDEO, XPOS and YPOS. */
static void cls (void) {
    int i;

    video = (unsigned char *) VIDEO;
    
    for (i = 0; i < COLUMNS * LINES * 2; i++) {
        *(video + i) = 0;
    }
    
    xpos = 0;
    ypos = 0;
}

void print_memory_size(multiboot_uint64_t memory_size) {
    // convert memory size in B to MB
    memory_size = memory_size >> 20;
    put_string("MemOS 2: Welcome *** System memory is (in MB): ");
    print_hex_string(memory_size);
    put_char('\n');
}

void print_memory_range(multiboot_uint64_t base_addr, multiboot_uint64_t len, multiboot_uint32_t type) {
    put_string("Address range: [");
    print_hex_string(base_addr);
    put_char('~');
    print_hex_string(base_addr + len);
    put_string("] -> ");
    if( type == 1 ) {
        put_string("Free memory (1)\n");
    }
    else if( type == 2 ) {
        put_string("Reserved memory (2)\n");
    }
    else {
        put_string("Others\n");
    }
}

void print_hex_string(multiboot_uint64_t data) {
    char hex_str[19];
    hex_str[0] = '0';
    hex_str[1] = 'x';
    hex_str[18] = '\0';
    multiboot_uint16_t index;
    multiboot_uint64_t temp;
    for( index = 17, temp = 0x0000000f; index > 1; index--, data = data >> 4 ) {
        multiboot_uint64_t hex_number = data & temp;
        if( hex_number > 9 ) {
            hex_number += 87;   // convert a~f to 'a'~'f'
        }
        else {
            hex_number += 48;
        }
        hex_str[index] = hex_number;
    }
    put_string(hex_str);
}

void put_char(char ch) {
    if (ch == '\n' || ch == '\r') {
        newline();
        return;
    }

    *(video + (xpos + ypos * COLUMNS) * 2) = ch & 0xFF;       // ASCII
    *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;   // set up color

    xpos++;
    if (xpos >= COLUMNS) {
        newline();
    }       
}

void newline(void) {
    xpos = 0;
    ypos++;
    if (ypos >= LINES) {
        ypos = 0;
    }
}

void put_string(char* str) {
    multiboot_uint16_t i;
    for( i = 0; str[i] != '\0'; i++ ) {
        put_char(str[i]);
    }
}

