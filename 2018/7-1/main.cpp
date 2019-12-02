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

struct graph_connection;

struct graph_node
{
    char id = 0;
    graph_connection* first_connection;
};

struct graph_connection
{
    graph_node* begin = nullptr;
    graph_node* end = nullptr;
    graph_connection* next_connection = nullptr;
};
graph_connection EMPTY_CONNECTION = {};

struct dependency
{
    char id;
    int count;
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

        graph_node node_pool[0x5000];
        graph_connection connection_pool[0x5000];
        uint allocated_node = 0;
        uint allocated_connection = 0;
        auto allocate_node = [&]( char id ) {
            auto node = &node_pool[allocated_node++];
            *node = { id, &EMPTY_CONNECTION };
            printf("Created node %c. (allocated: %d)\n", id, allocated_node);
            return node;
        };
        auto allocate_connection = [&]( graph_node* begin, graph_node* end ) {
            auto connection = &connection_pool[allocated_connection++];
            connection->begin = begin; connection->end = end;
            connection->next_connection = begin->first_connection;
            begin->first_connection = connection;

            connection = &connection_pool[allocated_connection++];
            connection->begin = begin; connection->end = end;
            connection->next_connection = end->first_connection;
            end->first_connection = connection;
        };
        auto find_node = [&]( char id ) -> graph_node* {
            for( uint idx = 0; idx<allocated_node; ++idx )
                if( node_pool[idx].id == id ) return &node_pool[idx];
            return nullptr;
        };

        for( uint i=0; i<entries.size(); ++i )
        {
            auto before_node = find_node( entries[i].before );
            auto after_node  = find_node( entries[i].after );
            if( !before_node ) before_node = allocate_node( entries[i].before );
            if( !after_node )  after_node  = allocate_node( entries[i].after );

            allocate_connection( before_node, after_node );
            printf( "[%d] Linked %c -> %c.\n", i, before_node->id, after_node->id );
        }
        printf( "Graph creation finished.\n" );

        string chain;
        vector<int> connection_matrix( allocated_node * allocated_node );
        for( uint i=0; i<allocated_connection; ++i )
        {
            auto connection = connection_pool[i];
            uint x = connection.begin->id - 'A';
            uint y = connection.end->id - 'A';

            connection_matrix[y+x*allocated_node] =  1;
            connection_matrix[x+y*allocated_node] = -1;
        }

        printf( "   " );
        for( int i=0; i<allocated_node; ++i )
            printf( "%c ", 'A'+i );
        printf( "\n" );

        for( int i=0; i<allocated_node; ++i )
        {
            printf( "%c ", 'A'+i );
            for( int j=0; j<allocated_node; ++j )
            {
                int idx = i+j*allocated_node;
                if(connection_matrix[idx] >= 0 )
                    printf(" %d", connection_matrix[idx]);
                else
                    printf("-1");
            }
            printf( "\n" );
        }

        vector<int> next_matrix( connection_matrix.size() );
        memcpy( next_matrix.data(), connection_matrix.data(), next_matrix.size()*sizeof(next_matrix[0]) );

        auto step = [&]() {
            for( int i=0; i<allocated_node; ++i )
            {
                bool all_nil = true;
                bool is_root = true;
                for( int j=0; j<allocated_node; ++j )
                {
                    if( connection_matrix[j+i*allocated_node] != 0 )
                        all_nil = false;
                    if( connection_matrix[j+i*allocated_node] < 0 )
                        is_root = false;
                }

                if( all_nil ) continue;
                if( is_root )
                {
                    chain += 'A'+i;
                    for( int j=0; j<allocated_node; ++j )
                    {
                        next_matrix[i+j*allocated_node] = 0;
                        next_matrix[j+i*allocated_node] = 0;
                    }
                    memcpy( connection_matrix.data(), next_matrix.data(), next_matrix.size()*sizeof(next_matrix[0]) );
                    return true;
                }
            }

            return false;
        };
        while(step());

        printf( "   " );
        for( int i=0; i<allocated_node; ++i )
            printf( "%c ", 'A'+i );
        printf( "\n" );

        for( int i=0; i<allocated_node; ++i )
        {
            printf( "%c ", 'A'+i );
            for( int j=0; j<allocated_node; ++j )
            {
                int idx = i+j*allocated_node;
                if(connection_matrix[idx] >= 0 )
                    printf(" %d", connection_matrix[idx]);
                else
                    printf("-1");
            }
            printf( "\n" );
        }

        cout << chain << endl;
    }
}