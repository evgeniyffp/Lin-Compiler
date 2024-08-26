section .data
	msg140176509747312 db "asasas", 10, 0
	msg140176509747600 db "tstes", 10, 0

section .text
	extern __malloc
	extern __exit
	extern __free
	extern __malloc_init
	extern __malloc_deinit
	extern __strnew
	extern __strcat
	extern __strlen
	extern __strdelete
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

	mov rdi, msg140176509747312
	call __strcpy
	mov qword [rbp - 8], qword rax

	mov rax, qword [rbp - 8]
	mov qword [rbp - 16], rax
	mov rdi, qword [rbp - 16]
	call __printf

	mov rdi, msg140176509747600
	call __strcpy
	mov qword [rbp - 8], rax

	mov rax, qword [rbp - 8]
	mov qword [rbp - 16], rax
	mov rdi, qword [rbp - 16]
	call __printf

	mov rax, qword [rbp - 8]
	mov qword [rbp - 16], rax
	mov rdi, qword [rbp - 16]
	call __printf

	mov rdi, qword [rbp - 8]
	call __strdelete

	pop rbp
	add rsp, 16
	ret


