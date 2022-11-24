[global load_page_dir]
[global enable_paging]

load_page_dir:
  ;; get first argument
  push ebp
  mov ebp, esp
  mov eax, [esp+8]

  ;; eax now has the first argument
  ;; which should be the page directory
  mov cr3, eax   ;; put address cr3

  ;; exit
  mov esp, ebp
  pop ebp
  ret

enable_paging:
	push ebp
	mov ebp, esp

  mov eax, cr0
  or eax, 0x80000000
  mov cr0, eax

  ;; exit
	mov esp, ebp
	pop ebp
	ret
