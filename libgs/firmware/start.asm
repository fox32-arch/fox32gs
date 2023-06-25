.section text
    mov rsp, 0x00400000
    call Main

    ; Main() should never return, but just in case it does, hang
    rjmp 0
