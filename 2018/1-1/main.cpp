#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    ifstream file( "input.txt" );
    if( file )
    {
        string line;
        long total = 0;
        while( getline( file, line ) )
        {
            bool inc = line[0] == '+';
            long value = stoi( line.c_str()+1 );
            if( inc ) total += value;
            else total -= value;
        }
        cout << total << endl;
    }
    
}