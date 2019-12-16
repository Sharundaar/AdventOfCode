#pragma once

#include "types.h"

#include <vector>

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
bool operator==( pixel32 a, pixel32 b ) { return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a; }
#pragma pack(pop)

void output_pixels_to_bmp( const std::vector<pixel32> pixels, uint width, const char* img )
{
    /* size of the image */
    const uint x_max = width;  /* width */
    const uint y_max = pixels.size() / width;  /* height */
    const uint pixelCount = x_max * y_max;
    FILE * fp;

    bmp_header bh = {
        'B', 'M',
        sizeof(bh) + sizeof(dib_header) + pixelCount * sizeof(pixel32),
    };
    bh.data_offset = sizeof(bmp_header) + sizeof(dib_header);

    dib_header dh = {
        sizeof(dib_header),
        (int)x_max,
        (int)y_max,
        1,
        32,
        0,
        (uint) ( pixelCount * sizeof(pixel32) ),
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
    fwrite( pixels.data(), sizeof(pixel32) * pixelCount, 1, fp );

    fclose(fp);
}
