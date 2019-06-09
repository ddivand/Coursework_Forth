; ( b a -- [ b + a] )
native "+", plus
    pop rax
    add [rsp], rax
    jmp next

; ( b a -- [ b - a ] )
native "-", minus
    pop rax
    sub [rsp], rax
    jmp next

; ( b a -- [ b * a ] )
native "*", mul
    pop rax
    pop rdx
    imul rdx
    push rax
    jmp next

; ( b a -- [ b / a ] )
native "/", div
    pop rcx
    pop rax
    cqo
    idiv rcx
    push rax
    jmp next

; ( b a -- [ b mod a ] )
native "%", mod
    pop rcx
    pop rax
    cqo
    idiv rcx
    push rdx
    jmp next

; ( x -- x' )
; x' = 0 if x != 0
; x' = 1 if x == 0
native "not", not
    pop rax
    test rax, rax
    setz al
    movzx rax, al
    push rax
    jmp next

; ( a b -- c )
; c = 1 if a == b
; c = 0 if a != b
native "=", equals
    pop rax
    pop rdx
    cmp rax, rdx
    sete al
    movzx rax, al
    push rax
    jmp next

; ( a b -- [b < a] )
native "<", lt
    pop rdx
    pop rax
    cmp rax, rdx
    setl al
    movzx rax, al
    push rax
    jmp next

; ( b a -- [ b & a ] )
; bitwise and
native "and", and
    pop rax
    and [rsp], rax
    jmp next

; ( b a -- b | a )
; bitwise or
native "or", or
    pop rax
    or [rsp], rax
    jmp next

; logic and
native "land", land
    pop rax
    pop rdx
    test rax, rax
    jz .no
    push rdx
    jmp next
.no:
    push rax
    jmp next

; logic or
native "lor", lor
    pop rax
    pop rdx
    test rax, rax
    jnz .yes
    push rdx
    jmp next
.yes:
    push rax
    jmp next


native "drop", drop
    add rsp, 8
    jmp next

; ( a b -- b a )
native "swap", swap
    pop rax
    pop rdx
    push rax
    push rdx
    jmp next

; ( a -- a a)
native "dup", dup
    push qword [rsp]
    jmp next

; return from the colon word
native "exit", exit
    rpop pc
    jmp next

native ">r", to_r
    pop rax
    rpush rax
    jmp next

native "r>", from_r
    rpop rax
    push rax
    jmp next

native "r@", r_fetch
    push qword [rstack]
    jmp next

; save PC when the colon word starts
native "docol", docol
    rpush pc
    add w, 8
    mov pc, w
    jmp next

; ( call_num a1 a2 a3 a4 a5 a6 -- new_rax new_rdx )
; execute syscall
native "syscall", syscall
    pop r9
    pop r8
    pop r10
    pop rdx
    pop rsi
    pop rdi
    pop rax
    syscall
    push rax
    push rdx
    jmp next

; ( -- )
; jump to a location
native "branch", branch
    mov pc, [pc]
    jmp next

native "0branch", branch0
    pop rax
    test rax, rax
    jnz .skip
    mov pc, [pc]
    jmp next
    .skip:
    add pc, 8
    jmp next

native "lit", lit
    push qword [pc]
    add pc, 8
    jmp next

native "execute", execute
    pop rax
    mov w, rax
    jmp [rax]

native "@", fetch
    pop rax
    push qword[rax]
    jmp next

native "!", write
    pop rax
    pop rdx
    mov [rax], rdx
    jmp next

native "c!", write_char
    pop rax
    pop rdx
    mov [rax], dl
    jmp next

; ( addr -- char )
; read one byte starting at addr
native "c@", fetch_char
    pop rax
    movzx rax, byte [rax]
    push rax
    jmp next

native "sp", sp
    push rsp
    jmp next

; ( a -- )
; print integer number
native ".", dot
    pop rdi
    call print_int
    jmp next

native ".S", show_stack
    mov rcx, rsp
    .loop:
        cmp rcx, [stack_start] 
        jae next
        mov rdi, [rcx]
        push rcx
        call print_int
        call print_newline
        pop rcx
        add rcx, 8
        jmp .loop


const "forth-dp", dp
