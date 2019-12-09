#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <climits>

#include "../helpers/image.hpp"
#include "../helpers/math.hpp"

using namespace std;

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

        point min = { FLT_MAX, FLT_MAX };
        point max = { FLT_MIN, FLT_MIN };
        for( auto& seg : segments )
        {
            min.x = ::min( seg.x1, ::min( seg.x2, min.x ) );
            min.y = ::min( seg.y1, ::min( seg.y2, min.y ) );
            max.x = ::max( seg.x1, ::max( seg.x2, max.x ) );
            max.y = ::max( seg.y1, ::max( seg.y2, max.y ) );
            
        }
        // output_area_to_image( segments, min, max, "img.bmp" );

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

                    if( intersection.x == segments[i].x1 && intersection.y == segments[i].y1 )
                        continue;
                    if( intersection.x == segments[j].x1 && intersection.y == segments[j].y1 )
                        continue;
                    intersections.push_back( intersection );
                }
            }
        }

        point min_man_p;
        float min_man = FLT_MAX;
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