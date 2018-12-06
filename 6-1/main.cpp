#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <ctime>
#include <cstdlib>

using namespace std;

typedef unsigned int uint;
typedef unsigned long ulong;

#define INVALID_AREA 0x1000000
#define CONFLICTED_AREA 0x1000001

struct entry
{
    int x, y;
};

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
    srand( 676838742 );
    for( int i=0; i<count; ++i )
        color_palette.push_back( rand() % 256 );
}

void output_area_to_image( const vector<uint>& area, uint w, const char* img )
{
    int x, y;
    /* size of the image */
    const int x_max = w;  /* width */
    const int y_max = area.size() / w;  /* height */
    /* 2D array for colors (shades of gray) */
    vector<unsigned char> data( area.size() );
    /* color component is coded from 0 to 255 ;  it is 8 bit color file */
    const int MaxColorComponentValue = 255;
    FILE * fp;
    /* comment should start with # */
    const char *comment = "# this is my new binary pgm file";

    /* fill the data array */
    for (y = 0; y < y_max; ++y) {
        for (x = 0; x < x_max; ++x) {
            if( area[y*w+x] == CONFLICTED_AREA )
                data[y*w+x] = 0;
            else
                data[y*w+x] = color_palette[area[y*w+x]];
        }
    }

    /* write the whole data array to ppm file in one step */
    /* create new file, give it a name and open it in binary mode */
    fp = fopen(img, "wb");
    /* write header to the file */
    fprintf(fp, "P5\n %s\n %d\n %d\n %d\n", comment, x_max, y_max,
            MaxColorComponentValue);
    /* write image data bytes to the file */
    fwrite(data.data(), data.size() * sizeof(data[0]), 1, fp);
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

            uint min_dist = 9999;
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
                size_per_area[min_dist_pt] = CONFLICTED_AREA;
            else
                size_per_area[min_dist_pt]++;
        }

        // ( area, w );
        output_area_to_image( area, w, "img.pgm");

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