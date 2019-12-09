#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

#include "../helpers/types.h"

using namespace std;

bool meet_criteria( uint pwd )
{
    uint i0 = pwd / 100000;
    uint i1 = ( pwd - i0 * 100000 ) / 10000;
    uint i2 = ( pwd - i0 * 100000 - i1 * 10000 ) / 1000;
    uint i3 = ( pwd - i0 * 100000 - i1 * 10000 - i2 * 1000 ) / 100;
    uint i4 = ( pwd - i0 * 100000 - i1 * 10000 - i2 * 1000 - i3 * 100 ) / 10;
    uint i5 = ( pwd - i0 * 100000 - i1 * 10000 - i2 * 1000 - i3 * 100 - i4 * 10 );

    bool always_increasing = i0 <= i1 && i1 <= i2 && i2 <= i3 && i3 <= i4 && i4 <= i5;
    bool at_least_two_adjacent_are_same = (i0 == i1 && i1 != i2) || (i0 != i1 && i1 == i2 && i2 != i3) || (i1 != i2 && i2 == i3 && i3 != i4) || ( i2 != i3 && i3 == i4 && i4 != i5 ) || ( i3 != i4 && i4 == i5 );
    
    return always_increasing && at_least_two_adjacent_are_same;
}

int main()
{
    const char* input = "158126-624574";
    
    uint range_start = 158126;
    uint range_end = 624574;

    uint meet_criteria_count = 0;
    for( uint pwd = range_start; pwd <= range_end; ++pwd )
    {
        if( meet_criteria( pwd ) ) meet_criteria_count++;
    }
    cout << meet_criteria_count << endl;

    return 0;
}