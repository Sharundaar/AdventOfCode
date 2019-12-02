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

struct entry {};
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
    fclose(fp);
    // printf("OK - file %s saved\n", img);
}

#define GRID_W 300
#define GRID_H 300

int cell_grid[GRID_W*GRID_H];

int main()
{
    ifstream file( "input.txt" );
    if( !file ) return 0;

    string line;
    getline( file, line );
    uint grid_id;
    sscanf( line.c_str(), "%d", &grid_id );
    for( int y=0; y<GRID_H; ++y )
    {
        for( int x=0; x<GRID_W; ++x )
        {
            int power_level = (((x+10)*y)+grid_id)*(x+10);
            int power_level_abs = abs(power_level);
            int hundreds_digit = power_level_abs/100 - ((power_level_abs/1000)*10);
            
            cell_grid[x+y*GRID_W] = hundreds_digit - 5;
        }
    }
}