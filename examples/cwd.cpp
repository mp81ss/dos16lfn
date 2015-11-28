#include <iostream.h>
#include <dos16lfn.h>

#define BUF_SIZE 1024


int main( int argc, const char** argv )
{
    char buf[BUF_SIZE];
    int n;

    if( argc == 1 )
        n = get_cwd16l(0, buf, sizeof( buf ) );
    else
        n = get_long16l( buf, sizeof( buf ), argv[1] );

    if( n == 0 )
        cout << buf << ends;
    else
        cout << "Error getting current directory" << ends;

    return n;
}
