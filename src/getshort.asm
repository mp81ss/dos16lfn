.8086
.model large, C

include dos16l_.inc

; int get_short16l(char* short_name, unsigned int len, const char* long_name);
public get_short16l

.CODE
get_short16l PROC p_short:DWORD, len:WORD, p_long:DWORD
        IS_NULL_PTR p_short
        mov     ax, 9
        je      short f_end
        IS_NULL_PTR p_long
        mov     ax, 9
        je      short f_end

        push    si
        push    di
        push    ds
        push    es

        lds     si, [p_long]
        .IF [len] >= SHORT_BUFFER_SIZE
            les     di, [p_short]
        .ELSE
            mov     ax, SEG main_buffer
            mov     es, ax
            mov     di, OFFSET main_buffer
        .ENDIF
        mov     cx, 8001h
        stc
        DOSW    7160h
        .IF !CARRY?
            xor     ax, ax
            .IF [len] < SHORT_BUFFER_SIZE
                mov     ax, SEG main_buffer
                mov     ds, ax
                mov     si, OFFSET main_buffer
                STR_LEN
                .IF cx > [len]
                    mov     ax, 18h
                .ELSE
                    TRANSFER_TO p_short
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
get_short16l ENDP

END
