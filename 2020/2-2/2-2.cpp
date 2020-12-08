#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../basics/basics.h"
#include "../basics/basics.cpp"
#include "../basics/file_parser.h"

struct Data
{
    int low;
    int high;
    char letter;
    std::string password;
};

int main( int argc, char* argv[] )
{
    std::ifstream file( "2-1/input.txt" );
    defer{ file.close(); };

    std::vector<Data> datas;

    if( file.is_open() )
    {
        std::string line;
        char buffer[256];
        while( std::getline( file, line ) )
        {
            if( is_line_empty_or_whitespace( line.c_str() ) ) continue;

            Data data;
            const char* str = line.c_str();
            sscanf_s( str, "%i-%i %c:", &data.low, &data.high, &data.letter );
            while( *str != ':' ) ++str;
            str += 2; // skip ': '
            data.password = str;
            datas.push_back( std::move( data ) );
        }
    }


    int valid_count = 0;
    for( const Data& data: datas )
    {
        if( ( data.password[data.low-1] == data.letter ) ^ ( data.password[data.high-1] == data.letter ) )
            valid_count++;
    }
    Com_Println( "Valid count: %i.", valid_count );
    return 1;
}