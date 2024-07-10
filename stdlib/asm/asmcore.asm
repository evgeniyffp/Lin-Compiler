section .data
    __newline db 10, 0

section .text
    global __exit
    global __brk
    global strlen
    global __printf
    global writeln

__exit:
    mov rax, 60
    syscall


__brk:
    mov rax, 0x0c
    syscall
    ret


strlen:
    xor rax, rax
.loop:
    cmp byte [rdi+rax], 0
    je .end
    inc rax
    jmp .loop
.end:
    ret


__printf:
    call strlen
    mov rdx, rax
    mov rax, 1
    mov rsi, rdi
    mov rdi, 1
    syscall
    mov rsi, rdi
    ret


writeln:
    push rdi
    call __printf
    mov rdi, __newline
    call __printf
    pop rdi
    ret

