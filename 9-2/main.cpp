#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char uchar;

template<typename T>
struct dyn_pool
{
    T* pool;
    uint allocated;

    ~dyn_pool() { delete[] pool; }

    void init( uint size ) { pool = new T[size]; }
    T* allocate() { return &pool[allocated++]; }
};

struct marble
{
    int value;
    marble* cw;
    marble* ccw;
};

using MarblePool = dyn_pool<marble>;

struct game_state
{
    marble* current_marble;
    MarblePool marble_pool;
};

int main()
{
    ifstream file( "input.txt" );
    if( !file ) return 0;

    string line;
    getline( file, line );

    uint player_count, last_marble_points;
    sscanf( line.c_str(), "%d players; last marble is worth %d points", &player_count, &last_marble_points );

    last_marble_points *= 100;

    vector<uint> player_scores( player_count );
    game_state state;
    state.marble_pool.init( last_marble_points + 1 );

    // setup marble 0
    state.current_marble = state.marble_pool.allocate();
    *state.current_marble = { 0, state.current_marble };

    auto zero_marble = state.current_marble;

    uint next_marble_value = 1;
    uint turn = 1;
    while( next_marble_value <= last_marble_points )
    {
        if( next_marble_value % 23 == 0 )
        {
            player_scores[ turn-1 ] += next_marble_value++;
            auto marble = state.current_marble;
            for( int i=0; i<7; ++i ) marble = marble->ccw;
            player_scores[ turn-1 ] += marble->value;
            marble->cw->ccw = marble->ccw;
            marble->ccw->cw = marble->cw;
            // marble_pool.deallocate( marble ) // we don't care about that...

            state.current_marble = marble->cw;
        }
        else
        {
            auto new_marble = state.marble_pool.allocate();
            new_marble->value = next_marble_value++;

            auto* next_from_current = state.current_marble->cw;
            new_marble->cw = next_from_current->cw;
            new_marble->cw->ccw = new_marble;
            new_marble->ccw = next_from_current;
            next_from_current->cw = new_marble;

            state.current_marble = new_marble;
        }

        ++turn;
        if( turn == player_count+1 )
            turn = 1;
    }

    auto it = max_element(begin(player_scores), end(player_scores));
    cout << *it << endl;
}