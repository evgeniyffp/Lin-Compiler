section .data
     msg db "Hello, World!", 10, "Testing", 0

section .text
global _start
_start:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, 22
    syscall

    xor rdi, rdi
    mov rax, 60
    syscall
    ret