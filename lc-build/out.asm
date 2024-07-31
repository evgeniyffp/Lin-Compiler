section .data
	msg140407214702704 db "Hello, World!", 10, 0

section .text
	extern __malloc
	extern __exit
	extern __free
	extern __malloc_init
	extern __malloc_deinit
	extern __strcat
	extern __strlen
	extern __strcpy
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

	mov rdi, msg140407214702704
	call __strcpy
	mov qword [rbp - 8], qword rax

	mov rdi, qword [rbp - 8]
	call __printf

	pop rbp
	add rsp, 16
	ret


