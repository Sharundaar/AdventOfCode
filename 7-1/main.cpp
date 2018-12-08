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
            *node = { id, EMPTY_CONNECTION };
            printf("Created node %c. (allocated: %d)\n", id, allocated_node);
            return node;
        };
        auto allocate_connection = [&]( graph_node* begin, graph_node* end ) {
            auto connection = &connection_pool[allocated_connection++];
            connection->next_connection = begin->first_connection;
            begin->first_connection = connection;
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
            if( !before_node->first_child )
            {
                auto first_child = after_node;
                while( first_child->prev_sibling ) first_child = first_child->prev_sibling;
                before_node->first_child = first_child;
                after_node->parent = before_node;
                printf( "[%d] Linked %c -> %c.\n", i, before_node->id, after_node->id );
            }
            else
            {
                auto last_child = before_node->first_child;
                int loop_prevention = 0;
                while(last_child->next_sibling)
                {
                    last_child = last_child->next_sibling;
                    loop_prevention++;
                    if( loop_prevention > 100 )
                    {
                        printf("Infinite loop detected!");
                        exit(0);
                    }
                } 
                last_child->next_sibling = after_node;
                after_node->prev_sibling = last_child;
                after_node->parent = before_node;
                printf( "[%d] Linked %c -> %c.\n", i, before_node->id, after_node->id );
            }
        }
        printf( "Graph creation finished.\n" );

        string chain;

        // find elements which has no parents
        graph_node* root = nullptr;
        for( int i=0; i<allocated_node; ++i )
        {
            auto node = &node_pool[i];
            if( !node->parent )
            {
                if( root ) printf("Multiple possible roots.\n");
                root = node;
            }
        }

        cout << chain << endl;
    }
}