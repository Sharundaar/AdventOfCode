#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "../basics/basics.h"
#include "../basics/basics.cpp"
#include "../basics/file_parser.h"

enum class CellType
{
    Error,
    Floor,
    Empty,
    Occupied
};

struct Automata
{
    int width;
    int height;
    CellType* cells;

    bool inside( int x, int y )
    {
        return 0 <= x && x < width 
            && 0 <= y && y < height;
    }

    CellType at( int x, int y )
    {
        if( inside( x, y ) )
            return cells[x + y * width];

        return CellType::Floor;
    }
};

void copy_automata( Automata& dst, const Automata& src )
{
    assert( dst.width == src.width && dst.height == src.height );
    memcpy( dst.cells, src.cells, sizeof( CellType ) * dst.width * dst.height );
}

CellType cell_from_char( char c )
{
    switch( c )
    {
        case '.': return CellType::Floor;
        case 'L': return CellType::Empty;
        case '#': return CellType::Occupied;
    }
    return CellType::Error;
}

void print_automata( const Automata& automata )
{
    for( int i=0; i<automata.width*automata.height; ++i )
    {
        char c;
        switch( automata.cells[i] )
        {
            case CellType::Floor: c = '.'; break;
            case CellType::Empty: c = 'L'; break;
            case CellType::Occupied: c = '#'; break;
            case CellType::Error: c = 'E'; break;
        }
        if( i % automata.width == automata.width - 1 ) Com_Println( "%c", c );
        else                                            Com_Printf( "%c", c );
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

        Automata automata_1;
        Automata automata_2;

        // compute width and height
        int width = 0, height = 0;
        while( std::getline( file, line ) )
        {
            if( is_line_empty_or_whitespace( line.c_str() ) || is_new_line( line[0] ) )
                continue;

            width = I_strlen( line.c_str() );
            height++;
        }

        automata_1.width = width;
        automata_1.height = height;
        automata_2 = automata_1;
        automata_1.cells = new CellType[width * height];
        automata_2.cells = new CellType[width * height];

        file.clear();
        file.seekg( std::ios_base::beg );
        int cell_idx = 0;
        while( std::getline( file, line ) )
        {
            if( is_line_empty_or_whitespace( line.c_str() ) || is_new_line( line[0] ) )
                continue;

            for( char c : line )
            {
                automata_1.cells[cell_idx++] = cell_from_char( c );
                assert(automata_1.cells[cell_idx-1] != CellType::Error);
            }
        }
        copy_automata( automata_2, automata_1 );

        Automata* current = &automata_1;
        Automata* next    = &automata_2;
        auto other_automata = [&]( Automata* a ) {
            if( a == &automata_1 ) return &automata_2;
            return &automata_1;
        };

        width = current->width; height = current->height; 
        bool any_change;
        do
        {
            any_change = false;

            for( int i=0; i < width * height; ++i )
            {
                const int x = i % width;
                const int y = i / width;
                CellType estimated_cell = current->cells[i];
                
                
                int occupied_neighbour_count = 0;
                // left
                for( int nx = x-1; nx >= 0; --nx )
                {
                    if( current->at( nx, y ) == CellType::Occupied )
                        occupied_neighbour_count++;
                    if( current->at( nx, y ) == CellType::Occupied || current->at( nx, y ) == CellType::Empty )
                        break;
                }
                // right
                for( int nx = x+1; nx < width; ++nx )
                {
                    if( current->at( nx, y ) == CellType::Occupied )
                        occupied_neighbour_count++;
                    if( current->at( nx, y ) == CellType::Occupied || current->at( nx, y ) == CellType::Empty )
                        break;
                }
                // up
                for( int ny = y-1; ny >= 0; --ny )
                {
                    if( current->at( x, ny ) == CellType::Occupied )
                        occupied_neighbour_count++;
                    if( current->at( x, ny ) == CellType::Occupied || current->at( x, ny ) == CellType::Empty )
                        break;
                }
                // down
                for( int ny = y+1; ny < height; ++ny )
                {
                    if( current->at( x, ny ) == CellType::Occupied )
                        occupied_neighbour_count++;
                    if( current->at( x, ny ) == CellType::Occupied || current->at( x, ny ) == CellType::Empty )
                        break;
                }
                // up - left
                for( int nx = x-1, ny = y-1; nx >= 0 && ny >= 0; --nx, --ny )
                {
                    if( current->at( nx, ny ) == CellType::Occupied )
                        occupied_neighbour_count++;
                    if( current->at( nx, ny ) == CellType::Occupied || current->at( nx, ny ) == CellType::Empty )
                        break;
                }
                // up - right
                for( int nx = x+1, ny = y-1; nx < width && ny >= 0; ++nx, --ny )
                {
                    if( current->at( nx, ny ) == CellType::Occupied )
                        occupied_neighbour_count++;
                    if( current->at( nx, ny ) == CellType::Occupied || current->at( nx, ny ) == CellType::Empty )
                        break;
                }
                // down - left
                for( int nx = x-1, ny = y+1; nx >= 0 && ny < height; --nx, ++ny )
                {
                    if( current->at( nx, ny ) == CellType::Occupied )
                        occupied_neighbour_count++;
                    if( current->at( nx, ny ) == CellType::Occupied || current->at( nx, ny ) == CellType::Empty )
                        break;
                }
                // down - right
                for( int nx = x+1, ny = y+1; nx < width && ny < height; ++nx, ++ny )
                {
                    if( current->at( nx, ny ) == CellType::Occupied )
                        occupied_neighbour_count++;
                    if( current->at( nx, ny ) == CellType::Occupied || current->at( nx, ny ) == CellType::Empty )
                        break;
                }

                if( estimated_cell == CellType::Empty && occupied_neighbour_count == 0 )
                {
                    next->cells[i] = CellType::Occupied;
                    any_change = true;
                }
                else if( estimated_cell == CellType::Occupied && occupied_neighbour_count >= 5 )
                {
                    next->cells[i] = CellType::Empty;
                    any_change = true;
                }
            }

            copy_automata( *current, *next );

            print_automata( *current );
            Com_Println( "--------------" );
        } while( any_change );

        int occupied_count = 0;
        for( int i=0; i < width * height; ++i )
        {
            if( next->cells[i] == CellType::Occupied )
                occupied_count++;
        }
        Com_Println( "Number of occupied seats: %d", occupied_count );

    /* Print automata
        for( int i=0; i<width*height; ++i )
        {
            char c;
            switch( automata_1.cells[i] )
            {
                case CellType::Floor: c = '.'; break;
                case CellType::Empty: c = 'L'; break;
                case CellType::Occupied: c = '#'; break;
                case CellType::Error: c = 'E'; break;
            }
            if( i % width == width - 1 ) Com_Println( "%c", c );
            else                         Com_Printf( "%c", c );
        }
    */
    }


    return 1;
}