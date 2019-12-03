#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char uchar;

#define INVALID_AREA 0x10000000
#define CONFLICTED_AREA 0x10000001

#define TO_PX32( COL_8, alpha ) pixel32{ (uchar)(((COL_8 & 0b11100000) >> 5) * 36), (uchar)(((COL_8 & 0b00011100) >> 2) * 36), (uchar)((COL_8 & 0b00000011) * 36), alpha }
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

static vector<char> color_palette;
void generate_color_palette( int count )
{
    srand( 67688742 );
    for( int i=0; i<count; ++i )
        color_palette.push_back( rand() % 256 );
}


template<typename T>
T max( T a, T b ) { return a >= b ? a : b; }
template<typename T>
T min( T a, T b ) { return a <= b ? a : b; }
template<typename T>
T inside( T t, T a, T b ) { return t >= a && t <= b; }

struct segment
{
    float x1, y1;
    float x2, y2;
};

struct point
{
    float x, y;
};

struct v2
{
    float x, y;
};

v2 sub( v2 a, v2 b )
{
    return { b.x - a.x, b.y - a.y };
}

float len( v2 v )
{
    return sqrt( v.x * v.x + v.y * v.y );
}

v2 norm( v2 v )
{
    float l = len( v );
    return { v.x / l, v.y / l };
}

v2 seg2vec( segment s )
{
    return { s.x2 - s.x1, s.y2 - s.y1 };
}

point p1( segment s )
{
    return { s.x1, s.y1 };
}

point p2( segment s )
{
    return { s.x2, s.y2 };
}

bool on_segment( segment s, point p )
{
    if (p.x <= max(s.x1, s.x2) && p.x >= min(s.x1, s.x2) && 
        p.y <= max(s.y1, s.y2) && p.y >= min(s.y1, s.y2)) 
       return true; 
  
    return false; 
}

bool intersect( segment s1, segment s2, float& t, float& u )
{
    float x1 = s1.x1;
    float x2 = s1.x2;
    float x3 = s2.x1;
    float x4 = s2.x2;
    float y1 = s1.y1;
    float y2 = s1.y2;
    float y3 = s2.y1;
    float y4 = s2.y2;
    
    float denum = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    float numt  = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4);
    float numu  = (x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3);

    if( abs( denum ) <= FLT_EPSILON * 4 )
    {
        t = FLT_MAX;
        u = FLT_MAX;
        return on_segment( s1, p1( s2 ) )
                || on_segment( s1, p2( s2 ) )
                || on_segment( s2, p1( s1 ) )
                || on_segment( s2, p2( s1 ) );
    }

    t = numt / denum;
    u = -numu / denum;
    return inside( t, 0.f, 1.f ) && inside( u, 0.f, 1.f );
}

float manhattan( point p )
{
    return abs( p.x ) + abs( p.y );
}

bool get_intersection_point( segment s1, segment s2, point& out )
{
    out = {};
    float t, u;
    if( intersect( s1, s2, t, u ) )
    {
        out = { s1.x1 + t * (s1.x2 - s1.x1), s1.y1 + t * (s1.y2 - s1.y1) };
        return true;
    }
    return false;
}

void output_area_to_image( const vector<segment>& segments, point min, point max, const char* img )
{
    auto from_seg_coord_to_img = []( segment seg, int img_w, int img_h ) {
        point pos_0 = { img_w / 2.0f, img_h / 2.0f };
        
    };

    int x, y;
    /* size of the image */
    const int x_max = max.x - min.x;  /* width */
    const int y_max = max.y - min.y;  /* height */
    FILE * fp;

    bmp_header bh = {
        'B', 'M',
        sizeof(bh) + sizeof(dib_header) + x_max * y_max * sizeof(pixel32),
    };
    bh.data_offset = sizeof(bmp_header) + sizeof(dib_header);

    dib_header dh = {
        sizeof(dib_header),
        x_max,
        y_max,
        1,
        32,
        0,
        x_max * y_max * sizeof(pixel32),
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

            
            fwrite( &pixel, sizeof(pixel32), 1, fp );
        }
    }
    fclose(fp);
    // printf("OK - file %s saved\n", img);
}

int main()
{
    ifstream file( "input.txt" );
    if( file )
    {
        string line;
        vector<segment> segments;

        while( getline( file, line ) )
        {
            // split ,
            string instruction;
            point p = {};
            auto stream = stringstream( line );
            while( getline( stream, instruction, ',') )
            {
                char dir = instruction[0];
                int len = atoi( instruction.c_str()+1 );
                point nextP = p;
                switch( dir )
                {
                    case 'R':
                        nextP.x += len;
                        break;
                    case 'L':
                        nextP.x -= len;
                        break;
                    case 'U':
                        nextP.y += len;
                        break;
                    case 'D':
                        nextP.y -= len;
                        break;

                }
                segments.push_back( { p.x, p.y, nextP.x, nextP.y } );
                p = nextP;
            }
        }

        vector<point> intersections;
        for( int i=0; i<segments.size(); ++i )
        {
            for( int j=i+1; j<segments.size(); ++j )
            {
                if( i == j )
                    continue;
                point intersection;
                if( get_intersection_point( segments[i], segments[j], intersection ) )
                {
                    if( segments[i].x1 == 0 && segments[i].y1 == 0
                        && segments[j].x1 == 0 && segments[j].y1 == 0 ) // if both segments starts at 0, ignore
                        continue;
                    intersections.push_back( intersection );
                }
            }
        }

        point min_man_p;
        float min_man;
        for( point& p: intersections )
        {
            float man = manhattan( p );
            if( man < min_man )
            {
                min_man_p = p;
                min_man = man;
            }
        }

        cout << min_man << endl;
    }

    return 0;
}