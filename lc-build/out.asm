section .data
	msg140245163765848 db "Hello, World!%d", 10, 0

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
	mov rax, msg140245163765848
	push rax

	push qword [rsp + 0]
	pop rdi
	mov rsi, 100
	call __printf

	mov rax, 0
	push rax
	pop rdi
	call __exit


