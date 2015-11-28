.8086
.model large, C

include dos16l_.inc

; int open16l(const char* filename, int mode)
public open16l

.CODE
open16l PROC USES bx ds filename:DWORD, mode:WORD
        IS_NULL_PTR filename
        mov     ax, -9
        je      short exit

        mov     ax, [mode]
        call    parse_flags
        cmp     ax, 0
        jge     short flags_ok
        mov     ax, -57h
        jmp     short exit

flags_ok:
        mov     bx, ax

        call    check16l
        test    ax, ax
        .IF ZERO?
            lds     dx, filename
            call    make_short
        .ELSE
            push    si
            lds     si, filename
            call    make_long
            pop     si
        .ENDIF

        jnc     short exit

        neg     ax

exit:
        ret
open16l ENDP

; Expects mode in ax
; Return < 0 on error, else mode in ax
parse_flags PROC NEAR
        mov     cx, ax
        neg     cx
        and     cx, ax
        .IF     (AX != CX) || (AX > 4)
            mov     ax, -1
        .ENDIF
        ret
parse_flags ENDP

; Receive filename in DS:DX, mode in BX
; Returns regs/flags of short api call
make_short PROC NEAR
        mov     ah, 3dh
        .IF bx == O_RDONLY_16L
            mov     al, 00100000b
        .ELSEIF bx == O_WRONLY_16L
            mov     al, 00010001b
        .ELSE
            mov     al, 00010010b ;set r/w flags, ignored if creating
            .IF bx == O_CREAT_TRUNC_16L
                mov     cx, NONE_16L
                dec     ah
            .ENDIF
        .ENDIF
        int     21h
        ret
make_short ENDP

; Receive filename in DS:SI, mode in BX
; Returns regs/flags of long api call
;
; Flags in BX
; 2-0    file access mode.
; 000 read-only.
; 001 write-only.
; 010 read-write.
; 100 read-only, do not modify file's last-access time
; 6-4    file sharing modes
; 7      no-inherit flag
; 8      do not buffer data (requires that all reads/writes be exact physical
; sectors)
; 9      do not compress file even if volume normally compresses files
; 10     use alias hint in DI as numeric tail for short-name alias
; 12-11  unused??? (0)
; 13     return error code instead of generating INT 24h if critical error
; while opening file
; 14     commit file after every write operation
;
; 6-4 sharing mode (DOS 3.0+)
; 000 compatibility mode.
; 001 "DENYALL" prohibit both read and write access by others.
; 010 "DENYWRITE" prohibit write access by others.
; 011 "DENYREAD" prohibit read access by others.
; 100 "DENYNONE" allow full access by others.

OPEN_MODE_RO            EQU 4
OPEN_MODE_WO            EQU 1
OPEN_MODE_RW            EQU 2

; OPEN_SHARE_DENY_ALL     EQU 16
; OPEN_SHARE_DENY_WRITE   EQU 32
; OPEN_SHARE_DENY_READ    EQU 48
OPEN_SHARE_MODE         EQU 64

OPEN_NO_INT_24          EQU 8192

make_long PROC NEAR
        mov     dx, 1
        .IF bx == O_RDONLY_16L
            mov     bx, OPEN_MODE_RO + OPEN_SHARE_MODE
        .ELSEIF bx == O_WRONLY_16L
            mov     bx, OPEN_MODE_WO + OPEN_SHARE_MODE
        .ELSE
            .IF bx == O_CREAT_TRUNC_16L
                mov     cx, NONE_16L
                mov     dx, 12h
            .ENDIF
            mov     bx, OPEN_MODE_RW + OPEN_SHARE_MODE
        .ENDIF
        or      bx, OPEN_NO_INT_24
        mov     ax, 716Ch
        stc
        int     21h
        ret
make_long ENDP

END
