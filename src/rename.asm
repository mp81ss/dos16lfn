.8086
.model large, C

include dos16l_.inc

; int rename16l(const char* old_file, const char* new_file);
public rename16l

.CODE
rename16l PROC USES di ds es old_file:DWORD, new_file:DWORD
        IS_NULL_PTR old_file
        mov     ax, 9
        je      short f_end
        IS_NULL_PTR new_file
        mov     ax, 9
        je      short f_end

        call    check16l
        lds     dx, [old_file]
        les     di, [new_file]
        test    ax, ax
        jne     short go_lfn
        DOSB    56h
        jmp     short go_end

go_lfn:
        stc
        DOSW    7156h

go_end:
        jc      short f_end
        xor     ax, ax

f_end:
        ret
rename16l ENDP

END
