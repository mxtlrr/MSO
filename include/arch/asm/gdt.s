[bits 32]

;; 0x0000 -> 0x0008
k_null_desc:
  dd 0x00
  dd 0x00

;; kernel mode data segment,
;; 0x0008 -> 0x0010
k_dseg:
  dw 0xffff
  dw 0x0000 ;; base low
  db 0x00   ;; base medium
  db 0x9a   ;; access byte
  db 0xcf   ;; flags + upper limit
  db 0x00   ;; base high


;; kernel mode code segment
;; 0x0010 -> 0x0018
k_cseg:
  ;; same thing as k_dseg
  dw 0xffff
  dw 0x0000
  db 0x00
  db 0x9a
  db 0xcf
  db 0x00

;; end
k_end:


gdtr:
  size_gdtr:
    dw k_end - k_null_desc - 1
    dq k_null_desc             ;; FIXME: this throws a warning when 
                               ;; compiled for elf32. maybe it should be dw?

gdt_dataseg equ k_dseg - k_null_desc


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
