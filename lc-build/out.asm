section .data
	msg140737331376568 db "1", 10, 0
	msg140737331377072 db "2", 10, 0
	msg140737331377576 db "3", 10, 0
	msg140737331377744 db "4", 10, 0

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

	mov rbx, 11
	mov rax, 10
	cmp rax, rbx
	sete cl
	movzx rax, cl
	mov rax, rax
	cmp rax, 1
	jne .else140737331376160
	mov rdi, msg140737331376568
	call __printf

	jmp .end140737331376160
.else140737331376160:
	mov rbx, 101
	mov rax, 10
	cmp rax, rbx
	sete cl
	movzx rax, cl
	mov rax, rax
	cmp rax, 1
	jne .else140737331376664
	mov rdi, msg140737331377072
	call __printf

	jmp .end140737331376664
.else140737331376664:
	mov rbx, 120
	mov rax, 10
	cmp rax, rbx
	sete cl
	movzx rax, cl
	mov rax, rax
	cmp rax, 1
	jne .else140737331377168
	mov rdi, msg140737331377576
	call __printf

	jmp .end140737331377168
.else140737331377168:
	mov rdi, msg140737331377744
	call __printf
.end140737331377168:
.end140737331376664:
.end140737331376160:

	pop rbp
	add rsp, 16
	ret


