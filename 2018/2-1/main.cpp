#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

using namespace std;

typedef unsigned int uint;
typedef unsigned long ulong;

int main()
{
    ifstream file( "input.txt" );
    if( file )
    {
        string line;

        ulong numTwoTimes = 0;
        ulong numThreeTimes = 0;

        uint letters[26];

        while( getline( file, line ) )
        {
            if( line[0] == '\0' )
                continue;

            memset(letters, 0, 26 * sizeof( ulong ));
            for( const char* c=line.c_str(); *c != '\0'; ++c )
                letters[*c-'a']++;

            bool hasTwo = false;
            bool hasThree = false;

            for( int i=0; i<26; ++i )
            {
                if( letters[i] == 2 )
                    hasTwo = true;
                if( letters[i] == 3 )
                    hasThree = true;
            }

            if(hasTwo) numTwoTimes++;
            if(hasThree) numThreeTimes++;
        }

        cout << numTwoTimes * numThreeTimes << endl;
    }
    
}