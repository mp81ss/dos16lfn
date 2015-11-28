.8086
.model large, C

include dos16l_.inc

; int get_cwd16l(int drive, char* buffer);
; if drive is 0, default drive is assumed, else 1 = A:, 2 = B:, etc.
public get_cwd16l

.CODE
get_cwd16l PROC USES si ds drive:WORD, buffer:DWORD, len:WORD
    LOCAL was_enough:WORD

        IS_NULL_PTR buffer
        mov     ax, 9
        jne     short @f
        jmp     f_end

@@:

        mov     dx, GETCWD_SHORT_SIZE
        .IF ax != 0
            mov     dx, GETCWD_LONG_SIZE
        .ENDIF

        .IF [len] < dx
            mov     dx, SEG main_buffer
            mov     ds, dx
            mov     si, OFFSET main_buffer
            mov     [was_enough], 0
        .ELSE
            lds     si, [buffer]
            mov     [was_enough], -1
        .ENDIF

        mov     ax, [drive]
        .IF ax == 0
            DOSB    19h
        .ELSE
            dec     ax
        .ENDIF
        add     al, 'A'

        mov     byte ptr [si], al
        mov     word ptr [si + 1], "\:"
        add     si, 3

        call    check16l
        mov     dx, [drive]
        test    ax, ax
        jne     short go_lfn

        DOSB    47h
        jmp     short go_end

go_lfn:
        stc
        DOSW    7147h

go_end:
        jc      short f_end

        xor     ax, ax
        .IF [was_enough] == 0
            mov     ax, SEG main_buffer
            mov     ds, ax
            mov     si, OFFSET main_buffer
            STR_LEN
            .IF [len] < cx
                mov     ax, 18h
            .ELSE
                push    di
                push    es
                TRANSFER_TO buffer
                pop     es
                pop     di
                xor     ax, ax
            .ENDIF
        .ENDIF

f_end:
        ret
get_cwd16l ENDP

END
