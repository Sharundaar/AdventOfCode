 #include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../basics/basics.h"
#include "../basics/basics.cpp"
#include "../basics/file_parser.h"

int main( int argc, char* argv[] )
{
    std::ifstream file( "5-1/input.txt" );
    defer{ file.close(); };

    constexpr int height = 323;
    constexpr int width = 31;
    
    // parse data
    if( file.is_open() )
    {
        std::string line;
        int highest_id = 0;
        while( std::getline( file, line ) )
        {
            if( is_line_empty_or_whitespace( line.c_str() ) || is_new_line( *line.c_str() ) )
                continue;

            int row_range_begin = 0;
            int row_range_end = 128;
            int column_range_begin = 0;
            int column_range_end = 8;
            for( char c : line )
            {
                if( c == 'F' ) // cut row_range_end
                    row_range_end = (row_range_end - row_range_begin) / 2 + row_range_begin;
                else if( c == 'B' ) // cut row_range_begin
                    row_range_begin = (row_range_end - row_range_begin) / 2 + row_range_begin;
                else if( c == 'R' ) // cut column_range_begin
                    column_range_begin = (column_range_end - column_range_begin) / 2 + column_range_begin;
                else if( c == 'L' ) // cut column_range_begin
                    column_range_end = (column_range_end - column_range_begin) / 2 + column_range_begin;
            }
            int id = row_range_begin * 8 + column_range_begin;
            if( id > highest_id )
                highest_id = id;
        }
        Com_Println( "Highest ID: %d.", highest_id );
    }


    return 1;
}