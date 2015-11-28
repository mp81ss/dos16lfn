.8086
.model large, C

include dos16l_.inc

; int mkdir16l(const char* dirname);
public mkdir16l

.CODE
mkdir16l PROC USES ds dirname:DWORD
        IS_NULL_PTR dirname
        mov     ax, 9
        je      short f_end

        call    check16l
        lds     dx, [dirname]
        test    ax, ax
        jne     short go_lfn
        DOSB    39h
        jmp     short go_end

go_lfn:
        stc
        DOSW    7139h

go_end:
        jc      short f_end
        xor     ax, ax

f_end:
        ret
mkdir16l ENDP

END
