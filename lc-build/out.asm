section .data
	msg140473603707240 db "%d", 10, 0

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

	mov rax, 1
	mov qword [rbp - 0], qword rax

	mov rax, 2
	mov qword [rbp - 0], qword rax

	mov rdi, msg140473603707240
	mov rax, [rsp - 0]
	mov rsi, rax
	call __printf

	pop rbp
	ret


