/* The number of columns. */
#define COLUMNS                 80
/* The number of lines. */
#define LINES                   24
/* The attribute of an character. */
#define ATTRIBUTE               7
/* The video memory address. */
#define VIDEO                   0xB8000

/* Variables. */
static int xpos;                        /* Save the X position (column). */
static int ypos;                        /* Save the Y position (row). */
static volatile unsigned char *video;   /* Point to the video memory. */

static void cls (void);             /* Clear the screen and initialize VIDEO, XPOS and YPOS. */
void put_char(char ch);             /* print a char */ 
void print(char* string);           /* print a string without switching to a new line*/
void println(char* string);         /* print a string and switch to a new line */
void newline(void);                 /* switch to a new line */

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

void print(char* str) {
    int i;
    for( i = 0; str[i] != '\0'; i++ ) {
        put_char(str[i]);
    }
}

void println(char* str) {
    int i;
    for( i = 0; str[i] != '\0'; i++ ) {
        put_char(str[i]);
    }
    newline();
}
