.8086
.model large, C

include dos16l_.inc

; int unlink16l(const char* filename);
public unlink16l

.CODE
unlink16l PROC USES ds filename:DWORD
        IS_NULL_PTR filename
        mov     ax, 9
        je      short f_end

        call    check16l
        lds     dx, [filename]
        test    ax, ax
        jnz     short go_lfn
        DOSB    41h
        jmp     short go_end

go_lfn:
        push    si
        xor     si, si
        stc
        DOSW    7141h
        pop     si

go_end:
        jc      short f_end
        xor     ax, ax

f_end:
        ret
unlink16l ENDP

END
