.8086
.MODEL LARGE
.STACK 100h

include dos16lfn.inc

.CONST
short_msg db "Short current directory is: $"
long_msg  db "Long  current directory is: $"

.DATA?
buffer db 261 dup(?)
BUFFER_SIZE EQU $ - OFFSET buffer

.CODE
.STARTUP
            mov     ax, @data
            mov     ds, ax

            mov     al, 0Ah
            int     29h
            mov     dx, OFFSET short_msg
            mov     ah, 9
            int     21h

            xor     ax, ax
            push    ax
            call    set_status16l
            add     sp, 2

            mov     cx, BUFFER_SIZE
            mov     dx, OFFSET buffer
            xor     ax, ax
            push    cx
            push    ds
            push    dx
            push    ax
            call    get_cwd16l
            add     sp, 8

            mov     si, OFFSET buffer
            call    PrintZ
            mov     al, 0Ah
            int     29h
            mov     al, 0Dh
            int     29h

            mov     ax, 1
            push    ax
            call    set_status16l
            add     sp, 2

            mov     cx, BUFFER_SIZE
            mov     dx, OFFSET buffer
            xor     ax, ax
            push    cx
            push    ds
            push    dx
            push    ax
            call    get_cwd16l
            add     sp, 8

            mov     dx, OFFSET long_msg
            mov     ah, 9
            int     21h
            mov     si, OFFSET buffer
            call    PrintZ

            mov ax, 4c00h
            int 21h

PrintZ PROC NEAR
           cld
PZag:      lodsb
           test al, al
           jz endpz
           int 29h
           jmp PZag
endpz:     ret
PrintZ ENDP

END
