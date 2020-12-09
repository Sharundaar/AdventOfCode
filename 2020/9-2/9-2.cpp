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
        std::vector<int> datas;
        while( std::getline( file, line ) )
        {
            if( is_line_empty_or_whitespace( line.c_str() ) || is_new_line( line[0] ) )
                continue;
            
            int value; try_parse_to_int( line, value );
            datas.push_back( value );
        }

        constexpr int target_number = 104054607;
        for( int i=0; i<datas.size(); ++i )
        {
            int k = i;
            int sum = 0;
            while( sum < target_number ) sum += datas[k++];
            if( sum == target_number )
            {
                int smallest = INT_MAX;
                int highest  = INT_MIN;
                for( int j = i; j<k; ++j )
                {
                    smallest = I_min( smallest, datas[j] );
                    highest  = I_max( highest, datas[j] );
                }
                Com_Println( "Found sum starting at %d and ending at %d, sum = %d.", i, k-1, smallest + highest );
                break;
            }
        }
    }


    return 1;
}