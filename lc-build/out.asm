section .data
	msg139857446449408 db "x = %d", 10, 0
	msg139857446449904 db "x = %d", 10, 0

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

	mov rax, 10
	mov qword [rbp - 8], qword rax

	mov rdi, msg139857446449408
	mov rax, qword [rbp - 8]
	mov rsi, rax
	call __printf

	mov rbx, 1
	mov rax, qword [rbp - 8]
	mov rax, rax
	add rax, rbx
	mov rax, rax
	mov qword [rbp - 8], rax

	mov rdi, msg139857446449904
	mov rax, qword [rbp - 8]
	mov rsi, rax
	call __printf

	pop rbp
	add rsp, 16
	ret


