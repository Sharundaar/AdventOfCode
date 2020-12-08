#pragma once

#include <string>

bool is_number(char c)
{
    return '0' <= c && c <= '9';
}

bool is_hex_char(char c)
{
    return ( '0' <= c && c <= '9' ) || ( 'a' <= c && c <= 'f' ) || ( 'A' <= c && c <= 'F' );
}

bool is_letter(char c)
{
    return ( 'a' <= c && c <= 'z' ) || ( 'A' <= c && c <= 'Z' );
}

bool is_eof(char c)
{
    return c == '\0';
}

bool is_new_line(char c)
{
    return c == '\n';
}

bool is_empty_space(char c)
{
    return c == ' ';
}

bool is_eof_or_new_line(char c)
{
    return is_new_line( c ) || is_eof( c );
}

const char* chomp_empty_space(const char* ptr)
{
    while( is_empty_space( *ptr ) ) ptr++;
    return ptr;
}

bool is_line_empty_or_whitespace( const char* line )
{
    while( *line != '\0' )
    {
        if( *(line++) == ' ' ) continue;
        return false;
    }

    return true;
}

bool try_parse_to_int( const char* str, int& out )
{
    char* pEnd = nullptr;
    out = static_cast<int>( std::strtod( str, &pEnd ) );
    if( *pEnd == '\0' )
        return false;
    return true;
}

bool try_parse_to_uint( const char* str, uint& out )
{
    char* pEnd = nullptr;
    out = static_cast<uint>( std::strtod( str, &pEnd ) );
    if( *pEnd == '\0' )
        return false;
    return true;
}


bool try_parse_to_int( const std::string& str, int& out )
{
    return try_parse_to_int( str.c_str(), out );
}

bool try_parse_to_uint( const std::string& str, uint& out )
{
    return try_parse_to_uint( str.c_str(), out );
}
