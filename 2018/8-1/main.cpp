#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <functional>

using namespace std;

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char uchar;

#define SKIP_SPACE( it ) {while(**it == ' ') ++(*it);}

struct entry
{

};

entry make_entry( const string& line )
{
    entry e;

    return e;
}

void print_entry( const entry& e )
{

}

struct node
{
    vector<node*> childs;
    vector<int> metadata;
};

template<typename T, int S>
struct pool
{
    T pool[S];
    uint allocated;

    T* allocate() { return &pool[allocated++]; }
};
using NodePool = pool<node, 2000>;

node* allocate_node( NodePool& pool ) 
{
        auto n = pool.allocate();
        *n = {};
        // printf( "Allocated node %d.\n", pool.allocated );
        return n;
}

node* parse_node( NodePool& pool, const char** it )
{
    int child_count = 0;
    int metadata_count = 0;

    child_count = atoi( *it );
    while( **it != ' ' ) ++(*it);
    ++(*it);
    metadata_count = atoi( *it );
    while( **it != ' ' ) ++(*it);
    ++(*it);

    auto n = allocate_node(pool);

    for( int i=0; i<child_count; ++i )
    {
        n->childs.push_back( parse_node( pool, it ) );
    }
    for( int i=0; i<metadata_count; ++i )
    {
        int metadata;
        metadata = atoi( *it );
        while( **it != ' ' && **it != '\0' ) ++(*it);
        if( **it != '\0' ) ++(*it);
        n->metadata.push_back( metadata );
    }

    return n;
}

void print_node( const node* node )
{
    printf( "%d %d ", node->childs.size(), node->metadata.size() );
    for( int i=0; i<node->childs.size(); ++i )
        print_node( node->childs[i] );
    for( int i=0; i<node->metadata.size(); ++i )
        printf( "%d ", node->metadata[i] );
}

int sum_metadata( const node* node )
{
    int metadata_sum = 0;
    for( int i=0; i<node->childs.size(); ++i )
        metadata_sum += sum_metadata( node->childs[i] );
    for( int i=0; i<node->metadata.size(); ++i )
        metadata_sum += node->metadata[i];
    return metadata_sum;
}

NodePool node_pool;

int main()
{
    ifstream file( "input.txt" );
    if( !file ) return 0;

    string line;
    getline( file, line );

    const char* it = line.c_str();
    auto* root = parse_node( node_pool, &it );
    // check that we reached the end of the file
    if( *it != '\0' )
    {
        printf( "Didn't reach end of file." );
        return 0;
    }

    // print_node( root );
    cout << sum_metadata( root ) << endl;
}