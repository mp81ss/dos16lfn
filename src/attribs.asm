.8086
.model large, C

include dos16l_.inc

; int get_attrib16l(const char* filename, int* p_attribs);
public get_attrib16l
; int set_attrib16l(const char* filename, int attribs);
public set_attrib16l

.CODE
get_attrib16l PROC USES bx ds filename:DWORD, p_attribs:DWORD
        IS_NULL_PTR filename
        mov     ax, 9
        je      short exit
        IS_NULL_PTR p_attribs
        mov     ax, 9
        je      short exit

        call    check16l
        lds     dx, [filename]
        test    ax, ax
        jne     short go_lfn
        DOSW    4300h
        jmp     short go_end

go_lfn:
        xor     bx, bx
        stc
        DOSW    7143h

go_end:
        jc      short exit
        lds     bx, [p_attribs]
        mov     word ptr [bx], cx
        xor     ax, ax

exit:
        ret
get_attrib16l ENDP

set_attrib16l PROC USES bx ds filename:DWORD, attribs:WORD
        IS_NULL_PTR filename
        mov     ax, 9
        je      short f_end

        call    check16l
        lds     dx, [filename]
        mov     cx, [attribs]
        test    ax, ax
        jne     short go_lfn
        DOSW    4301h
        jmp     short go_end

go_lfn:
        mov     bl, 1
        stc
        DOSW    7143h

go_end:
        jc      short f_end
        xor     ax, ax

f_end:
        ret
set_attrib16l ENDP

END
