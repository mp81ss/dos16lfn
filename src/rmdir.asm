.8086
.model large, C

include dos16l_.inc

; int rmdir16l(const char* dir_name);
public rmdir16l

.CODE
rmdir16l PROC USES ds dirname:DWORD
        IS_NULL_PTR dirname
        mov     ax, 9
        je      short f_end

        call    check16l
        lds     dx, [dirname]
        test    ax, ax
        jne     short go_lfn
        DOSB    3Ah
        jmp     short go_end

go_lfn:
        stc
        DOSW    713Ah

go_end:
        jc      short f_end
        xor     ax, ax

f_end:
        ret
rmdir16l ENDP

END
