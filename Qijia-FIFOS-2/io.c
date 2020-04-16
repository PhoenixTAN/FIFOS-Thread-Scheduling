#include "io.h"

static unsigned short *videoram = (unsigned short *)0xB8000; //Base address of the VGA frame buffer
static int attrib = 0x0F;                                    //black background, white foreground
static int csr_x = 0, csr_y = 0;

#define COLS 80
#define ROWS 25

void printc(char c)
{
    if (c == 0x09)
    { // Tab (move to next multiple of 8)
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    else if (c == '\r')
    { // CR
        csr_x = 0;
    }
    else if (c == '\n')
    { // LF (unix-like)
        csr_x = 0;
        csr_y++;
    }
    else if (c >= ' ')
    {                                                             // Printable characters
        *(videoram + (csr_y * COLS + csr_x)) = c | (attrib << 8); // Put the character w/ attributes
        csr_x++;
    }
    if (csr_x >= COLS)
    {
        csr_x = 0;
        csr_y++;
    } // wrap around!
}

void prints(char *s)
{
    while (*s)
    {
        printc(*s++);
    }
}

void printd(unsigned d)
{
    unsigned char buffer[10];
    unsigned char *p = buffer;
    if (!d)
    {
        printc('0');
        return;
    }
    do
    {
        *p++ = d % 10 + '0';
        d /= 10;
    } while (d);
    while (p-- > buffer)
    {
        printc(*p);
    }
}

void printl(unsigned l)
{
    char buffer[9];
    char *p = buffer + 8;
    *p = '\0';
    while (p-- > buffer)
    {
        char c = l & 0xf;
        *p = (c < 10 ? '0' : 'A' - 10) + c;
        l >>= 4;
    }
    prints(buffer);
}

void clear(void)
{
    int i = COLS * ROWS;
    while (i--)
    {
        printc(' ');
    }
    csr_x = csr_y = 0;
}
