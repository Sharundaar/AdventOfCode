#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

typedef unsigned int uint;
typedef unsigned long ulong;

struct date
{
    int yy = 0;
    int mm = 0;
    int dd = 0;

    int h = 0;
    int m = 0;
};

enum class entry_type
{
    GuardBegin,
    Asleep,
    WakeUp,
};

struct entry
{
    entry_type type;
    uint guard_id = 0; // depends on entry_type
    date d = {};
};

struct guard_info
{
    uint guard_id = 0;
    uint time_asleep = 0;
    uint asleep_per_minutes[60] = {0};
};

entry make_entry( const string& line )
{
    entry e = {};
    sscanf( line.c_str(), "[%d-%d-%d %d:%d]", &e.d.yy, &e.d.mm, &e.d.dd, &e.d.h, &e.d.m );

    const char* buffer = line.c_str() + 19;
    constexpr char FallAsleepToken[] = "falls asleep";
    constexpr char WakesUpToken[] = "wakes up";

    if( !strcmp( buffer, FallAsleepToken ) )
        e.type = entry_type::Asleep;
    else if( !strcmp( buffer, WakesUpToken ) )
        e.type = entry_type::WakeUp;
    else
    {
        e.type = entry_type::GuardBegin;
        sscanf( buffer, "Guard #%d begins shift", &e.guard_id );
    }

    return e;
}

bool cmpdate( const date& d1, const date& d2 )
{
    if( d1.yy < d2.yy ) return true;
    if( d1.yy > d2.yy ) return false;
    if( d1.mm < d2.mm ) return true;
    if( d1.mm > d2.mm ) return false;
    if( d1.dd < d2.dd ) return true;
    if( d1.dd > d2.dd ) return false;
    if( d1.h  < d2.h )  return true;
    if( d1.h  > d2.h )  return false;
    if( d1.m  < d2.m )  return true;
    if( d1.m  > d2.m )  return false;
}

bool cmpentries( const entry& e1, const entry& e2 )
{
    return cmpdate( e1.d, e2.d );
}

void print_entry( const entry& e )
{
    printf( "[%d-%02d-%02d %02d:%02d] ", e.d.yy, e.d.mm, e.d.dd, e.d.h, e.d.m );
    switch(e.type)
    {
        case entry_type::GuardBegin:
            printf( "Guard #%d begins shift", e.guard_id );
            break;
        case entry_type::Asleep:
            printf( "falls asleep" );
            break;
        case entry_type::WakeUp:
            printf( "wakes up" );
            break;
    }
    printf("\n");
}

bool sort_by_asleep_time( const guard_info& g1, const guard_info& g2 )
{
    return g1.time_asleep > g2.time_asleep;
}

int main()
{
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
        }

        sort( entries.begin(), entries.end(), cmpentries );

        unordered_map<uint, guard_info> guard_infos;
        // guard_infos.resize( 2000 );
        int current_guard = -1;
        uint asleep_total = 0;
        date asleep_start = {0};
        for( int i=0; i<entries.size(); ++i )
        {
            // print_entry( entries[i] );
            if( entries[i].type == entry_type::GuardBegin )
            {
                if( current_guard != -1 )
                    guard_infos[current_guard].time_asleep += asleep_total;
                current_guard = entries[i].guard_id;
                asleep_start.yy = 0;
                asleep_total = 0;
            }
            else if( entries[i].type == entry_type::Asleep )
            {
                asleep_start = entries[i].d;
            }
            else if( entries[i].type == entry_type::WakeUp )
            {
                if( current_guard != -1)
                {
                    for( int s = asleep_start.m; s < entries[i].d.m; ++s )
                        guard_infos[current_guard].asleep_per_minutes[s]++;
                }
                
                asleep_total += entries[i].d.m - asleep_start.m;
            }
        }

        // sort( guard_infos.begin(), guard_infos.end(), sort_by_asleep_time );
        uint max_guard_id = 0;
        uint max_minute_asleep = 0;
        uint max_minute_asleep_count = 0;
        for( auto& pair : guard_infos )
        {
            for( int s = 0; s<60; ++s )
            {
                if( pair.second.asleep_per_minutes[s] > max_minute_asleep_count )
                {
                    max_guard_id = pair.first;
                    max_minute_asleep = s;
                    max_minute_asleep_count = pair.second.asleep_per_minutes[s];
                }
            }
        }

        printf("\n%d %d %d\n", max_minute_asleep, max_minute_asleep_count, max_guard_id);
        cout << max_minute_asleep * max_guard_id << endl;
    }
    
}