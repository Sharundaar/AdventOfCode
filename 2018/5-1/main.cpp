#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <ctype.h>

using namespace std;

typedef unsigned int uint;
typedef unsigned long ulong;

#define AREA_W 1000
#define AREA_H 1000

int main()
{
    ifstream file( "input.txt" );
    if( file )
    {
        string polymere;
        getline( file, polymere );

        auto step = [&]() {
            bool found=false;
            for( int i=0; i<polymere.size()-1; ++i )
            {
                if(polymere[i] == '#') continue;

                int next_idx = i+1;
                while( next_idx < polymere.size() && polymere[next_idx] == '#' ) ++next_idx;
                if( next_idx >= polymere.size() ) break;

                if( tolower( polymere[i] ) == tolower( polymere[next_idx] ) && polymere[next_idx] != polymere[i] )
                {
                    polymere[i] = '#';
                    polymere[next_idx] = '#';
                    found = true;
                }
            }
            return found;
        };

        while( step() );

        uint count = 0;
        for( int i=0; i<polymere.size(); ++i )
            if( polymere[i] != '#' ) ++count;
        cout << count << endl;
    }
    
}