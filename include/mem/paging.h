#pragma once
#include <stdint.h>

extern void load_page_dir(unsigned int*);
extern void enable_paging();

void setup_pg();
