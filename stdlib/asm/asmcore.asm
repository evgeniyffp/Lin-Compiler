section .data
    __newline db 10, 0

section .text
    global __exit
    global __brk
    global __strlen
    global puts
    global putchar

__exit:
    mov rax, 60
    syscall
    ret


__brk:
    mov rax, 0x0c
    syscall
    ret


__strlen:
    xor rax, rax
.loop:
    cmp byte [rdi+rax], 0
    je .end
    inc rax
    jmp .loop
.end:
    ret

puts:
    call __strlen
    mov rdx, rax
    mov rax, 1
    mov rsi, rdi
    mov rdi, 1
    syscall
    mov rsi, rsi
    ret

putchar:
    push rdi
    mov rsi, rsp - 1
    mov rdx, 1
    mov rax, 1
    mov rdi, 1
    syscall
    pop rdi
    ret
