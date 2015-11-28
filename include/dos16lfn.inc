; DOS16LFN Header, see C header for brief doc or check help folder

; Header guard
IFNDEF DOS16LFN_INC_
DOS16LFN_INC_           EQU     1

; To detect dos16lfn with preprocessor
DOS16LFN                EQU     1

; Version macros
DOS16LFN_MAJOR	        =       1
DOS16LFN_MINOR          =       0

; open flags
O_RDONLY_16L            EQU     0
O_WRONLY_16L            EQU     1
O_RDWR_16L              EQU     2
O_CREAT_TRUNC_16L       EQU     4

; attribute flags
NONE_16L                EQU     0
READ_ONLY_16L           EQU     1
HIDDEN_16L              EQU     2
SYSTEM_16L              EQU     4
FOLDER_16L              EQU     16
ARCHIVE_16L             EQU     32

; API prototypes
get_version16l      PROTO FAR C pMajor:DWORD, pMinor:DWORD
is_supported16l     PROTO FAR C
get_status16l       PROTO FAR C
set_status16l       PROTO FAR C status:WORD
get_last_status16l  PROTO FAR C

get_attrib16l       PROTO FAR C filename:DWORD, p_attrib:DWORD
set_attrib16l       PROTO FAR C filename:DWORD, attrib:WORD

open16l             PROTO FAR C filename:DWORD, mode:WORD
rename16l           PROTO FAR C old_filename:DWORD, new_filename:DWORD
unlink16l           PROTO FAR C filename:DWORD

get_cwd16l          PROTO FAR C drive:WORD, buffer:DWORD, buffer_len:WORD
mkdir16l            PROTO FAR C dir_name:DWORD
chdir16l            PROTO FAR C dir_name:DWORD
rmdir16l            PROTO FAR C dir_name:DWORD

get_short16l        PROTO FAR C short_path:DWORD,
                                short_len:WORD,
                                long_path:DWORD

get_long16l         PROTO FAR C long_path:DWORD,
                                long_len:WORD,
                                short_path:DWORD

ENDIF ; DOS16LFN_INC_
