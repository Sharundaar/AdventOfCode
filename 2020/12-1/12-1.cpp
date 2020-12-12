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

Direction turn_left( Direction dir )
{
    switch( dir )
    {
        case Direction::East: return Direction::North;
        case Direction::North: return Direction::West;
        case Direction::West: return Direction::South;
        case Direction::South: return Direction::East;
    }
}

Direction turn_right( Direction dir )
{
    switch( dir )
    {
        case Direction::East: return Direction::South;
        case Direction::North: return Direction::East;
        case Direction::West: return Direction::North;
        case Direction::South: return Direction::West;
    }
}

Direction turn( Direction dir, int degree )
{
    int sign = (degree < 0 ) ? -1 : 1;
    degree = I_abs( degree );
    while( degree > 0 )
    {
        if( sign > 0 ) dir = turn_right( dir );
        else           dir = turn_left( dir );
        degree -= 90;
    }
    return dir;
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
            try_parse_to_int( line.c_str()+1, entry.num );
            entries.push_back( entry );
        }

        Direction direction = Direction::East; // 0 means east
        int e = 0, n = 0;
        for( Entry entry : entries )
        {
            switch( entry.dir )
            {
                case 'N': n += entry.num; break;
                case 'S': n -= entry.num; break;
                case 'E': e += entry.num; break;
                case 'W': e -= entry.num; break;
                case 'L': direction = turn( direction, -entry.num ); break;
                case 'R': direction = turn( direction, entry.num ); break;
                case 'F': 
                    switch( direction )
                    {
                        case Direction::East: e += entry.num; break;
                        case Direction::North: n += entry.num; break;
                        case Direction::West: e -= entry.num; break;
                        case Direction::South: n -= entry.num; break;
                    }
                    break;
            }
        }
        Com_Println( "%d", I_abs( e ) + I_abs( n ) );
    }


    return 1;
}