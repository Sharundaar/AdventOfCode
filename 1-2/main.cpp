#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

int main()
{
    ifstream file( "input.txt" );
    if( file )
    {
        string line;
        long total = 0;
        unordered_map<long, bool> freqHistory;
        /*constexpr long freqHistorySize = 65536*2;
        long freqHistory[freqHistorySize];
        long lastHistorySet = -1;*/
        long foundRepFreq = 0;
        bool hasFoundRepFreq = false;

        while( !hasFoundRepFreq )
        {
            if( !getline( file, line ) )
            {
                file.clear();
                file.seekg(0);
                getline( file, line );
            }
            if( line[0] != '+' && line[0] != '-' )
                continue;

            bool inc = line[0] == '+';
            long value = stoi( line.c_str()+1 );
            if( inc ) total += value;
            else total -= value;

            auto it = freqHistory.find(total);
            if( it != freqHistory.cend() )
            {
                hasFoundRepFreq = true;
                foundRepFreq = total;
                break;
            }

            freqHistory[total] = true;
            
            /*
            for( long i = 0; i<lastHistorySet; ++i )
            {
                if( freqHistory[i] == freqHistory[lastHistorySet] )
                {
                    hasFoundRepFreq = true;
                    foundRepFreq = freqHistory[lastHistorySet];
                    break;
                }
            }
            */
        }

        /*if( !foundRepFreq && lastHistorySet == freqHistorySize-1 )
            cout << "Busted history size." << endl;*/
        if( !foundRepFreq )
            cout << "Didn't find any rep freq." << endl;
        else
            cout << foundRepFreq << endl;
    }
    
}