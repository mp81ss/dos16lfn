.8086
.model large, C

include dos16l_.inc

; int get_long16l(char* long_name, unsigned int len, const char* short_name);
public get_long16l

.CODE
get_long16l PROC p_long:DWORD, len:WORD, p_short:DWORD
        IS_NULL_PTR p_long
        mov     ax, 9
        je      short f_end
        IS_NULL_PTR p_short
        mov     ax, 9
        je      short f_end

        push    si
        push    di
        push    ds
        push    es

        mov     cx, 8002h
        lds     si, [p_short]
        .IF [len] >= LONG_BUFFER_SIZE
            les     di, [p_long]
        .ELSE
            mov     ax, SEG main_buffer
            mov     es, ax
            mov     di, OFFSET main_buffer
        .ENDIF
        stc
        DOSW    7160h
        .IF !CARRY?
            xor     ax, ax
            .IF [len] < LONG_BUFFER_SIZE
                mov     ax, SEG main_buffer
                mov     ds, ax
                mov     si, OFFSET main_buffer
                STR_LEN
                .IF cx > [len]
                    mov     ax, 18h
                .ELSE
                    TRANSFER_TO p_long
                    xor     ax, ax
                .ENDIF
            .ENDIF
        .ENDIF

        pop     es
        pop     ds
        pop     di
        pop     si

f_end:
        ret
get_long16l ENDP

END
