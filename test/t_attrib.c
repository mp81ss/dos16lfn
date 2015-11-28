#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dos.h> /* read/write/close */
#include <io.h>  /* lseek */

#include <dos16lfn.h>

#define YCT_DISABLE_INT64
#include <yacut.h>


static const char long_filename[] = "A Very Long Filename.Long Extension";
static const char short_name[] = "file.ext";
static char buf[256];


YCT_TEST( attribs )
{
    const char* pActualBuf;
    int i, attribs, attribs2;
    int j, a, r, h, s;

    strcat( strcpy( buf, "echo \"\" > " ), short_name );
    system( buf );

    for( i = 0; i < 2; ++i )
    {
        set_status16l( i );
        pActualBuf = i == 0 ? buf : long_filename;

        if( i == 0 ) strcpy( buf, short_name );
        else
        {
            YCT_ASSERT_EQUAL( 0, rename16l( short_name, long_filename ) );

            YCT_ASSERT_EQUAL( 0, get_short16l( buf, sizeof( buf ),
                                                    long_filename ) );
        }

        if( i == 0 || is_supported16l() != 0 )
        {
            attribs = O_RDONLY_16L;
            YCT_ASSERT_EQUAL( 0, set_attrib16l( pActualBuf, attribs ) );
            YCT_ASSERT_EQUAL( 0, get_attrib16l( pActualBuf, &attribs2 ) );
            YCT_ASSERT_EQUAL( attribs, attribs2 );

            attribs = ARCHIVE_16L;
            YCT_ASSERT_EQUAL( 0, set_attrib16l( pActualBuf, attribs ) );
            YCT_ASSERT_EQUAL( 0, get_attrib16l( pActualBuf, &attribs2 ) );
            YCT_ASSERT_EQUAL( attribs, attribs2 );

            attribs = HIDDEN_16L;
            YCT_ASSERT_EQUAL( 0, set_attrib16l( pActualBuf, attribs ) );
            YCT_ASSERT_EQUAL( 0, get_attrib16l( pActualBuf, &attribs2 ) );
            YCT_ASSERT_EQUAL( attribs, attribs2 );

            attribs = SYSTEM_16L;
            YCT_ASSERT_EQUAL( 0, set_attrib16l( pActualBuf, attribs ) );
            YCT_ASSERT_EQUAL( 0, get_attrib16l( pActualBuf, &attribs2 ) );
            YCT_ASSERT_EQUAL( attribs, attribs2 );

            attribs = NONE_16L;
            YCT_ASSERT_EQUAL( 0, set_attrib16l( pActualBuf, attribs ) );
            YCT_ASSERT_EQUAL( 0, get_attrib16l( pActualBuf, &attribs2 ) );
            YCT_ASSERT_EQUAL( attribs, attribs2 );

            for( j = 0; j < 16; j++ )
            {
                r = j & 1 ? O_RDONLY_16L : 0;
                h = j & 2 ? HIDDEN_16L : 0;
                s = j & 4 ? SYSTEM_16L : 0;
                a = j < 8 ? 0 : ARCHIVE_16L;
                attribs = a | r | h | s;

                YCT_ASSERT_EQUAL( 0, set_attrib16l( pActualBuf, attribs ) );
                YCT_ASSERT_EQUAL( 0, get_attrib16l( pActualBuf, &attribs2 ) );
                YCT_ASSERT_EQUAL( attribs, attribs2 );
            }
        }
    }

    YCT_ASSERT_EQUAL( 0, get_attrib16l( ".", &attribs2 ) );
    YCT_ASSERT_MSG( IS_DIR16L( attribs2 ) != 0,
                    "folder attribute not detected" );

    if( is_supported16l() != 0 )
    {
        YCT_ASSERT_EQUAL( 0, rename16l( long_filename, short_name ) );
    }
    YCT_ASSERT_EQUAL( 0, set_attrib16l( short_name, NONE_16L ) );
    YCT_ASSERT_EQUAL( 0, unlink16l( short_name ) );
}

static void tear_down(void)
{
    unlink16l( short_name );
    unlink16l( long_filename );
}

YCT_SUITE( suite, NULL, tear_down )
{
    YCT_TEST_ADD( attribs );
}
YCT_SUITE_END

int main( int argc, const char** argv )
{
    YCT_BEGIN("DOS16LFN Attributes Test");
    YCT_SET_FULL_BLOCKED();
    YCT_SUITE_RUN( suite );
    YCT_DUMP();
    YCT_END();

    return 0;
}
