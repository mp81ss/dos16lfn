#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dos.h> /* read/write/close */
#include <io.h>  /* lseek */

#include <dos16lfn.h>

#define YCT_DISABLE_INT64
#include <yacut.h>


static const char long_filename[] = "A Very Long Filename.Long Extension";
static const char long_filename2[] = "A Very Long Filename2.Long Extension2";
static const char short_name[] = "file.ext";
static const char short_name2[] = "file2.ex2";
static const char short_dir[] = "dir";
static const char long_dir[] = "This Is A Very Long Directory";
static char buf[256], buf2[512], temp[1024];


YCT_TEST( NULL_Pointers_small_buffers )
{
    char small_buf[2];
    char* pChar = NULL;
    int attribs, i;

    get_version16l( &i, NULL );
    YCT_ASSERT_EQUAL( DOS16LFN_MAJOR, i );
    get_version16l( NULL, &i );
    YCT_ASSERT_EQUAL( DOS16LFN_MINOR, i );
    get_version16l( NULL, NULL );

    for( i = 0; i < 2; ++i )
    {
        set_status16l( i );

        YCT_ASSERT_EQUAL( 9, get_attrib16l(NULL, NULL) );
        YCT_ASSERT_EQUAL( 9, get_attrib16l( NULL, &attribs ) );
        YCT_ASSERT_EQUAL( 9, get_attrib16l( pChar, NULL ) );
        YCT_ASSERT_EQUAL( 9, set_attrib16l( NULL, attribs ) );

        YCT_ASSERT_EQUAL( 9, mkdir16l( NULL ) );
        YCT_ASSERT_EQUAL( 9, chdir16l( NULL ) );
        YCT_ASSERT_EQUAL( 9, rmdir16l( NULL ) );
        YCT_ASSERT_EQUAL( 9, get_cwd16l( -1, NULL, 0 ) );

        YCT_ASSERT_EQUAL( -9, open16l( NULL, i ) );
        YCT_ASSERT_EQUAL( 9, rename16l( NULL, NULL ) );
        YCT_ASSERT_EQUAL( 9, rename16l( NULL, pChar ) );
        YCT_ASSERT_EQUAL( 9, rename16l( pChar, NULL ) );
        YCT_ASSERT_EQUAL( 9, unlink16l( NULL ) );

        YCT_ASSERT_EQUAL( 9, get_short16l( NULL, 0, NULL ) );
        YCT_ASSERT_EQUAL( 9, get_short16l( NULL, 0, pChar ) );
        YCT_ASSERT_EQUAL( 9, get_short16l( pChar, 0, NULL ) );

        YCT_ASSERT_EQUAL( 9, get_long16l( NULL, 0, NULL ) );
        YCT_ASSERT_EQUAL( 9, get_long16l( NULL, 0, pChar ) );
        YCT_ASSERT_EQUAL( 9, get_long16l( pChar, 0, NULL ) );

        YCT_ASSERT_EQUAL( 0x18,
                          get_cwd16l( -1, small_buf, sizeof( small_buf ) ) );

        if( i != 0 && is_supported16l() != 0 )
        {
            YCT_ASSERT_EQUAL( 0, get_cwd16l( -1, buf2, sizeof( buf2 ) ) );

            YCT_ASSERT_EQUAL( 0x18, get_short16l( small_buf,
                                                  sizeof( small_buf ),
                                                  buf2 ) );

            YCT_ASSERT_EQUAL( 0x18, get_long16l( small_buf,
                                                 sizeof( small_buf ),
                                                 buf2 ) );
        }
    }
}

YCT_TEST( dirs_and_status )
{
    static const int n = -1;

    set_status16l( 0 );
    YCT_ASSERT_EQUAL( 0, get_cwd16l( n, buf, 63 ) );
    YCT_ASSERT_EQUAL( 0, get_last_status16l() );
    YCT_ASSERT_EQUAL( 0, mkdir16l( short_dir ) );
    YCT_ASSERT_EQUAL( 0, chdir16l( short_dir ) );
    YCT_ASSERT_EQUAL( 0, get_cwd16l( n, buf2, sizeof( buf2 ) ) );
    YCT_ASSERT_EQUAL( 0, chdir16l( ".." ) );
    YCT_ASSERT_EQUAL( 0, get_cwd16l( n, buf2, sizeof( buf2 ) ) );
    YCT_ASSERT_EQUAL_STR( buf, buf2 );
    YCT_ASSERT_EQUAL( 0, rmdir16l( short_dir ) );

    if( is_supported16l() != 0 )
    {
        set_status16l( 1 );
        YCT_ASSERT_MSG( get_status16l() != 0, "Status was not set" );
        YCT_ASSERT_EQUAL( 0, mkdir16l( long_dir ) );

        YCT_ASSERT_MSG( get_last_status16l() != 0,
                        "Long Status was not used" );

        YCT_ASSERT_EQUAL( 0, get_short16l( buf, sizeof( buf ), long_dir ) );
        YCT_ASSERT_EQUAL( 0, get_long16l( buf2, sizeof( buf2 ), buf ) );
        YCT_ASSERT_EQUAL( 0, get_cwd16l( n, temp, sizeof( temp ) ) );
        YCT_ASSERT_EQUAL( 0, chdir16l( buf ) );
        YCT_ASSERT_EQUAL( 0, get_cwd16l( n, temp, sizeof( temp ) ) );
        YCT_ASSERT_EQUAL( 0, chdir16l( ".." ) );
        YCT_ASSERT_EQUAL( 0, get_cwd16l( n, temp, sizeof( temp ) ) );
        YCT_ASSERT_EQUAL( 0, rmdir16l( buf ) );
        YCT_ASSERT_EQUAL( 0, get_cwd16l( n, temp, sizeof( temp ) ) );
        YCT_ASSERT_EQUAL( 0, mkdir16l( long_dir ) );
        YCT_ASSERT_EQUAL( 0, chdir16l( long_dir ) );
        YCT_ASSERT_EQUAL( 0, get_cwd16l( n, buf2, sizeof( buf2 ) ) );
        YCT_ASSERT_EQUAL( 0, chdir16l( ".." ) );
        YCT_ASSERT_EQUAL( 0, get_cwd16l( n, buf2, 260 ) );
        YCT_ASSERT_EQUAL_STR( temp, buf2 );
        YCT_ASSERT_EQUAL( 0, rmdir16l( long_dir ) );
    }
}

YCT_TEST( unlink_and_rename )
{
    int attribs;

    strcat( strcpy( buf, "echo \"\" > " ), short_name );
    system( buf );

    set_status16l( 0 );
    YCT_ASSERT_MSG( get_status16l() == 0, "Status should be ZERO" );
    YCT_ASSERT_EQUAL( 0, rename16l( short_name, short_name2 ) );
    YCT_ASSERT_EQUAL( 0, rename16l( short_name2, short_name ) );
    YCT_ASSERT_EQUAL( 0, unlink16l( short_name ) );

    if( is_supported16l() != 0 )
    {
        set_status16l( 1 );
        YCT_ASSERT_MSG( get_status16l() != 0, "Status should be non-ZERO" );
        strcat( strcpy( buf, "echo \"\" > " ), short_name );
        system( buf );

        YCT_ASSERT_MSG( get_attrib16l( short_name, &attribs) == 0,
                        "File should exist" );

        YCT_ASSERT_EQUAL( 0, rename16l( short_name, long_filename ) );

        YCT_ASSERT_EQUAL( 0, get_short16l( buf, sizeof( buf ),
                                           long_filename ) );

        YCT_ASSERT_EQUAL( 0, get_long16l( buf2, sizeof( buf2 ), buf ) );

        YCT_ASSERT_EQUAL_STR( long_filename,
                            buf2 + strlen( buf2 ) - strlen( long_filename ) );

        YCT_ASSERT_EQUAL( 0, rename16l( long_filename, long_filename2 ) );
        YCT_ASSERT_EQUAL( 0, rename16l( long_filename2, short_name ) );
        YCT_ASSERT_EQUAL( 0, rename16l( short_name, long_filename ) );
        YCT_ASSERT_EQUAL( 0, rename16l( long_filename, short_name ) );
        YCT_ASSERT_EQUAL( 0, rename16l( short_name, short_name2 ) );
        YCT_ASSERT_EQUAL( 0, unlink16l( short_name2 ) );
        YCT_ASSERT( get_attrib16l( short_name2, &attribs ) > 0 );
        strcat( strcpy( buf, "echo \"\" > " ), short_name );
        system( buf );
        YCT_ASSERT_EQUAL( 0, rename16l( short_name, long_filename ) );
        YCT_ASSERT_EQUAL( 0, unlink16l( long_filename ) );
        YCT_ASSERT( get_attrib16l( buf, &attribs ) > 0 );
    }
}

static void tear_down(void)
{
    unlink16l( short_name );
    unlink16l( short_name2 );
    unlink16l( long_filename );
    unlink16l( long_filename2 );
    rmdir16l( short_dir );
    rmdir16l( long_dir );
}

YCT_SUITE( suite, NULL, tear_down )
{
    YCT_TEST_ADD( dirs_and_status );
    YCT_TEST_ADD( unlink_and_rename );
}
YCT_SUITE_END

int main( int argc, const char** argv )
{
    YCT_BEGIN("DOS16LFN Test");
    YCT_SET_FULL_BLOCKED();
    YCT_TEST_RUN( NULL_Pointers_small_buffers );
    YCT_SUITE_RUN( suite );
    YCT_DUMP();
    YCT_END();

    return 0;
}
