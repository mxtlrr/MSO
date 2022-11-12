[bits 32]

k_null_desc:
    dd 0x0 ; 4 byte
    dd 0x0 ; 4 byte

k_cseg: 
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

k_dseg:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

;; end
k_end:

gdtr:
  size_gdtr:
    dw k_end - k_null_desc - 1
    dd k_null_desc             ;; FIXME: this throws a warning when 
                               ;; compiled for elf32. maybe it should be dw?

gdt_dataseg equ k_dseg - k_null_desc
gdt_codeseg equ k_cseg - k_null_desc

[global load_gdt]

load_gdt:
  lgdt [gdtr]   ;; GDTR is the pointer to our gdt table
  ;; Loaded!
  
  ;; dataseg is offset in GDT for data segment
  mov ax, gdt_dataseg
  ;; set all data segs
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  ;; uncomment this and you got yourself a triple fault!
  ;; jmp $
  mov ss, ax  ;; <-- triple fault

  ;; far jump and return
  jmp 0x08:.flush
  .flush:
    ret
