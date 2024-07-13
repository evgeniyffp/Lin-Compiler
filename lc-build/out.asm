section .data
	msg140668446953824 db "Hello, World!", 10, "", 0

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

	mov rax, msg140668446953824
	push rax
	pop rdi
	call __printf

	mov rax, 0
	push rax
	pop rdi
	call __exit


