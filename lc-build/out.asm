section .data
	msg140598055558592 db "%d", 10, 0

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

	mov rax, 1
	mov qword [rbp - 16], qword rax

	mov rax, 1
	mov qword [rbp - 24], qword rax

	mov rax, 1
	mov qword [rbp - 32], qword rax

	mov rax, 1
	mov qword [rbp - 40], qword rax

	mov rax, 1
	mov qword [rbp - 48], qword rax

	mov rax, 1
	mov qword [rbp - 56], qword rax

	mov rax, 1
	mov qword [rbp - 64], qword rax

	mov rax, 1
	mov qword [rbp - 72], qword rax

	mov rax, 1
	mov qword [rbp - 80], qword rax

	mov rax, 1
	mov qword [rbp - 88], qword rax

	mov rax, 1
	mov qword [rbp - 96], qword rax

	mov rax, 1
	mov qword [rbp - 104], qword rax

	mov rax, 1
	mov qword [rbp - 112], qword rax

	mov rax, 1
	mov qword [rbp - 120], qword rax

	mov rax, 1
	mov qword [rbp - 128], qword rax

	mov rax, 1
	mov qword [rbp - 136], qword rax

	mov rax, 1
	mov qword [rbp - 144], qword rax

	mov rax, 1
	mov qword [rbp - 152], qword rax

	mov rax, 1
	mov qword [rbp - 160], qword rax

	mov rax, 1
	mov qword [rbp - 168], qword rax

	mov rax, 1
	mov qword [rbp - 176], qword rax

	mov rax, 1
	mov qword [rbp - 184], qword rax

	mov rax, 1
	mov qword [rbp - 192], qword rax

	mov rax, 1
	mov qword [rbp - 200], qword rax

	mov rax, 1
	mov qword [rbp - 208], qword rax

	mov rax, 1
	mov qword [rbp - 216], qword rax

	mov rax, 1
	mov qword [rbp - 224], qword rax

	mov rax, 1
	mov qword [rbp - 232], qword rax

	mov rax, 1
	mov qword [rbp - 240], qword rax

	mov rax, 1
	mov qword [rbp - 248], qword rax

	mov rax, 1
	mov qword [rbp - 256], qword rax

	mov rax, 1
	mov qword [rbp - 264], qword rax

	mov rax, 1
	mov qword [rbp - 272], qword rax

	mov rax, 1
	mov qword [rbp - 280], qword rax

	mov rax, 10
	mov qword [rbp - 288], qword rax

	mov rax, 3
	mov qword [rbp - 296], qword rax

	mov rax, 3
	mov qword [rbp - 304], qword rax

	mov rax, 3
	mov qword [rbp - 312], qword rax

	mov rax, 3
	mov qword [rbp - 320], qword rax

	mov rax, 3
	mov qword [rbp - 328], qword rax

	mov rax, 3
	mov qword [rbp - 336], qword rax

	mov rax, 3
	mov qword [rbp - 344], qword rax

	mov rax, 3
	mov qword [rbp - 352], qword rax

	mov rax, 3
	mov qword [rbp - 360], qword rax

	mov rax, 3
	mov qword [rbp - 368], qword rax

	mov rax, 3
	mov qword [rbp - 376], qword rax

	mov rax, 3
	mov qword [rbp - 384], qword rax

	mov rax, 3
	mov qword [rbp - 392], qword rax

	mov rax, 3
	mov qword [rbp - 400], qword rax

	mov rax, 3
	mov qword [rbp - 408], qword rax

	mov rax, 3
	mov qword [rbp - 416], qword rax

	mov rax, 3
	mov qword [rbp - 424], qword rax

	mov rax, 3
	mov qword [rbp - 432], qword rax

	mov rax, 3
	mov qword [rbp - 440], qword rax

	mov rax, 3
	mov qword [rbp - 448], qword rax

	mov rax, 3
	mov qword [rbp - 456], qword rax

	mov rax, 3
	mov qword [rbp - 464], qword rax

	mov rax, 3
	mov qword [rbp - 472], qword rax

	mov rax, 3
	mov qword [rbp - 480], qword rax

	mov rax, 3
	mov qword [rbp - 488], qword rax

	mov rax, 3
	mov qword [rbp - 496], qword rax

	mov rax, 3
	mov qword [rbp - 504], qword rax

	mov rax, 3
	mov qword [rbp - 512], qword rax

	mov rax, 1
	mov qword [rbp - 520], qword rax

	mov rax, 1
	mov qword [rbp - 528], qword rax

	mov rax, 1
	mov qword [rbp - 536], qword rax

	mov rax, 1
	mov qword [rbp - 544], qword rax

	mov rax, 1
	mov qword [rbp - 552], qword rax

	mov rax, 1
	mov qword [rbp - 560], qword rax

	mov rax, 1
	mov qword [rbp - 568], qword rax

	mov rax, 1
	mov qword [rbp - 576], qword rax

	mov rax, 1
	mov qword [rbp - 584], qword rax

	mov rax, 1
	mov qword [rbp - 592], qword rax

	mov rax, 1
	mov qword [rbp - 600], qword rax

	mov rax, 11212
	mov qword [rbp - 608], qword rax


	mov rdi, msg140598055558592
	mov rax, qword [rbp - 608]
	mov rsi, rax
	call __printf

	pop rbp
	add rsp, 16
	ret


