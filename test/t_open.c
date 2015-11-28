#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dos.h> /* read/write/close */
#include <io.h>  /* lseek */

#include <dos16lfn.h>

#define YCT_DISABLE_INT64
#include <yacut.h>

#ifdef __BORLANDC__
#define LFN_SEEK lseek
#else
#define LFN_SEEK _lseek
#endif


static const char* long_filename = "A Very Long Filename.Long Extension";
static const char* short_name = "file.ext";
static char buf[256];


YCT_TEST( opens )
{
    const char* file_names[2];
    const char* pFname;
    int i, n, handle;
    size_t buflen;
    unsigned int processed;

    file_names[0] = short_name;
    file_names[1] = long_filename;
    buflen = sizeof( buf );

    for( i = 0, n = is_supported16l() == 0 ? 1 : 2; i < n; ++i )
    {
        YCT_WARNING_MSG( n == 2, "Long mode not supported") ;

        set_status16l( i );
        pFname = file_names[i];

        handle = open16l( pFname, -1 );
        YCT_ASSERT_EQUAL( -87, handle );
        handle = open16l( pFname, 3 );
        YCT_ASSERT_EQUAL( -87, handle );
        handle = open16l( pFname, 5 );
        YCT_ASSERT_EQUAL( -87, handle );
        handle = open16l( pFname, 6 );
        YCT_ASSERT_EQUAL( -87, handle );
        handle = open16l( pFname, 7 );
        YCT_ASSERT_EQUAL( -87, handle );
        handle = open16l( pFname, 8 );
        YCT_ASSERT_EQUAL( -87, handle );

        handle = open16l( pFname, O_CREAT_TRUNC_16L );
        YCT_ASSERT_MSG( handle >= 0, "Cannot create filename" );
        YCT_ASSERT_EQUAL( 0, _dos_close( handle ) );
        handle = open16l( pFname, O_CREAT_TRUNC_16L );
        YCT_ASSERT_MSG( handle >= 0,
                        "Cannot create/trunc existent filename" );
        YCT_ASSERT_EQUAL( 0, _dos_write( handle, buf, buflen, &processed ) );
        YCT_ASSERT_EQUAL( buflen, processed );
        YCT_ASSERT_EQUAL( 0, LFN_SEEK( handle, 0, SEEK_SET ) );
        YCT_ASSERT_EQUAL( 0, _dos_read( handle, buf, buflen, &processed ) );
        YCT_ASSERT_EQUAL( buflen, processed );
        YCT_ASSERT_EQUAL( 0, _dos_close( handle ) );

        handle = open16l( "nonEXIST.TmP", O_RDONLY_16L );
        YCT_ASSERT_MSG( handle < 0, "read-only mode created file" );
        handle = open16l( pFname, O_RDONLY_16L );
        YCT_ASSERT_MSG( handle >= 0, "Cannot open filename read-only" );
        YCT_ASSERT_EQUAL( 0, _dos_read( handle, buf, buflen, &processed ) );
        YCT_ASSERT_EQUAL( buflen, processed );
        YCT_ASSERT_MSG( _dos_write( handle, buf, buflen, &processed ) != 0,
                        "Write to read-only file" );
        YCT_ASSERT_EQUAL( 0, LFN_SEEK( handle, 0, SEEK_SET ) );
        YCT_ASSERT_EQUAL( 0, _dos_close( handle ) );

        handle = open16l( "nonEXIST.TmP", O_WRONLY_16L );
        YCT_ASSERT_MSG( handle < 0, "write-only mode created file" );
        handle = open16l( pFname, O_WRONLY_16L );
        YCT_ASSERT_MSG( handle >= 0, "Cannot open filename write-only" );
        YCT_ASSERT_EQUAL( 0, _dos_write( handle, buf, buflen, &processed ) );
        YCT_ASSERT_EQUAL( buflen, processed );
        YCT_ASSERT_EQUAL( 0, LFN_SEEK( handle, 0, SEEK_SET ) );
        YCT_ASSERT_MSG( _dos_read( handle, buf, buflen, &processed ) != 0,
                        "Read to write-only file" );
        YCT_ASSERT_EQUAL( 0, _dos_close( handle ) );

        handle = open16l( "nonEXIST.TmP", O_RDWR_16L );
        YCT_ASSERT_MSG( handle < 0, "read-write mode created file" );
        handle = open16l( pFname, O_RDWR_16L );
        YCT_ASSERT_MSG( handle >= 0, "Cannot open filename read-write" );
        YCT_ASSERT_EQUAL( 0, _dos_write( handle, buf, buflen, &processed ) );
        YCT_ASSERT_EQUAL( buflen, processed );
        YCT_ASSERT_EQUAL( 0, LFN_SEEK( handle, 0, SEEK_SET ) );
        YCT_ASSERT_EQUAL( 0, _dos_read( handle, buf, buflen, &processed ) );
        YCT_ASSERT_EQUAL( buflen, processed );
        YCT_ASSERT_EQUAL( 0, _dos_close( handle ) );

        YCT_ASSERT_EQUAL( 0, unlink16l( pFname ) );

        if( i > 0 )
        {
            char b[] = "This is a long name.exts1";
            const size_t len = strlen( b );

            YCT_ASSERT( is_supported16l() != 0 );
            YCT_ASSERT( get_status16l() != 0 );

            handle = open16l( b, O_CREAT_TRUNC_16L );
            YCT_ASSERT_MSG( handle >= 0, "Cannot create filename" );
            YCT_ASSERT( get_last_status16l() != 0 );
            YCT_ASSERT_EQUAL( 0, _dos_close( handle ) );
            b[len - 1]++;
            handle = open16l( b, O_CREAT_TRUNC_16L );
            YCT_ASSERT_MSG( handle >= 0, "Cannot create filename" );
            YCT_ASSERT( get_last_status16l() != 0 );
            YCT_ASSERT_EQUAL( 0, _dos_close( handle ) );
            b[len - 1]++;
            handle = open16l( b, O_CREAT_TRUNC_16L );
            YCT_ASSERT_MSG( handle >= 0, "Cannot create filename" );
            YCT_ASSERT( get_last_status16l() != 0 );
            YCT_ASSERT_EQUAL( 0, _dos_close( handle ) );
            b[len - 1]++;
            handle = open16l( b, O_CREAT_TRUNC_16L );
            YCT_ASSERT_MSG( handle >= 0, "Cannot create filename" );
            YCT_ASSERT( get_last_status16l() != 0 );
            YCT_ASSERT_EQUAL( 0, _dos_close( handle ) );

            YCT_ASSERT_EQUAL( 0, unlink16l( b ) );
            b[len - 1]--;
            YCT_ASSERT_EQUAL( 0, unlink16l( b ) );
            b[len - 1]--;
            YCT_ASSERT_EQUAL( 0, unlink16l( b ) );
            b[len - 1]--;
            YCT_ASSERT_EQUAL( 0, unlink16l( b ) );

            handle = open16l( b, O_CREAT_TRUNC_16L );
            YCT_ASSERT_MSG( handle >= 0, "Cannot create filename" );
            YCT_ASSERT( get_last_status16l() != 0 );
            YCT_ASSERT_EQUAL( 0, _dos_close( handle ) );
            b[len - 7]++;
            handle = open16l( b, O_CREAT_TRUNC_16L );
            YCT_ASSERT_MSG( handle >= 0, "Cannot create filename" );
            YCT_ASSERT( get_last_status16l() != 0 );
            YCT_ASSERT_EQUAL( 0, _dos_close( handle ) );
            b[len - 7]++;
            handle = open16l( b, O_CREAT_TRUNC_16L );
            YCT_ASSERT_MSG( handle >= 0, "Cannot create filename" );
            YCT_ASSERT( get_last_status16l() != 0 );
            YCT_ASSERT_EQUAL( 0, _dos_close( handle ) );
            b[len - 7]++;
            handle = open16l( b, O_CREAT_TRUNC_16L );
            YCT_ASSERT_MSG( handle >= 0, "Cannot create filename" );
            YCT_ASSERT( get_last_status16l() != 0 );
            YCT_ASSERT_EQUAL( 0, _dos_close( handle ) );

            YCT_ASSERT_EQUAL( 0, unlink16l( b ) );
            b[len - 7]--;
            YCT_ASSERT_EQUAL( 0, unlink16l( b ) );
            b[len - 7]--;
            YCT_ASSERT_EQUAL( 0, unlink16l( b ) );
            b[len - 7]--;
            YCT_ASSERT_EQUAL( 0, unlink16l( b ) );
        }
    }
}

static void tear_down(void)
{
    unlink16l( short_name );
    unlink16l( long_filename );
}

YCT_SUITE( suite, NULL, tear_down )
{
    YCT_TEST_ADD( opens );
}
YCT_SUITE_END

int main( int argc, const char** argv )
{
    YCT_BEGIN("DOS16LFN Open Test");
    YCT_SET_FULL_BLOCKED();
    YCT_SUITE_RUN( suite );
    YCT_DUMP();
    YCT_END();

    return 0;
}
