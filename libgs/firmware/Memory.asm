.section text

; copy specified number of bytes from source pointer to destination pointer
; if the source and destination overlap, the behavior is undefined
; inputs:
; r0: pointer to source
; r1: pointer to destination
; r2: number of bytes to copy
; outputs:
; none
CopyMemoryChar:
    push r0
    push r1
    push r31

    mov r31, r2
    cmp r31, 0
    ifz jmp CopyMemoryChar_end
CopyMemoryChar_loop:
    mov.8 [r1], [r0]
    inc r0
    inc r1
    loop CopyMemoryChar_loop
CopyMemoryChar_end:
    pop r31
    pop r1
    pop r0
    ret

; copy specified number of words from source pointer to destination pointer
; if the source and destination overlap, the behavior is undefined
; inputs:
; r0: pointer to source
; r1: pointer to destination
; r2: number of words to copy
; outputs:
; none
CopyMemoryInt:
    push r0
    push r1
    push r31

    mov r31, r2
    cmp r31, 0
    ifz jmp CopyMemoryInt_end
CopyMemoryInt_loop:
    mov [r1], [r0]
    add r0, 4
    add r1, 4
    loop CopyMemoryInt_loop
CopyMemoryInt_end:
    pop r31
    pop r1
    pop r0
    ret

; compare specified number of bytes from source pointer with destination pointer
; inputs:
; r0: pointer to source
; r1: pointer to destination
; r2: number of bytes to compare
; outputs:
; r0: 1 if equal, 0 if not equal
CompareMemoryChar:
    push r1
    push r31

    mov r31, r2
CompareMemoryChar_loop:
    cmp.8 [r1], [r0]
    ifnz jmp CompareMemoryChar_not_equal
    inc r0
    inc r1
    loop CompareMemoryChar_loop
    ; set Z flag if we reach this point
    mov r0, 0
    cmp r0, 0
CompareMemoryChar_not_equal:
    pop r31
    pop r1
    ifz mov r0, 1
    ifnz mov r0, 0
    ret

; compare specified number of words from source pointer with destination pointer
; inputs:
; r0: pointer to source
; r1: pointer to destination
; r2: number of words to compare
; outputs:
; r0: 1 if equal, 0 if not equal
CompareMemoryInt:
    push r1
    push r31

    mov r31, r2
CompareMemoryInt_loop:
    cmp [r1], [r0]
    ifnz jmp CompareMemoryInt_not_equal
    add r0, 4
    add r1, 4
    loop CompareMemoryInt_loop
    ; set Z flag if we reach this point
    mov r0, 0
    cmp r0, 0
CompareMemoryInt_not_equal:
    pop r31
    pop r1
    ifz mov r0, 1
    ifnz mov r0, 0
    ret
