#pragma once

#include "io.h"
#include "libc/string.h"

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

void serial_putc(char c);
void serial_puts(char* fmt);
