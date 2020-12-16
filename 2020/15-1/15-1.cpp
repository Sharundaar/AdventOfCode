#include <iostream>
#include <unordered_map>
#include <vector>

#include "../basics/basics.h"
#include "../basics/basics.cpp"
#include "../basics/file_parser.h"

int main( int argc, char* argv[] )
{
    std::vector<int> numbers = { 9, 12, 1, 4, 17, 0, 18 };
    std::unordered_map<int, int> last_turn_seen = {};
    last_turn_seen[9] = 0;
    last_turn_seen[12] = 1;
    last_turn_seen[1] = 2;
    last_turn_seen[4] = 3;
    last_turn_seen[17] = 4;
    last_turn_seen[0] = 5;

    while( numbers.size() < 30000000 )
    {
        int turn = numbers.size();
        int last_number = numbers[numbers.size()-1];
        auto it = last_turn_seen.find( last_number );
        if( it == last_turn_seen.cend() )
        {
            numbers.push_back( 0 );
        }
        else
        {
            int turn_diff = turn - 1 - it->second;
            numbers.push_back( turn_diff );
        }
        last_turn_seen[last_number] = turn-1;
    }

    Com_Println( "2020 number: %d", numbers[30000000-1] );

    return 1;
}