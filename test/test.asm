; functional checks are done by c tests. This file checks ASM protos only

.8086
.model large, C
.STACK 100h

include dos16lfn.inc

.CONST
err_msg db "Version error!$"

.DATA?
maj dw ?
min dw ?

.CODE
.STARTUP

    invoke      get_version16l, ADDR maj, ADDR min
    .IF         (([maj] != 1) || ([min] != 0))
        mov ax, @data
        mov ds, ax
        mov dx, OFFSET err_msg
        mov ah, 9
        int 21h
        mov ax, 4c01h
        int 21h
    .ENDIF

    invoke      is_supported16l

    invoke      get_status16l
    invoke      set_status16l, 1
    invoke      get_last_status16l

    invoke      get_attrib16l, ADDR maj, ADDR min
    invoke      set_attrib16l, ADDR maj, min
    invoke      open16l, ADDR maj, min
    invoke      rename16l, ADDR maj, ADDR min
    invoke      unlink16l, ADDR maj

    invoke      get_cwd16l, min, ADDR maj, min
    invoke      mkdir16l, ADDR maj
    invoke      chdir16l, ADDR maj
    invoke      rmdir16l, ADDR maj

    invoke      get_short16l, ADDR maj, min, ADDR min
    invoke      get_long16l, ADDR maj, min, ADDR min

    mov         ax, 4c00h
    int         21h
END
