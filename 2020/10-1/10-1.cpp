#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "../basics/basics.h"
#include "../basics/basics.cpp"
#include "../basics/file_parser.h"

#define FETCH_NEXT_VALUE( values, values_size, idx ) ( idx < values_size ? values[idx] : ( idx == values_size ? values[idx-1] + 3 : UINT_MAX ) )
inline static int fetch_next_value( const int* values, const int values_size, const int idx )
{
    return ( idx < values_size ? values[idx] : ( idx == values_size ? values[idx-1] + 3 : INT_MAX ) );
}

static std::vector<u64> memoized_arrengements;
static u64 count_arrengements( const int* values, const int values_size, int from_idx )
{
    if( from_idx == values_size ) return 1;

    int current_value = from_idx == -1 ? 0 : values[from_idx];
    u64 arrengements = 0;
    if( from_idx >= 0 && from_idx < values_size && memoized_arrengements[from_idx] != 0 ) return memoized_arrengements[from_idx];

    if( fetch_next_value( values, values_size, from_idx + 1 ) - current_value <= 3 ) arrengements += count_arrengements( values, values_size, from_idx + 1 );
    if( fetch_next_value( values, values_size, from_idx + 2 ) - current_value <= 3 ) arrengements += count_arrengements( values, values_size, from_idx + 2 );
    if( fetch_next_value( values, values_size, from_idx + 3 ) - current_value <= 3 ) arrengements += count_arrengements( values, values_size, from_idx + 3 );

    if( from_idx >= 0 && from_idx < values_size ) memoized_arrengements[from_idx] = arrengements;
    return arrengements;
}

int main( int argc, char* argv[] )
{
    std::ifstream file( "input.txt" );
    defer{ file.close(); };
    
    // parse data
    if( file.is_open() )
    {
        std::string line;
        std::vector<int> values;
        while( std::getline( file, line ) )
        {
            if( is_line_empty_or_whitespace( line.c_str() ) || is_new_line( line[0] ) )
                continue;
            int value; try_parse_to_int( line, value );
            values.push_back( value );
        }
        std::sort( values.begin(), values.end() );
        
        // Part 1
        int jolt_diff_1 = 0;
        int jolt_diff_3 = 0;
        int value_count = static_cast<int>( values.size() );
        for( int i = -1; i < value_count; ++i )
        {
            int current_value = i == -1 ? 0 : values[i];
            int next_value    = i == value_count - 1 ? current_value + 3 : values[i+1];

            int diff = next_value - current_value;
            assert( diff >= 1 && diff <= 3 );
            if( diff == 1 ) jolt_diff_1++;
            if( diff == 3 ) jolt_diff_3++;
        }

        Com_Println( "1 * 3 jolt diff = %d", jolt_diff_1 * jolt_diff_3 );

        // Part 2
        for( int i=0; i<values.size(); ++i ) memoized_arrengements.push_back( 0 );
        u64 possible_arrengements = count_arrengements( values.data(), values.size(), -1 );
        Com_Println( "number of possible arrengements: %llu", possible_arrengements );
    }


    return 1;
}