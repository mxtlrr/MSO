#include "libc/stdio.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint16_t *video_memory = (uint16_t*)0xB8000;

const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;


size_t get_terminal_row() {
  return terminal_row;
}

void set_terminal_row(size_t n){
  terminal_row = n;
}

// **************************************

size_t get_terminal_column() {
  return terminal_column;
}

void set_terminal_column(size_t n){
  terminal_column = n;
}
void update_cursor(int x, int y){
	uint16_t pos = y * VGA_WIDTH + x;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len]) len++;
	return len;
}

void move_cursor(){
  uint16_t curPos = terminal_column * 80 + terminal_row;
  outb(0x3d4, 14);
  outb(0x3d5, curPos >> 8);
  outb(0x3d4, 15);
  outb(0x3d5, curPos);
}

void scroll(){
   // Get a space character with the default colour attributes.
   uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
   uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

   // Row 25 is the end, this means we need to scroll up
   if(terminal_column >= 25){
       // Move the current text chunk that makes up the screen
       // back in the buffer by a line
       int i;
       for (i = 0*80; i < 24*80; i++){
           video_memory[i] = video_memory[i+80];
       }

       // The last line should now be blank. Do this by writing
       // 80 spaces to it.
       for (i = 24*80; i < 25*80; i++) video_memory[i] = blank;

       // The cursor should now be on the last line.
       terminal_column = 24;
   }
} 

void terminal_setcolor(uint8_t color) {
  terminal_color = color;
}

uint8_t backColour = 0;
uint8_t foreColour = 15;

void set_colors(uint8_t bg, uint8_t fg){
  backColour = bg;
  foreColour = fg;
}

void terminal_putchar(char c){
   // The attribute byte is made up of two nibbles - the lower being the
   // foreground colour, and the upper the background colour.
   uint8_t attributeByte = (backColour << 4) | (foreColour & 0x0F);
   // The attribute byte is the top 8 bits of the word we have to send to the
   // VGA board.
   uint16_t attribute = attributeByte << 8;
   uint16_t *location;

   // Handle a backspace, by moving the cursor back one space
   if (c == 0x08 && terminal_row) terminal_row--;
   else if (c == 0x09) terminal_row = (terminal_row+8) & ~(8-1);
   else if (c == '\r') terminal_row = 0;
   else if (c == '\n') {
       terminal_row = 0;
       terminal_column++;
   }
   // Handle any other printable character.
   else if(c >= ' ') {
       location = video_memory + (terminal_column*80 + terminal_row);
       *location = c | attribute;
       terminal_row++;
   }

   // Check if we need to insert a new line because we have reached the end
   // of the screen.
   if (terminal_row >= 80){
       terminal_row = 0;
       terminal_column++;
   }

   // Scroll the screen if needed.
   scroll();
   move_cursor();
}

void puts(char* data) {
  for(int i = 0; data[i]; i++) terminal_putchar(data[i]); 
}


#define putc(x) terminal_putchar(x)

void printf(char* fmt, ...){
  va_list ap;
  va_start(ap, fmt);
  char* ptr;

  for (ptr = fmt; *ptr != '\0'; ++ptr) {
    if (*ptr == '%') {
      ++ptr;
      switch (*ptr) {
        case 's':
          puts(va_arg(ap, char*));
          break;
        
        case 'd':
          puts((char*)itoa(va_arg(ap, uint32_t), 10));
          break;

        case 'x':
          puts((char*)itoa(va_arg(ap, uint32_t), 16));
          break;

        case 'c':
          terminal_putchar(va_arg(ap, int));
          break;
      }
    } else {
      char terminated[2] = {*ptr, 0};
      puts(terminated);
    }
  }
}


void assertFailed(char* file, int line, char* failed_expr){
  set_colors(0x0, 0x4);
  printf("ASSERTION \"%s\" FAILED (IN FILE %s, ON LINE %d)\n", failed_expr, file, line);
  set_colors(0x0, 0x7);
}

void assertSucess(char* successful_expr){
  set_colors(0x0, 0xa);
  printf("The assertion of \"%s\" is true.\n", successful_expr);
  set_colors(0x0, 0x7);
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end){
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void putc_at_pos(uint8_t c, uint8_t fg, uint8_t bg, int x, int y){
  uint16_t attrib = (bg << 4) | (fg & 0x0F);
	volatile uint16_t* where;
	where = (volatile uint16_t*)0xB8000 + (y * 80 + x) ;
	*where = c | (attrib << 8);
}