#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct entry
{
    int mass = 0;
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

        int sum = 0;
        for( entry& e: entries )
        {
            int entry_required_mass = e.mass / 3 - 2;
            sum += entry_required_mass;
        }

        cout << sum << endl;
    }
}