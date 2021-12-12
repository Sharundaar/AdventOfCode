package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"
import "core:os"
import "core:intrinsics"

input :: string(#load( "input.txt" ))

contains_slice :: proc( container, containee: $T/[]$E ) -> bool where intrinsics.type_is_comparable(E) {
	n := len( containee )
	if n > len( container ) {
		return false
	}
	for i in 0..<n {
		if !find( container, containee[i] ) {
			return false
		}
	}
	return true
}

find :: proc{
    slice.contains,
    contains_slice,
}

count_node :: proc( container: []int, idx: int ) -> int {
    count: int
    for k in container {
        if k == idx do count += 1
    }

    return count
}

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

search_paths :: proc( nodes: []Node, connectivity: []int, current_path: [dynamic]int, current_node: int ) -> Maybe([dynamic][dynamic]int) {
    node := &nodes[current_node]
    if !node.is_big && find( current_path[:], current_node ) {
        delete( current_path )
        return nil
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
            found_path := search_paths( nodes, connectivity, slice.to_dynamic( current_path[:] ), i )
            if found_path != nil {
                for path in found_path.? {
                    append( &path_list, path )
                }
            }
        }
    }
    return path_list
}

part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    lines := strings.split(input, "\r\n")
    nodes: [dynamic]Node
    edges: [dynamic]Edge
    for line in lines {
        splt := strings.split( line, "-" ) ; defer delete( splt )
        n1 := find_node( nodes[:], splt[0] )
        n2 := find_node( nodes[:], splt[1] )
        if n1 == -1 {
            node := Node{}
            node.name = strings.clone( splt[0] )
            if node.name[0] >= 'A' && node.name[0] <= 'Z' do node.is_big = true
            n1 = len( nodes )
            node.idx = n1
            append( &nodes, node )
        }
        if n2 == -1 {
            node := Node{}
            node.name = strings.clone( splt[1] )
            if node.name[0] >= 'A' && node.name[0] <= 'Z' do node.is_big = true
            n2 = len( nodes )
            node.idx = n2
            append( &nodes, node )
        }

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

search_paths_2 :: proc( nodes: []Node, connectivity: []int, current_path: [dynamic]int, current_node: int, has_visited_twice: bool ) -> Maybe([dynamic][dynamic]int) {
    node := &nodes[current_node]
    has_visited_twice := has_visited_twice
    if has_visited_twice || ( node.name == "start" || node.name == "end" ) {
        if !node.is_big && find( current_path[:], current_node ) {
            delete( current_path )
            return nil
        }
    } else {
        if !node.is_big && find( current_path[:], current_node ) {
            has_visited_twice = true
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
            found_path := search_paths_2( nodes, connectivity, slice.to_dynamic( current_path[:] ), i, has_visited_twice )
            if found_path != nil {
                for path in found_path.? {
                    append( &path_list, path )
                }
            }
        }
    }
    return path_list
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    lines := strings.split(input, "\r\n")
    nodes: [dynamic]Node
    edges: [dynamic]Edge
    for line in lines {
        splt := strings.split( line, "-" ) ; defer delete( splt )
        n1 := find_node( nodes[:], splt[0] )
        n2 := find_node( nodes[:], splt[1] )
        if n1 == -1 {
            node := Node{}
            node.name = strings.clone( splt[0] )
            if node.name[0] >= 'A' && node.name[0] <= 'Z' do node.is_big = true
            n1 = len( nodes )
            node.idx = n1
            append( &nodes, node )
        }
        if n2 == -1 {
            node := Node{}
            node.name = strings.clone( splt[1] )
            if node.name[0] >= 'A' && node.name[0] <= 'Z' do node.is_big = true
            n2 = len( nodes )
            node.idx = n2
            append( &nodes, node )
        }

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
    paths := search_paths_2( nodes[:], connectivity_matrix, path, start_node, false )
    fmt.println( len( paths.? ) )

    fmt.println("==== Part 2 End ====")
}

main :: proc() {
    part1()
    part2()
}