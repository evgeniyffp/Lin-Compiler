section .data
	msg139635787247704 db "Hello, World!", 10, "", 0
	msg139635787248248 db "x = %d", 10, "y = %d", 10, "z = %d", 10, "", 0

section .text
	extern __malloc
	extern __free
	extern __malloc_init
	extern __malloc_deinit
	extern __strlen
	extern __printf
	extern __exit

global _start
global main

_start:
	call __malloc_init
	call main
	call __malloc_deinit
	ret

main:
	mov rax, msg139635787247704
	push rax
	pop rdi
	call __printf

	mov rax, 1
	push rax

	mov rax, 2
	push rax

	mov rax, 3
	push rax

	mov rax, msg139635787248248
	push rax
	pop rdi
	push qword [rsp + 16]
	pop rsi
	push qword [rsp + 8]
	pop rdx
	push qword [rsp + 0]
	pop rcx
	call __printf

	mov rax, 0
	push rax
	pop rdi
	call __exit


