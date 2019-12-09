#include "types.h"

#include <cmath>

template<typename T>
T min( T a, T b ) { return a < b ? a : b; }
template<typename T>
T max( T a, T b ) { return a > b ? a : b; }
template<typename T>
bool inside( T t, T a, T b ) { return t >= a && t <= b; }

struct segment
{
    float x1, y1;
    float x2, y2;
};

struct point
{
    float x, y;
};
bool operator==( point p1, point p2 ) { return p1.x == p2.x && p1.y == p2.y; }
bool operator!=( point p1, point p2 ) { return !(p1 == p2); }

struct v2
{
    float x, y;
};
point operator+( point p, v2 v ) { return { p.x + v.x, p.y + v.y }; }
v2 operator*( v2 a, float b ) { return { a.x * b, a.y * b }; }

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