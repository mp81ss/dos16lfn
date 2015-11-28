.8086
.model large, C

include dos16l_.inc

; int chdir16l(const char* new_dir);
public chdir16l

.CODE
chdir16l PROC USES ds dirname:DWORD
        IS_NULL_PTR dirname
        mov     ax, 9
        je      short f_end

        call    check16l
        lds     dx, [dirname]
        test    ax, ax
        jne     short go_lfn
        DOSB    3Bh
        jmp     short go_end

go_lfn:
        stc
        DOSW    713Bh

go_end:
        jc      short f_end
        xor     ax, ax

f_end:
        ret
chdir16l ENDP

END
