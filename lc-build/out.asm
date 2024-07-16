section .data
	msg139703324274776 db "Hello, World!", 10, "", 0
	msg139703324275320 db "x = %d", 10, "y = %d", 10, "z = %d", 10, "", 0

section .text
	extern __malloc
	extern __exit
	extern __free
	extern __malloc_init
	extern __malloc_deinit
	extern __strlen
	extern __printf

global _start
global main

_start:
	call __malloc_init
	call main
	call __malloc_deinit
	mov rdi, 0
	call __exit
	ret

main:
	mov rdi, msg139703324274776
	call __printf

	mov rax, 1
	push rax

	mov rax, 2
	push rax

	mov rax, 3
	push rax

	mov rdi, msg139703324275320
	mov rax, [rsp + 16]
	mov rsi, rax
	mov rax, [rsp + 8]
	mov rdx, rax
	mov rax, [rsp + 0]
	mov rcx, rax
	call __printf

	pop rax
	pop rax
	pop rax

	ret


