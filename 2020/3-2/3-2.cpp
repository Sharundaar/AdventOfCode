#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../basics/basics.h"
#include "../basics/basics.cpp"
#include "../basics/file_parser.h"

struct Slope
{
    int right;
    int down;
    int result;
};

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

    std::vector<Slope> slopes = {
        { 1, 1, 0 },
        { 3, 1, 0 },
        { 5, 1, 0 },
        { 7, 1, 0 },
        { 1, 2, 0 }
    };

    long long result = 1;
    for( Slope& slope: slopes )
    {
        int x = 0, y = 0;
        int tree_count = 0;
        while( true )
        {
            x = ( x + slope.right ) % width;
            y += slope.down;

            if( y > height ) break;

            if( data[y][x] == '#' )
                tree_count++;
        }
        slope.result = result;
        result *= tree_count;
    }

    Com_Println( "result: %d.", result );
    return 1;
}