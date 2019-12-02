#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <limits.h>
#include <string>

#define TO_PX32( COL_8, alpha ) pixel32{ (uchar)(((COL_8 & 0b11100000) >> 5) * 36), (uchar)(((COL_8 & 0b00011100) >> 2) * 36), (uchar)((COL_8 & 0b00000011) * 36), alpha }

using namespace std;

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char uchar;

#pragma pack(push, 1)
struct bmp_header
{
    char header_field[2];
    uint size;
    char reserved[4];
    int data_offset;
};

struct dib_header
{
    uint size = sizeof(dib_header);
    int width;
    int height;
    ushort no_color_plane = 1;
    ushort bpp = 32;
    int compression;
    uint image_size;
    uint horizontal_resolution;
    uint vertical_resolution;
    int no_color_in_palette = 0;
    int no_color_important = 0;
};
#pragma pack(pop)


struct pixel32
{
    uchar r, g, b, a;
};

struct vector2
{
    union
    {
        struct { int x, y; };
        struct { int w, h; };
        int m[2];
    };

    vector2 operator+( const vector2& other ) const { return {x+other.x, y+other.y}; }
    vector2 operator-( const vector2& other ) const { return {x-other.x, y-other.y}; }
    void operator+=( const vector2& other ) { x+=other.x; y+=other.y; }
    void operator-=( const vector2& other ) { x-=other.x; y-=other.y; }
    int operator[](int i) { return m[i]; }

    long long area() { return ((long long)w)*((long long)h); }
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

void compute_points_bb( const vector<entry>& entries, vector2& min, vector2& max )
{
    min = {INT_MAX,INT_MAX};
    max = {INT_MIN, INT_MIN};
    for( int i=0; i<entries.size(); ++i )
    {
        min.x = ::min(min.x, entries[i].pos.x );
        min.y = ::min(min.y, entries[i].pos.y );
        max.x = ::max(max.x, entries[i].pos.x );
        max.y = ::max(max.y, entries[i].pos.y );
    }
}

void update_points( vector<entry>& entries )
{
    for( int i=0; i<entries.size(); ++i )
    {
        entries[i].pos += entries[i].vel;
    }
}

void rewind_points( vector<entry>& entries )
{
    for( int i=0; i<entries.size(); ++i )
    {
        entries[i].pos -= entries[i].vel;
    }
}

void output_area_to_image( const vector<entry>& entries, vector2 min, vector2 max, const char* img )
{
    int x, y;
    vector2 size = max-min;
    size.x+=1;
    size.y+=1;
    FILE * fp;

    bmp_header bh = {
        'B', 'M',
        sizeof(bh) + sizeof(dib_header) + size.w*size.h * sizeof(pixel32),
    };
    bh.data_offset = sizeof(bmp_header) + sizeof(dib_header);

    dib_header dh = {
        sizeof(dib_header),
        size.w,
        size.h,
        1,
        32,
        0,
        size.w*size.h * sizeof(pixel32),
        2835,
        2835,
        0,
        0
    };

    /* create new file, give it a name and open it in binary mode */
    fp = fopen(img, "wb");
    /* write header to the file */
    fwrite( &bh, sizeof(bmp_header), 1, fp );
    fwrite( &dh, sizeof(dib_header), 1, fp );
    
    /* write the pixels */
    for(int j=size.h-1; j>=0; --j)
    {
        for(int i=0; i<size.w; ++i)
        {
            pixel32 pixel;
            vector2 pos = {i+min.x, j+min.y};
            bool foundEntry = false;
            for(int k=0; k<entries.size() && !foundEntry; ++k)
                foundEntry = (entries[k].pos.x == pos.x && entries[k].pos.y == pos.y);
            if(foundEntry) pixel = TO_PX32(255, 255);
            else           pixel = TO_PX32(0, 255);

            fwrite( &pixel, sizeof(pixel32), 1, fp );
        }
    }
    fclose(fp);
    // printf("OK - file %s saved\n", img);
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

    vector2 min, max;
    vector2 size = {INT_MAX, INT_MAX};
    vector2 prev_size = {INT_MAX, INT_MAX};

    compute_points_bb( entries, min, max );
    size = max-min;

    int seconds = 0;
    while( size.x <= prev_size.x || size.y <= prev_size.y )
    {
        prev_size = size;
        update_points(entries);
        compute_points_bb( entries, min, max );
        size = max - min;
        seconds++;
    }

    rewind_points( entries );
    compute_points_bb( entries, min, max );
    seconds--;

    output_area_to_image( entries, min, max, "img.bmp" );
    printf("%d seconds\n", seconds);
}