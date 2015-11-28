/* DOS16LFN header */

#ifndef DOS16LFN_H_
#define DOS16LFN_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DOS16LFN

#define DOS16LFN_MAJOR      1
#define DOS16LFN_MINOR      0

#define O_RDONLY_16L        0
#define O_WRONLY_16L        1
#define O_RDWR_16L          2
#define O_CREAT_TRUNC_16L   4

#define NONE_16L            0
#define READ_ONLY_16L       1
#define HIDDEN_16L          2
#define SYSTEM_16L          4
#define FOLDER_16L          16
#define ARCHIVE_16L         32

#define IS_DIR16L(a)            ((a) & FOLDER_16L)

void __cdecl get_version16l( int* pMajor, int* pMinor );
int  __cdecl  is_supported16l( void );
int  __cdecl get_status16l( void );
void __cdecl set_status16l( int status );
int  __cdecl get_last_status16l( void );

int __cdecl get_attrib16l( const char* filename, int* p_attribs );
int __cdecl set_attrib16l( const char* filename, int attribs );
int __cdecl open16l( const char* filename, int mode );
int __cdecl rename16l( const char* old_name, const char* new_name );
int __cdecl unlink16l( const char* file_name );

int __cdecl get_cwd16l( int drive, char* buffer, unsigned int buffer_len );
int __cdecl mkdir16l( const char* dir_name );
int __cdecl chdir16l( const char* new_dir );
int __cdecl rmdir16l( const char* dir_name );

int __cdecl get_short16l( char* short_buf,
                          unsigned int buf_len,
                          const char* long_name );

int __cdecl get_long16l( char* long_buf,
                         unsigned int buf_len,
                         const char* short_name );

#ifdef __cplusplus
}
#endif

#endif /* DOS16LFN_H_ */
