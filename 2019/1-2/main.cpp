#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct entry
{
    int mass = 0;
    int required_fuel = 0;
};


entry make_entry( const string& line )
{
    entry e = {};
    sscanf( line.c_str(), "%d", &e.mass );
    return e;
}

int main()
{
    ifstream file( "input.txt" );
    if( file )
    {
        string line;
        vector<entry> entries;

        while( getline( file, line ) )
        {
            if( line[0] == '\0' )
                continue;
            entries.push_back( make_entry( line ) );
        }

        for( entry& e: entries )
        {
            int entry_required_fuel = e.mass / 3 - 2;

            while( entry_required_fuel > 0 )
            {
                e.required_fuel += entry_required_fuel;
                entry_required_fuel = entry_required_fuel / 3 - 2;
            }
        }

        int sum = 0;
        for( entry& e: entries )
            sum += e.required_fuel;                

        cout << sum << endl;
    }
}