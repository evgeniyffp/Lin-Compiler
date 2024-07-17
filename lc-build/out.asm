section .data
	msg140113012179200 db "Hello, World!", 10, 0
	msg140113012179408 db "%d", 10, 0

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
	push rbp
	mov rbp, rsp
	sub rsp, 16

	mov rax, 1
	mov qword [rbp - 8], qword rax

	mov rdi, msg140113012179200
	call __printf


	mov rdi, msg140113012179408
	mov rax, qword [rbp - 8]
	mov rsi, rax
	call __printf

	pop rbp
	add rsp, 16
	ret


