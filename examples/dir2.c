#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

#include <dos16lfn.h>


static const char* base_name( const char* path )
{
    const char* ret = path;
    int i = 0;
    char c;

    if( path != NULL ) {
        do {
            c = path[i++];
            if( c == '\\' || c == ':' || c == '/' )
                ret = path + i;
        }
        while ( c != '\0' );
    }

    return ret;
}

int main( int argc, const char** argv )
{
    char full_path[160], long_buf[261];
    char dir_buf[160], base_dir[132];
    int n, sup, maj_ver, min_ver;
    unsigned long files = 0, dirs = 0, space = 0;
    struct find_t ft;

    sup = is_supported16l();

    if ( get_short16l( dir_buf,
                       sizeof( dir_buf ),
                       argc == 2 ? argv[1] : "." ) != 0 )
    {
        puts( "Unable to get source directory" );
        return 1;
    }

    n = strlen( dir_buf );
    if( dir_buf[n - 1] != '\\' && dir_buf[n - 1] != '/')
        strcat( dir_buf, "\\" );
    strcpy( base_dir, dir_buf );
    strcat( dir_buf, "*.*" );

    if( _dos_findfirst( dir_buf,
                        _A_NORMAL | _A_ARCH | _A_RDONLY | _A_SUBDIR,
                        &ft )
        != 0 )
    {
        puts( "Unable to get files list" );
        return 1;
    }

    puts( "<NAME>       <TYPE/SIZE> <LONG_NAME>\n" );

    do {
        const int is_dot = ( strcmp( ft.name, "." ) == 0
                             || strcmp( ft.name, ".." ) == 0 ) ? 1 : 0;

        n = printf( "%s", ft.name );

        while ( n++ < 13 )
            putchar( ' ' );

        if( ( ft.attrib & _A_SUBDIR ) != 0 ) {
            n = printf( "[DIR]" );
            if ( !is_dot )
                dirs++;
        }
        else {
            n = printf( "%lu", ft.size );
            files++;
            space += ft.size;
        }

        while ( n++ < 12 )
            putchar( ' ' );

        strcpy( full_path, base_dir );
        strcat( full_path, ft.name );

        if( get_long16l( long_buf, sizeof( long_buf ), full_path ) == 0 ) {
            if ( !is_dot )
                puts( base_name ( long_buf ) );
            else
                putchar( '\n' );
        }
        else
            puts( "ERROR: Unable to get long file name" );

        n = _dos_findnext( &ft );
    }
    while( n == 0 );

#ifdef __WATCOMC__
    _dos_findclose( &ft );
#endif

    get_version16l( &maj_ver, &min_ver );
    printf( "\n[%lu files, %lu directories, %lu bytes]\n\nDOS16LFN v%d.%d, "
            "LFN were%s used.", files, dirs, space,  maj_ver, min_ver,
                                sup ? "" : " NOT" );

    return 0;
}
