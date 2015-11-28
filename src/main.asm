.8086
.model large, C

include dos16l_.inc


; Internal function, called by library functions only.
public check16l

; void get_version16l( int* pMajor, int* pMinor );
public C get_version16l

; int is_supported16l(void);
; Return non-zero if LFN are supported, ZERO otherwise.
public C is_supported16l

; int get_status16l(void);
; Return 0 for short-filename calls, non-zero otherwise.
public C get_status16l

; void set_status16l(int status);
; Status is 0 for short-filename calls, non-zero otherwise.
public C set_status16l

; int get_last_status16l(void);
; Return an int < 0 if never called, 0 for short call, > 0 for long call.
public C get_last_status16l

; used by library functions only.
public main_buffer

.FARDATA?
main_buffer db MAIN_BUFFER_SIZE dup (?)

.DATA
is_enabled      dw 1 ; 0 if disabled, non-zero if enabled, enabled by default
last_status     dw 0 ; 0 = not called, 1 = short, 2 = long
partial_support dw 0 ; 0 = not checked, 1 = not supported, 2 = supported

.CODE
get_version16l PROC USES ds bx pMajor:DWORD, pMinor:DWORD
        IS_NULL_PTR pMajor
        .IF !ZERO?
            lds     bx, pMajor
            mov     word ptr [bx], DOS16LFN_MAJOR
        .ENDIF

        IS_NULL_PTR pMinor
        .IF !ZERO?
            lds     bx, pMinor
            mov     word ptr [bx], DOS16LFN_MINOR
        .ENDIF

        xor     ax, ax

        ret
get_version16l ENDP

is_supported16l PROC USES ds
        mov     ax, @data
        mov     ds, ax

        .IF     word ptr [partial_support] == 0

            inc     word ptr [partial_support]

            DOSB    30h
            cmp     al, 4
            jae     short test_win_ver

short_exit:
            xor     ax, ax
            jmp     short exit

test_win_ver: ; Checks by DOSLFN (Haftmann)
            mov     ax, 160Ah
            int     2Fh
            test    ax, ax
            jne     short part_ok
            cmp     bh, 4
            jae     short short_exit

        .ELSEIF     word ptr [partial_support] == 1
            jmp     short short_exit
        .ELSE
            jmp     short do_check
        .ENDIF

part_ok:
            inc     word ptr [partial_support]

do_check:

        push    bx
        mov     ax, @data
        mov     ds, ax
        mov     bx, -1
        mov     dx, OFFSET main_buffer
        stc
        DOSW    71A6h
        pop     bx
        jnc     short is_supported
        cmp     ax, 7100h
        jz      short short_exit

is_supported:
        mov     ax, 1

exit:
        ret
is_supported16l ENDP

check16l PROC USES ds ; return 0 for short, non-zero for long.
        mov     dx, @data
        mov     ds, dx
        cmp     [is_enabled], 0
        mov     ax, 0
        jz      @f

        call    is_supported16l

@@:
        mov     [last_status], ax
        ret
check16l ENDP

get_status16l PROC USES ds
        mov     ax, @data
        mov     ds, ax
        mov     ax, [is_enabled]
        ret
get_status16l ENDP

set_status16l PROC USES ds status:WORD
        mov     ax, @data
        mov     ds, ax
        mov     ax, [status]
        mov     [is_enabled], ax
        xor     ax, ax
        ret
set_status16l ENDP

get_last_status16l PROC USES ds
        mov     ax, @data
        mov     ds, ax
        mov     ax, [last_status]
        ret
get_last_status16l ENDP

END
