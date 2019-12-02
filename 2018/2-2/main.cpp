#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

typedef unsigned int uint;
typedef unsigned long ulong;

#define ENTRY_SIZE 26

struct entry
{
    char data[ENTRY_SIZE];
    char operator[]( int i ) const { return data[i]; }
};

entry make_entry( const string& line )
{
    entry e;
    int idx = 0;
    while( line[idx] != '\0' )
    {
        e.data[idx] = line[idx];
        ++idx;
    }

    return e;
}

entry sub_entries( const entry& e1, const entry& e2 )
{
    entry e;
    for( int i=0; i<ENTRY_SIZE; ++i )
    {
        e.data[i] = e1.data[i] - e2.data[i];
    }
    return e;
}

uint count_non_zero( const entry& e )
{
    uint non_zero = 0;
    for( int i=0; i<ENTRY_SIZE; ++i )
    {
        if( e.data[i] != 0 ) non_zero++;
    }
    return non_zero;
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

            entries.emplace_back( make_entry( line ) );
        }

        const int numEntries = entries.size();
        entry sub;
        for( int i=0; i<entries.size()-1; ++i )
        {
            for( int j=i+1; j<entries.size(); ++j )
            {
                sub = sub_entries( entries[i], entries[j] );
                if( count_non_zero(sub) == 1 )
                {
                    for( int k=0; k<ENTRY_SIZE; ++k )
                    {
                        if( sub[k] == 0 )
                            cout << entries[i][k];
                    }
                    cout << endl;
                    return 0;
                }
            }
        }
    }
    
}