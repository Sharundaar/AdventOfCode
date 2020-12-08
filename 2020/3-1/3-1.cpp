#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../basics/basics.h"
#include "../basics/basics.cpp"
#include "../basics/file_parser.h"

int main( int argc, char* argv[] )
{
    std::ifstream file( "3-1/input.txt" );
    defer{ file.close(); };

    constexpr int height = 323;
    constexpr int width = 31;
    char data[height][width];

    if( file.is_open() )
    {
        std::string line;
        int y = 0;
        while( std::getline( file, line ) )
        {
            if( is_line_empty_or_whitespace( line.c_str() ) ) continue;
            for( int i=0; i<line.size(); ++i )
                data[y][i] = line[i];
            ++y;
        }
    }

    for( int y=0; y<height; ++y )
    {
        for( int x=0; x<width; ++x )
        {
            Com_Printf( "%c", data[y][x] );
        }
        Com_Printf( "\n" );
    }


    int x = 0, y = 0;
    int tree_count = 0;
    while( true )
    {
        x = ( x + 3 ) % width;
        y += 1;

        if( y > height ) break;

        if( data[y][x] == '#' )
            tree_count++;
    }
    Com_Println( "Tree encountered: %i.", tree_count );
    return 1;
}