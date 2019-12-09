#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>

#include "../helpers/types.h"

using namespace std;

struct orbit
{
    char source[4];
    char orbiter[4];
};

int get_id( const vector<char*>& id_map, const char* obj_name )
{
    for( int i=0; i<id_map.size(); ++i )
    {
        if( strcmp( id_map[i], obj_name ) == 0 )
            return i;
    }

    assert( false );
    return -1;
}

int main()
{
    ifstream file( "input.txt" );
    if( file )
    {
        string line;
        vector<orbit> orbits;
        
        while( getline( file, line ) )
        {
            orbit o;
            memcpy( o.source, line.c_str(), 3 * sizeof( char ) );
            o.source[3] = '\0';
            memcpy( o.orbiter, line.c_str() + 4, 3 * sizeof( char ) );
            o.orbiter[3] = '\0';
            orbits.emplace_back( o );
        }

        vector<char*> object_to_id_map;
        for( auto& o: orbits )
        {
            bool found_orbiter = false;
            bool found_source = false;
            for( char* id: object_to_id_map )
            {
                if( strcmp( id, o.orbiter ) == 0 )
                    found_orbiter = true;
                if( strcmp( id, o.source ) == 0 )
                    found_source = true;

                if( found_orbiter && found_source )
                    break;
            }

            if( !found_source )
                object_to_id_map.push_back( o.source );
            if( !found_orbiter )
                object_to_id_map.push_back( o.orbiter );
        }

        int object_count = object_to_id_map.size();
        vector<int> orbit_matrix( object_count * object_count );
        for( const auto& o : orbits )
        {
            int source_id = get_id( object_to_id_map, o.source );
            int orbiter_id = get_id( object_to_id_map, o.orbiter );

            orbit_matrix[ orbiter_id + source_id * object_count ] = 1;
        }

        int orbit_count = 0;
        for( int orbiter=0; orbiter<object_count; ++orbiter )
        {
            
        }

    }

    return 0;
}