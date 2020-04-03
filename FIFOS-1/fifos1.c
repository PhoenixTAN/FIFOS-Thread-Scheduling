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
void init(multiboot_info_t* pmb);
static void cls (void);
void put_char(char ch);
void put_string(char* string);
void newline(void);

void init(multiboot_info_t* pmb) {
    cls();
    put_string("FIFOS-1: Ziqi Tan, Jiaqian Sun");
    
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

