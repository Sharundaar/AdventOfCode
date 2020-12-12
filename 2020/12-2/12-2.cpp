 #include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../basics/basics.h"
#include "../basics/basics.cpp"
#include "../basics/file_parser.h"

struct Entry
{
    char dir;
    int  num;
};

enum class Direction
{
    East,
    North,
    West,
    South
};

void turn( int& x, int& y, int degree )
{
    int sign = (degree < 0 ) ? -1 : 1;
    degree = I_abs( degree );
    while( degree > 0 )
    {
        if( sign > 0 )
        {
            int tmp = y;
            y = -x;
            x = tmp;
        }
        else
        {
            int tmp = x;
            x = -y;
            y = tmp;
        }
        degree -= 90;
    }
}


int main( int argc, char* argv[] )
{
    std::ifstream file( "input.txt" );
    defer{ file.close(); };
    
    // parse data
    if( file.is_open() )
    {
        std::string line;
        std::vector<Entry> entries;
        while( std::getline( file, line ) )
        {
            if( is_line_empty_or_whitespace( line.c_str() ) || is_new_line( line[0] ) )
                continue;
            
            Entry entry;
            entry.dir = line[0];
            entry.num = parse_to_int( line.c_str()+1 );
            entries.push_back( entry );
        }

        Direction direction = Direction::East; // 0 means east
        int e = 0, n = 0;
        int we = 10, wn = 1;
        for( Entry entry : entries )
        {
            switch( entry.dir )
            {
                case 'N': wn += entry.num; break;
                case 'S': wn -= entry.num; break;
                case 'E': we += entry.num; break;
                case 'W': we -= entry.num; break;
                case 'L': turn( we, wn, -entry.num ); break;
                case 'R': turn( we, wn, entry.num ); break;
                case 'F':
                    e += entry.num * we;
                    n += entry.num * wn;
                    break;
            }
        }
        Com_Println( "%d", I_abs( e ) + I_abs( n ) );
    }


    return 1;
}