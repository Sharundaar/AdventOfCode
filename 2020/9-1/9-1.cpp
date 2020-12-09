 #include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../basics/basics.h"
#include "../basics/basics.cpp"
#include "../basics/file_parser.h"

int main( int argc, char* argv[] )
{
    std::ifstream file( "input.txt" );
    defer{ file.close(); };
    
    // parse data
    if( file.is_open() )
    {
        std::string line;
        constexpr int preamble_size = 25;
        std::vector<int> datas;
        for( int i=0; i<preamble_size; ++i )
        {
            std::getline( file, line );
            int value;
            try_parse_to_int( line, value );
            datas.push_back( value );
        }

        int next = 0;
        while( std::getline( file, line ) )
        {
            if( is_line_empty_or_whitespace( line.c_str() ) || is_new_line( line[0] ) )
                continue;
            
            int value;
            try_parse_to_int( line, value );
            bool found_pair = false;
            for( int i=0; i < preamble_size-1; ++i )
            {
                for( int j = i+1; j < preamble_size; ++j )
                {
                    if( datas[i] + datas[j] == value )
                        found_pair = true;

                    if( found_pair ) break;
                }
                if( found_pair ) break;
            }

            if( !found_pair )
            {
                Com_Println( "Found a number that's not the sum of the last %d numbers: %d", preamble_size, value );
                break;
            }

            datas[next++] = value;
            next = next % preamble_size;
        }
    }


    return 1;
}