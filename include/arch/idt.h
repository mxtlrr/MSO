/* interrupts */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define KERNEL_CS 0x08 // :)

typedef struct {
  uint16_t isr_low;
  uint16_t kernel_cs;
  uint8_t  reserved;
  uint8_t  attrib;
  uint16_t isr_high;
}__attribute__((packed)) idt_entry_t;

typedef struct {
  uint16_t limit;
  uint32_t base;
}__attribute__((packed)) idtr_t;


typedef struct registers {
   uint32_t ds;                  // Data segment selector
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
   uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

__attribute__((noreturn))
void exception_handler(registers_t regs);

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);

extern void* isr_stub_table[];
void idt_init(void);

void irq_handler(registers_t regs);
