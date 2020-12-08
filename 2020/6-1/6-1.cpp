 #include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../basics/basics.h"
#include "../basics/basics.cpp"
#include "../basics/file_parser.h"

int main( int argc, char* argv[] )
{
    std::ifstream file( "6-1/input.txt" );
    defer{ file.close(); };
    
    // parse data
    if( file.is_open() )
    {
        std::string line;
        
        constexpr int question_count = 26; // a through z
        int total_yes = 0;
        bool yes[question_count] = { false };
        while( std::getline( file, line ) )
        {
            if( is_line_empty_or_whitespace( line.c_str() ) || is_new_line( line[0] ) )
            {
                // new group
                for( int i=0; i < ARRAY_SIZE( yes ) ; ++i )
                    total_yes += yes[i] ? 1 : 0;
                memset( yes, false, sizeof( yes ) );
                continue;
            }

            for( char c : line )
            {
                if( c >= 'a' && c <= 'z' )
                    yes[c - 'a'] = true;
            }
        }
        for( int i=0; i < ARRAY_SIZE( yes ) ; ++i )
                    total_yes += yes[i] ? 1 : 0;

        Com_Println( "Total yes: %d.", total_yes );
    }


    return 1;
}