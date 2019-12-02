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
        string saved_polymere;
        getline( file, saved_polymere );

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

        int smallest_length = 999999;
        char best_type = 0;
        for( char c='a'; c<='z'; ++c )
        {
            polymere = saved_polymere;
            for(int i=0; i<polymere.size(); ++i)
                if( tolower( polymere[i] ) == c ) polymere[i] = '#';
            while( step() );
            uint count = 0;
            for( int i=0; i<polymere.size(); ++i )
                if( polymere[i] != '#' ) ++count;
            cout << (char) c << ": " << count << endl;

            if( count < smallest_length )
            {
                smallest_length = count;
                best_type = c;
            }
        }

        cout << (char) best_type << endl;
    }
    
}