/* interrupts */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <arch/isr.h> /* isr handler */
#include <arch/irq.h> /* irq handler */

#define KERNEL_CS 0x08 // :)

struct idt_entry_struct {
	uint16_t base_low;
	uint16_t sel;
	uint8_t  zero;
	uint8_t  flags;
	uint16_t base_high;
}__attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct {
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;


extern void idt_flush(uint32_t);

void init_idt();
void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t);
