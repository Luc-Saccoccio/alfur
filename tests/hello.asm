section .data
    hello_world db "Hello world!", 10
    hello_world_len  equ $ - hello_world
section .text
    global _start
    _start:
        mov rax, 1 ; write(2)
        mov rdi, 1 ; stdout
        mov rsi, hello_world ; buf
        mov rdx, hello_world_len ; buf len
        syscall
        mov rax, 60 ; exit(2)
        mov rdi, 0 ; exit code
        syscall
