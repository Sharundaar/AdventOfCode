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
    char searched_state[5];
    char output;
};

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

#define STATE_SIZE 300

entry make_entry( const string& line )
{
    entry e;

    e.searched_state[0] = line[0];
    e.searched_state[1] = line[1];
    e.searched_state[2] = line[2];
    e.searched_state[3] = line[3];
    e.searched_state[4] = line[4];
    // line[5]; =
    // line[6]; >
    e.output = line[7];

    return e;
}

void print_entry( const entry& e )
{
    printf("%c%c%c%c%c => %c\n", e.searched_state[0], e.searched_state[1], e.searched_state[2], e.searched_state[3], e.searched_state[4], e.output);
}

int main()
{
    ifstream file( "input.txt" );
    if( !file ) return 0;

    string line;
    
    getline( file, line );
    const char* initial_state = line.c_str();
    int initial_state_size;
    while( !(*initial_state == '#' || *initial_state == '.') ) ++initial_state;
    initial_state_size = line.size() - (initial_state - line.c_str());
    printf("%d\n", initial_state_size);

    char state[STATE_SIZE];
    int zero = STATE_SIZE / 2;
    for( int i=0; i<STATE_SIZE; ++i )
    {
        if( i < zero || i >= zero+initial_state_size )
            state[i] = '.';
        else
            state[i] = initial_state[i-zero];
    }
    initial_state = nullptr;

    for( int i=0; i<STATE_SIZE; ++i )
        printf("%c", state[i]);
    printf("\n");

    vector<entry> entries;
    while( getline( file, line ) )
    {
        if( line[0] == '\n' ) continue;
        entries.emplace_back( make_entry( line ) );
    }

    for(auto& e : entries)
        print_entry(e);
}