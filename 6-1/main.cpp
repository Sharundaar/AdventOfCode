#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <climits>

using namespace std;

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char uchar;

#define INVALID_AREA 0x10000000
#define CONFLICTED_AREA 0x10000001

#define TO_PX32( COL_8, alpha ) pixel32{ (uchar)(((COL_8 & 0b11100000) >> 5) * 36), (uchar)(((COL_8 & 0b00011100) >> 2) * 36), (uchar)((COL_8 & 0b00000011) * 36), alpha }
#define DEBUG_PRINT printf( "#### EVERYTHING IS FINE HERE #### (L: %d)", __LINE__ );

struct entry
{
    int x, y;
};

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

struct pixel32
{
    uchar r, g, b, a;
};
#pragma pack(pop)

entry make_entry( const string& line )
{
    entry e = {};
    sscanf( line.c_str(), "%d, %d", &e.x, &e.y );

    return e;
}

void print_entry( const entry& e )
{
    printf( "%d, %d\n", e.x, e.y );
}

void print_area( const vector<uint>& area, uint w )
{
    for( int i=0; i< area.size(); ++i )
    {
        if( area[i] == CONFLICTED_AREA )
            printf(".");
        else
            printf("%d", area[i]);

        if( i % w == w-1 )
            printf("\n");
    }

    printf("\n");
}

static vector<char> color_palette;
void generate_color_palette( int count )
{
    srand( 67688742 );
    for( int i=0; i<count; ++i )
        color_palette.push_back( rand() % 256 );
}

bool point_is_entry( uint x, uint y, const vector<entry>& entries )
{
    for(int i=0; i<entries.size(); ++i )
    {
        auto& entry = entries[i];
        if( entry.x == x && entry.y == y ) return true;
    }
    return false;
}

void output_area_to_image( const vector<entry>& entries, uint wx_min, uint wy_min, const vector<uint>& area, uint w, const char* img )
{
    int x, y;
    /* size of the image */
    const int x_max = w;  /* width */
    const int y_max = area.size() / w;  /* height */
    FILE * fp;

    bmp_header bh = {
        'B', 'M',
        sizeof(bh) + sizeof(dib_header) + area.size() * sizeof(pixel32),
    };
    bh.data_offset = sizeof(bmp_header) + sizeof(dib_header);

    dib_header dh = {
        sizeof(dib_header),
        x_max,
        y_max,
        1,
        32,
        0,
        area.size() * sizeof(pixel32),
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
    for (y = 0; y < y_max; ++y) {
        for (x = 0; x < x_max; ++x) {
            pixel32 pixel;

            if( area[y*w+x] == CONFLICTED_AREA )
                pixel = TO_PX32( 0, 255 );
            else if( point_is_entry( x+wx_min, y+wy_min, entries ) )
                pixel = TO_PX32( 255, 255 );
            else
            {
                auto color = (uint)color_palette[area[y*w+x]]; // 8 bit color
                pixel = TO_PX32( color, 255 );
            }
            fwrite( &pixel, sizeof(pixel32), 1, fp );
        }
    }
    fclose(fp);
    // printf("OK - file %s saved\n", img);
}

int main()
{
    int min_x = 99999, min_y = 99999;
    int max_x = -99999, max_y = -99999;

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
            const entry& last = entries[entries.size()-1];
            min_x = min( min_x, last.x );
            min_y = min( min_y, last.y );
            max_x = max( max_x, last.x );
            max_y = max( max_y, last.y );
        }
        generate_color_palette( entries.size() );

        vector<uint> size_per_area( entries.size() );       

        int w = max_x - min_x;
        int h = max_y - min_y;
        vector<uint> area( w*h );

        for( uint i=0; i<area.size(); ++i )
        {
            int x = (i % w) + min_x;
            int y = (i / w) + min_y;

            uint min_dist = UINT_MAX;
            uint min_dist_pt = 0;
            bool conflict_on_dist = false;

            for( uint k=0; k<entries.size(); ++k )
            {
                uint dist = abs( x - entries[k].x ) + abs( y - entries[k].y );
                if( dist < min_dist )
                {
                    min_dist = dist;
                    min_dist_pt = k;
                    conflict_on_dist = false;
                }
                else if( dist == min_dist )
                {
                    conflict_on_dist = true;
                }
            }

            area[i] = min_dist_pt;

            if( x == min_x || x == max_x || y == min_y || y == max_y )
                size_per_area[min_dist_pt] = INVALID_AREA;
            else if( conflict_on_dist )
                area[i] = CONFLICTED_AREA;
            else if( size_per_area[min_dist_pt] != INVALID_AREA )
                size_per_area[min_dist_pt]++;
        }

        output_area_to_image( entries, min_x, min_y, area, w, "img.bmp");

        for( int i=0; i<size_per_area.size(); ++i )
            printf("%d: %d\n", i, size_per_area[i]);

        uint max_area = 0;
        uint max_area_entry = 0;
        for( uint i=0; i<entries.size(); ++i )
        {
            if( size_per_area[i] > max_area && size_per_area[i] != INVALID_AREA )
            {
                max_area = size_per_area[i];
                max_area_entry = i;
            }
        }

        cout << max_area << endl;
    }
    
}