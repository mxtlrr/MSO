#include "arch/interrupts/irq1.h"

#define KEY_PRESS_LIM 0x81 // anything < 0x81 is a press
                           // anything > 0x81 is a release.
#define LIM_X         0x01 // this will probably change
                           // once a shell is implemented
                           // but for onow it works perfectly fine


const char kbd_table[] ={
	0, 0, '1', '2',
	'3', '4', '5', '6',
	'7', '8', '9', '0',
	'-', '=', 0, 0,
	'q', 'w', 'e', 'r',
	't', 'y', 'u', 'i',
	'o', 'p', '[', ']',
	0, 0, 'a', 's',
	'd', 'f', 'g', 'h',
	'j', 'k', 'l', ';',
	'\'', '`', 0, '\\',
	'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',',
	'.', '/', 0, '*',
	0, ' '
};

int lshift = 0;
int rshift = 0;

#define KBD_LSHIFT_ON 0x2a
#define KBD_RSHIFT_ON 0x36

#define KBD_LSHIFT_OFF 0xaa
#define KBD_RSHIFT_OFF 0xb6

void kbd_handle(registers_t r){
  uint8_t c     = inb(0x60);
  uint8_t _char = 0;

  if(c < 0x3a) _char = kbd_table[c];

  if(_char != 0){
    switch(lshift | rshift){
      case 0:
        terminal_putchar(_char);
        break;
      case 1:
        terminal_putchar(_char - 32);
        break;
    }
  } else {
    // Special characters.
    switch(c){
      // left shift (on/off)
      case KBD_LSHIFT_ON:
        lshift = 1;
        break;
      case KBD_LSHIFT_OFF:
        lshift = 0;
        break;

      // right shift
      case KBD_RSHIFT_ON:
        rshift = 1;
        break;
      case KBD_RSHIFT_OFF:
        rshift = 0;
        break;

      // Enter.
      case 0x9c:
        terminal_putchar('\n');
        break;

      // Backspace
      case 0x8e:
        asm("nop");
        size_t f = get_terminal_row();
        if(!(f < LIM_X)){
          update_cursor(f-1, get_terminal_column());
          set_terminal_row(f-1);
        
          // TODO: remove the character we are currently on
        }
        break;

    }
  }
}

void init_kbd(){
  reg_int_handler(IRQ1, &kbd_handle);
  serial_puts("Init IRQ1...\n");
}
