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

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len]) len++;
	return len;
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
}

void puts(char* data) {
  for(int i = 0; data[i]; i++) terminal_putchar(data[i]); 
}


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
          puts(itoa(va_arg(ap, int), 10));
          break;

        case 'x':
          puts(itoa(va_arg(ap, uint32_t), 16));
          break;
      }
    } else {
      char terminated[2] = {*ptr, 0};
      puts(terminated);
    }
  }
}

