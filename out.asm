global _start
_start:
    mov rax, 17
    push rax
    mov rax, 60
    pop rdi
    syscall
