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

    std::vector<int> datas;

    if( file.is_open() )
    {
        std::string line;
        while( std::getline( file, line ) )
        {
            if( is_line_empty_or_whitespace( line.c_str() ) ) continue;

            int data;
            sscanf_s( line.c_str(), "%i", &data );
            datas.push_back( data );
        }
    }


    for( int i=0; i<datas.size()-1; ++i )
    {
        for( int j=i+1; j<datas.size(); ++j )
        {
            int sum = datas[i] + datas[j];
            if( sum == 2020 )
            {
                Com_Println( "Found 2020 sum at i=%i and j=%i, product = %i.", i, j, datas[i] * datas[j] );
                return 0;
            }
        }
    }
    return 1;
}