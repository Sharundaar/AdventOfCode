#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

typedef unsigned int uint;
typedef unsigned long ulong;

#define AREA_W 1000
#define AREA_H 1000

struct rect
{
    uint x, y, w, h;
};

struct entry
{
    uint id;
    rect r;
};

entry make_entry( const string& line )
{
    uint id, x, y, w, h;
    sscanf( line.c_str(), "#%d @ %d,%d: %dx%d", &id, &x, &y, &w, &h );
    return entry {
        id, rect{ x, y, w, h }
    };
}

void print_entry( const entry& e )
{
    printf( "#%d @ %d,%d: %dx%d\n", e.id, e.r.x, e.r.y, e.r.w, e.r.h );
}

rect rect_intersect( const rect& r1, const rect& r2 )
{
    uint x11 = r1.x, x12 = r1.x+r1.w, y11=r1.y, y12 = r1.y+r1.h;
    uint x21 = r2.x, x22 = r2.x+r2.w, y21=r2.y, y22 = r2.y+r2.h;

    uint newLeft = (x11 > x21) ? x11 : x21;
    uint newTop = (y11 > y21) ? y11 : y21;
    uint newRight = (x12 < x22) ? x12 : x22;
    uint newBottom = (y12 < y22) ? y12 : y22;
    if ((newRight > newLeft) && (newBottom > newTop))
        return rect{newLeft, newTop, newRight - newLeft, newBottom - newTop};
    return rect{0,0,0,0};
}

uint rect_area(const rect& r)
{
    return r.w*r.h;
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

        constexpr auto AREA_SIZE = AREA_W*AREA_H;
        bool area[AREA_SIZE];
        memset( area, false, AREA_SIZE*sizeof(bool));
        rect intersect;
        for( int i=0; i<entries.size()-1; ++i )
        {
            for( int j=i+1; j<entries.size(); ++j )
            {
                intersect = rect_intersect( entries[i].r, entries[j].r );
                if( rect_area(intersect) > 0 )
                {
                    for( uint y=intersect.y; y<intersect.y+intersect.h; ++y )
                        for( uint x=intersect.x; x<intersect.x+intersect.w; ++x )
                            area[y*AREA_W+x] = true;
                }
            }
        }

        uint count_inches = 0;
        for( int i=0; i<AREA_SIZE; ++i )
            if( area[i] ) ++count_inches;

        cout << count_inches << endl;
    }
    
}