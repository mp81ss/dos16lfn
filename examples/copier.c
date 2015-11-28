#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

#include <dos16lfn.h>

#define BUF_SIZE 1024


int main( int argc, const char** argv )
{
    char buffer[BUF_SIZE];
    int src_handle, dst_handle;
    unsigned int value, red, written;

    puts("SIMPLE FILE COPIER\n");

    if( argc != 3 ) {
        printf( "TYPE COPIER <SRC_FILE> <DST_FILE>" );
        return 1;
    }

    printf( "%s -> %s", argv[1], argv[2] );

    src_handle = open16l( argv[1], O_RDONLY_16L );
    if ( src_handle < 0 ) {
        printf( "\n%s: %s", argv[1], strerror( -src_handle ) );
        return 2;
    }

    dst_handle = open16l( argv[2], O_CREAT_TRUNC_16L );
    if ( dst_handle < 0 ) {
        _dos_close( src_handle );
        printf( "\n%s: %s", argv[2], strerror( -dst_handle ) );
        return 2;
    }

    do {
        value = _dos_read( src_handle, buffer, sizeof( buffer ), &red );
        if( value != 0 )
            printf( ": ERROR reading file" );

        if( value == 0 && red > 0 ) {
            value = _dos_write( dst_handle, buffer, red, &written );
            if( value != 0 || written < red )
                printf( ": ERROR writing file" );
        }
    }
    while( value == 0 && red > 0 );

    _dos_close( src_handle );
    _dos_close( dst_handle );

    return 0;
}
