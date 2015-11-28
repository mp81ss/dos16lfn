; Internal inclusion for dos16l library

INCLUDE ..\include\dos16lfn.inc

SHORT_BUFFER_SIZE   EQU 128
LONG_BUFFER_SIZE    EQU 261
GETCWD_SHORT_SIZE   EQU 64
GETCWD_LONG_SIZE    EQU LONG_BUFFER_SIZE
MAIN_BUFFER_SIZE    EQU GETCWD_LONG_SIZE

externdef main_buffer:BYTE
externdef C check16l:PROC

DOSB MACRO bval
          mov ah, bval
          int 21h
ENDM

DOSW MACRO wval
          mov ax, wval
          int 21h
ENDM

IS_NULL_PTR MACRO p_far
    mov     ax, word ptr [p_far]
    or      ax, word ptr [p_far + 2]
ENDM

TRANSFER_TO MACRO buffer
local main_loop
        mov     ax, SEG main_buffer
        mov     ds, ax
        mov     si, OFFSET main_buffer
        les     di, buffer
        xor     ax, ax
        cld
main_loop:
        lodsb
        stosb
        test    ax, ax
        jne     short main_loop
ENDM

STR_LEN MACRO
local main_loop
        cld
        xor     ax, ax
        mov     cx, si
main_loop:
        lodsb
        test    ax, ax
        jne     short main_loop
        sub     cx, si
        neg     cx
ENDM
