#pragma once

template<typename T>
struct graph_node
{
    T data = {};

    graph_node* parent = nullptr;
    graph_node* first_child = nullptr;
    graph_node* next_sibling = nullptr;

    graph_node* last_child()
    {
        graph_node* node = first_child;
        while( node )
            if( !node->next_sibling ) return node;
        return node;
    }

    template<typename F>
    graph_node* find_child( F finder )
    {
        graph_node* child = first_child;
        while( child )
        {
            if( finder( child->data ) ) return child;
            auto found = child->find_child( finder );
            if( found ) return found;
            child = child->next_sibling;
        }

        return nullptr;
    }

    bool has_children() { return first_child; }
    bool has_parent() { return parent; }
};

template<typename T, int S>
struct graph_pool
{
    using graph_node_t = graph_node<T>;
    graph_node_t* pool;
    int high = 0;

    graph_pool<T, S>() { pool = new graph_node_t[S]; }
    ~graph_pool<T, S>() { delete[] pool; }
    graph_node_t* allocate() { return (pool+high++); }
};

template<typename T, int S>
struct graph
{
    using graph_node_t = graph_node<T>;

    graph_pool<T, S> pool;
    graph_node_t* root;
    bool lock = false;

    graph_node_t* add_node( graph_node_t* parent, T data )
    {
        auto node = pool.allocate();
        node->data = data;
        node->parent = parent;

        if( !parent )
        {
            root = node;
        }
        else if( parent->has_children() )
        {
            auto last_child = parent->last_child();
            last_child->next_sibling = node;
        }
        else
        {
            parent->first_child = node;
        }

        return node;
    }

    template<typename F>
    void iterate_all_nodes( F iterator )
    {
        lock = true;
        int node_idx = 0;
        while( node_idx < pool.high )
            iterator( pool.pool[node_idx++] );
        lock = false;
    }

    template<typename F>
    void iterate_all_nodes_with_stops( F iterator )
    {
        lock = true;
        int node_idx = 0;
        while( node_idx < pool.high )
            if( iterator( pool.pool[node_idx++] ) ) break;
        lock = false;
    }
};
