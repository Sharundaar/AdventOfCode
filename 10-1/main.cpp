#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char uchar;

struct vector2
{
    union
    {
        struct { int x, y; };
        struct { int w, h; };
        int m[2];
    };

    vector2 operator+( const vector2& other ) { return {x+other.x, y+other.y}; }
    void operator+=( const vector2& other ) { x+=other.x; y+=other.y; }
    int operator[](int i) { return m[i]; }

    int area() { return w*h; }
};

struct entry
{
    vector2 pos;
    vector2 vel;
};


entry make_entry( const string& line )
{
    entry e;
    sscanf( line.c_str(), "position=<%d,%d> velocity=<%d,%d>", &e.pos.x, &e.pos.y, &e.vel.x, &e.vel.y );
    return e;
}

void compute_points_bb( const vector<entry>& entries, vector2& center, vector2& size )
{
    center = {0, 0};
    size   = {0, 0};

    vector2 min = {INT_MAX,INT_MAX}, max={INT_MIN, INT_MIN};
    for( int i=0; i<entries.size(); ++i )
    {
        center += entries[i].pos;
        min.x = min(size.x, )
    }
}

int main()
{
    ifstream file( "input.txt" );
    if( !file ) return 0;

    string line;
    vector<entry> entries;
    while( getline( file, line ) )
    {
        if( line[0] == '\0' )
            continue;

        entries.emplace_back( make_entry( line ) );
    }

    vector2 bounding_box_center;
    int prev_bounding_box_area = -1;
    vector2 bounding_box_size;

    while( prev_bounding_box_area == -1 || prev_bounding_box_area >= bounding_box_size.area() )
    {

    }

}