section .data
	msg140202742251872 db "12", 0

section .text
	extern strlen
	extern __printf
	extern __exit
	extern __malloc_init
	extern __free
	extern __malloc_deinit
	extern __malloc

global _start
global main

_start:
	call __malloc_init
	call main
	call __malloc_deinit
	ret

main:
	mov rax, 0
	push rax
	mov rax, 1
	push rax
	pop rax
	pop rbx
	cmp rax, rbx
	sete cl
	movzx rax, cl
	push rax

	mov rax, msg140202742251872
	push rax
	pop rdi
	call strlen

	mov rax, 0
	push rax
	pop rdi
	call __exit


