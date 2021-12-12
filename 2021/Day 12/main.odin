package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"
import "core:os"
import "core:intrinsics"

input :: string(#load( "input.txt" ))

Node :: struct {
    idx : int,
    name: string,
    is_big: bool,
}
Edge :: struct {
    start: int,
    end: int,
}

find_node :: proc( nodes: []Node, name: string ) -> int {
    for node, i in nodes {
        if node.name == name do return i
    }
    return -1
}

has_connection :: proc( m: []int, n_count, a, b: int ) -> bool {
    return m[a*n_count + b] > 0
}

add_node :: proc( nodes: ^[dynamic]Node, name: string ) -> int {
    nodes := nodes
    node := Node{}
    node.name = strings.clone( name )
    if node.name[0] >= 'A' && node.name[0] <= 'Z' do node.is_big = true
    n := len( nodes )
    node.idx = n
    append( nodes, node )
    return n
}

search_paths :: proc( nodes: []Node, connectivity: []int, current_path: [dynamic]int, current_node: int, visit_twice_version : bool = false, has_visited_twice : bool = false ) -> Maybe([dynamic][dynamic]int) {
    node := &nodes[current_node]
    has_visited_twice := has_visited_twice
    if visit_twice_version {
        if has_visited_twice || ( node.name == "start" || node.name == "end" ) {
            if !node.is_big && slice.contains( current_path[:], current_node ) {
                delete( current_path )
                return nil
            }
        } else {
            if !node.is_big && slice.contains( current_path[:], current_node ) {
                has_visited_twice = true
            }
        }
    } else {
        if !node.is_big && slice.contains( current_path[:], current_node ) {
            delete( current_path )
            return nil
        }
    }
    current_path := current_path
    append( &current_path, current_node )
    path_list : [dynamic][dynamic]int
    if node.name == "end" {
        append( &path_list, current_path )
        return path_list
    }

    for i in 0..<len(nodes) {
        if i == current_node do continue
        connected := (connectivity[current_node*len(nodes) + i] > 0)
        if connected {
            found_path := search_paths( nodes, connectivity, slice.to_dynamic( current_path[:] ), i, visit_twice_version, has_visited_twice )
            if found_path != nil {
                append( &path_list, ..found_path.?[:] )
            }
        }
    }
    return path_list
}

part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    lines := strings.split(input, "\r\n") ; defer delete( lines )
    nodes: [dynamic]Node
    edges: [dynamic]Edge
    for line in lines {
        splt := strings.split( line, "-" ) ; defer delete( splt )
        n1, n2 := find_node( nodes[:], splt[0] ), find_node( nodes[:], splt[1] )
        if n1 == -1 do n1 = add_node( &nodes, splt[0] )
        if n2 == -1 do n2 = add_node( &nodes, splt[1] )

        append( &edges, Edge{ n1, n2 } )
    }

    start_node, end_node: int
    for n in &nodes {
        if n.name == "start" do start_node = n.idx
        if n.name == "end" do end_node = n.idx
    }
    fmt.printf( "start idx: %d\n", start_node )
    fmt.printf( "end idx: %d\n", end_node )

    connectivity_matrix := make( []int, len(nodes)*len(nodes) )
    add_connection :: proc( m: []int, n_count, a, b: int ) {
        m[a*n_count + b] = 1
        m[b*n_count + a] = 1
    }
    
    for edge in &edges {
        add_connection( connectivity_matrix, len(nodes), edge.start, edge.end )
    }

    path : [dynamic]int
    paths := search_paths( nodes[:], connectivity_matrix, path, start_node )

    fmt.println( len( paths.? ) )

    fmt.println("==== Part 1 End ====")
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    lines := strings.split(input, "\r\n") ; defer delete( lines )
    nodes: [dynamic]Node
    edges: [dynamic]Edge
    for line in lines {
        splt := strings.split( line, "-" ) ; defer delete( splt )
        n1 := find_node( nodes[:], splt[0] )
        n2 := find_node( nodes[:], splt[1] )
        if n1 == -1 do n1 = add_node( &nodes, splt[0] )
        if n2 == -1 do n2 = add_node( &nodes, splt[1] )

        append( &edges, Edge{ n1, n2 } )
    }

    start_node, end_node: int
    for n in &nodes {
        if n.name == "start" do start_node = n.idx
        if n.name == "end" do end_node = n.idx
    }
    fmt.printf( "start idx: %d\n", start_node )
    fmt.printf( "end idx: %d\n", end_node )

    connectivity_matrix := make( []int, len(nodes)*len(nodes) )
    add_connection :: proc( m: []int, n_count, a, b: int ) {
        m[a*n_count + b] = 1
        m[b*n_count + a] = 1
    }
    
    for edge in &edges {
        add_connection( connectivity_matrix, len(nodes), edge.start, edge.end )
    }

    path : [dynamic]int
    paths := search_paths( nodes[:], connectivity_matrix, path, start_node, true, false )
    fmt.println( len( paths.? ) )

    fmt.println("==== Part 2 End ====")
}

main :: proc() {
    part1()
    part2()
}