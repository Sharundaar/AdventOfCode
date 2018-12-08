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
#include <functional>

using namespace std;

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char uchar;

struct entry
{
    char before, after;
};

#define ID_TO_IDX( id )  (id-'A')
#define IDX_TO_ID( idx ) (idx+'A')
#define WORKER_COUNT 5
#define TASK_ID_COUNT 26

struct task
{
    uint ttl = 0;
    char id = 0;
    bool dependencies[TASK_ID_COUNT] = { false };
    int start_time = -1;
    bool completed = false;
};

entry make_entry( const string& line )
{
    entry e = {};
    sscanf( line.c_str(), "Step %c must be finished before step %c can begin.", &e.before, &e.after );

    return e;
}

void print_entry( const entry& e )
{
    printf( "Step %c must be finished before step %c can begin.\n", e.before, e.after );
}

bool can_do_task( const task& t )
{
    for(int i=0; i<TASK_ID_COUNT; ++i)
        if( t.dependencies[i] ) return false;
    return true;
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

        vector<task> tasks(TASK_ID_COUNT);
        for( int i=0 ; i < entries.size() ; ++i )
        {
            auto& before_task = tasks[ ID_TO_IDX( entries[i].before ) ];
            auto& after_task = tasks[ ID_TO_IDX( entries[i].after ) ];
            
            before_task.id  = entries[i].before;
            before_task.ttl = 60 + ( before_task.id - 'A' + 1);

            after_task.id  = entries[i].after;
            after_task.ttl = 60 + ( after_task.id - 'A' + 1);

            after_task.dependencies[ID_TO_IDX( before_task.id )] = true;
        }

        uint worker_count = WORKER_COUNT;
        uint timer = 0;
        string tasks_done;

        auto step = [&]() {
            // find a task to queue
            for( int i=0; i<tasks.size(); ++i )
            {
                if( tasks[i].completed ) continue;
                if( tasks[i].start_time == -1 && can_do_task( tasks[i] ) && worker_count > 0 ) // task is queuable
                {
                    worker_count--;
                    tasks[i].start_time = timer;
                    return true;
                }
            }

            // We didn't find any task to queue, let's advance the timer to complete a task
            uint min_time_to_advance = UINT_MAX;
            task* task_to_complete = nullptr;
            for( int i=0; i<tasks.size(); ++i )
            {
                if( tasks[i].completed ) continue;
                if( tasks[i].start_time > -1 && tasks[i].start_time + tasks[i].ttl < min_time_to_advance ) // task is queuable
                {
                    min_time_to_advance = tasks[i].start_time + tasks[i].ttl;
                    task_to_complete = &tasks[i];
                }
            }

            if( min_time_to_advance < UINT_MAX && task_to_complete != nullptr )
            {
                task_to_complete->completed = true;
                worker_count++;
                tasks_done += task_to_complete->id;
                timer = min_time_to_advance;
                for( int j=0 ; j<tasks.size(); ++j )
                {
                    tasks[j].dependencies[ID_TO_IDX(task_to_complete->id)] = false;
                }
                return true;
            }

            // nothing worked so we've probably done everything or we're stuck
            return false;
        };
        while( step() );

        cout << tasks_done << endl;
        cout << timer << endl;
    }
}