#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../basics/basics.h"
#include "../basics/basics.cpp"
#include "../basics/file_parser.h"

enum Mask
{
    SET_ZERO,
    SET_ONE,
    IGNORE,
};

struct Operation
{
    u64 position;
    u64 value;
};

struct Entry
{
    Mask mask[36];
    std::vector<Operation> ops;
};

static u64 get_zero_mask( Mask mask[36] )
{
    u64 zero_mask = 0;
    for( int i=0; i<36; ++i )
    {
        if( mask[i] == SET_ZERO ) zero_mask += 1;
        if( i < 35 )
            zero_mask = zero_mask << 1;
    }

    return zero_mask;
}

static u64 get_one_mask( Mask mask[36] )
{
    u64 one_mask = 0;
    for( int i=0; i<36; ++i )
    {
        if( mask[i] == SET_ONE ) one_mask += 1;
        if( i < 35 )
            one_mask = one_mask << 1;
    }

    return one_mask;
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
        Entry current_entry; bool current_entry_set = false;
        while( std::getline( file, line ) )
        {
            char buffer[512];
            if( is_line_empty_or_whitespace( line.c_str() ) || is_new_line( line[0] ) )
                continue;
            
            I_strcpy( buffer, line.c_str() );
            if( I_strispresent( buffer, "mask" ) )
            {
                if( current_entry_set )
                {
                    entries.emplace_back( current_entry );
                    current_entry_set = false;
                    memset( current_entry.mask, 0, sizeof( current_entry.mask ) );
                    current_entry.ops.clear();
                }

                current_entry_set = true;
                const char* it = buffer;
                while( *it != '=' ) ++it;
                ++it; ++it; // skipping = and empty space
                int mask_pos = 0;
                while( !is_eof_or_new_line( *it ) )
                {
                    assert( mask_pos < 36 );
                    current_entry.mask[mask_pos++] = ( *it == 'X' ? IGNORE : ( *it == '0' ? SET_ZERO : SET_ONE ) );
                    ++it;
                }
            }
            else if( I_strispresent( buffer, "mem" ) )
            {
                Operation op;
                sscanf_s( buffer, "mem[%lu] = %lu", &op.position, &op.value );
                current_entry.ops.emplace_back( op );
            }
        }
        if( current_entry_set )
            entries.emplace_back( current_entry );

        u64 memory[100000] = { 0 };
        for( int i=0; i<entries.size(); ++i )
        {
            auto& entry = entries[i];
            u64 zero_mask = get_zero_mask( entry.mask );
            u64 one_mask = get_one_mask( entry.mask );
            for( const Operation& op : entry.ops )
            {
                assert( op.position < ARRAY_SIZE( memory ) );
                memory[op.position] = ( ( op.value & ~zero_mask ) | one_mask );
            }
        }

        u64 sum = 0;
        for( int i=0; i < ARRAY_SIZE( memory ); ++i )
        {
            sum += memory[i];
        }
        Com_Println( "Memory sum: %llu", sum );

/*
        for( const Entry& entry : entries )
        {
            char mask_buffer[37] = { 0 };
            for( int i=0; i<36; ++i ) mask_buffer[i] = (entry.mask[i] == IGNORE ? 'X' : (entry.mask[i] == SET_ZERO ? '0' : '1'));
            Com_Println( "mask = %s", mask_buffer );
            for( const Operation& op : entry.ops )
            {
                Com_Println( "mem[%lu] = %lu", op.position, op.value );
            }
        }
*/
    }


    return 1;
}