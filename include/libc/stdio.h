#pragma once

#include "libc/string.h"
#include "io.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

enum vga_color {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
size_t strlen(const char* str);
void printf(char* fmt, ...);

void scroll();

void set_colors(uint8_t bg, uint8_t fg);

void move_cursor();

#define kassert(expr)\
  if(!(expr)) assertFailed(__FILE__, __LINE__, #expr);\
  else assertSucess(#expr)\

void assertFailed(char* file, int line, char* failed_expr);
void assertSucess(char* successful_expr);


void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

size_t get_terminal_row();
void   set_terminal_row(size_t n);

size_t get_terminal_column();
void   set_terminal_column(size_t n);

void update_cursor(int x, int y);