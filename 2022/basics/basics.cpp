#include "basics.h"

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <cstdlib>

#include <crtdbg.h>

bool I_strcmp( const char* s1, const char* s2 )
{
    return strcmp( s1, s2 ) == 0;
}

bool I_stricmp( const char* s1, const char* s2 )
{
    return _stricmp( s1, s2 ) == 0;
}

size_t I_strlen( const char* str )
{
    return strlen( str );
}

void I_strcpy( char* dest, size_t size, const char* src )
{
    strcpy_s( dest, size, src );
}

bool I_strispresent( const char* haystack, const char* str )
{
    return strstr( haystack, str ) != nullptr;
}

bool Com_Assert( bool condition, const char* message, const char* file, u32 line )
{
    if( !condition )
    {
        Com_PrintError( "Assert triggered %s:%d : %s\n", file, line, message );
        _CrtDbgBreak();
    }
    return condition;
}

int Com_sprintf( char* dest, size_t size, const char* fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    int res = vsprintf_s( dest, size, fmt, args );
    va_end( args );
    return res;
}

void Com_Printf( const char* fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    vprintf_s( fmt, args );
    va_end( args );
    fflush( stdout );
}

void Com_Println( const char* fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    vprintf_s( fmt, args );
    va_end( args );
    putc( '\n', stdout );
    fflush( stdout );
}

void Com_PrintError( const char* fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    vfprintf_s( stderr, fmt, args );
    va_end( args );
    fflush( stderr );
}

void Com_FatalError( int errCode, const char* fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    vfprintf_s( stderr, fmt, args );
    va_end( args );
    fflush( stderr );

    exit( errCode );
}

void Com_EllipseString( char* dest, size_t size, const char* src )
{
    u32 idx = 0;
    const char ellipse[] = "...";
    const size_t ellipse_length = I_strlen( ellipse );

    while( true )
    {
        if( src[idx] == '\0' ) { dest[idx] = '\0'; break; }
        if( idx == size-ellipse_length-1 ) { strcpy_s( dest+idx, ellipse_length+1, ellipse ); break; }
        dest[idx] = src[idx];
        idx++;
    }
}
