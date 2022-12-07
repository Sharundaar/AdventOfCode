package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"
import "core:os"
import "core:intrinsics"

input :: string(#load( "input.txt" ))

SnailNumberId :: distinct int
Element :: union #no_nil {SnailNumberId, int}
SnailNumber :: struct {
    id: SnailNumberId,
    left, right: Element,
}

parse_number :: proc( str: []u8, it: int, pool: ^[dynamic]SnailNumber ) -> (SnailNumberId, int) {
    str := str
    it := it
    assert( str[it] == '[' )

    id := SnailNumberId( len( pool ) )
    append( pool, SnailNumber{} )
    pool[int(id)].id = id

    it += 1

    c := str[it]
    switch c {
        case '[':
            a: SnailNumberId
            a, it = parse_number( str, it, pool )
            pool[int(id)].left = Element( a )
        case '0'..'9':
            a := int(c) - int('0')
            it += 1
            pool[int(id)].left = Element( a )
    }

    assert( str[it] == ',' )
    it += 1

    c = str[it]
    switch str[it] {
        case '[':
            b: SnailNumberId
            b, it = parse_number( str, it, pool )
            pool[int(id)].right = Element( b )
        case '0'..'9':
            b := int(c) - int('0')
            it += 1
            pool[int(id)].right = Element( b )
    }

    assert( str[it] == ']' )
    it += 1

    return id, it
}

print_number :: proc( n: SnailNumberId, pool: []SnailNumber ) {
    no := pool[n]
    fmt.printf( "[" )
    switch left in no.left {
        case int: fmt.printf( "%d", left )
        case SnailNumberId: print_number( left, pool )
    }
    fmt.printf( "," )
    switch right in no.right {
        case int: fmt.printf( "%d", right )
        case SnailNumberId: print_number( right, pool )
    }
    fmt.printf( "]" )
}

add_numbers :: proc( left, right: SnailNumberId, pool: ^[dynamic]SnailNumber ) -> SnailNumberId {
    id := SnailNumberId( len( pool ) )
    append( pool, SnailNumber{} )
    pool[int(id)].id = id
    pool[int(id)].left = Element( left )
    pool[int(id)].right = Element( right )
    return id
}

INVALID_ID :: -1

find_right_most :: proc( n: SnailNumberId, pool: []SnailNumber ) -> (SnailNumberId, Element) {
    no := pool[n]
    if _, ok := no.right.(int); ok {
        return n, no.right
    }
    return find_right_most( no.right.(SnailNumberId), pool )
}

find_left_most :: proc( n: SnailNumberId, pool: []SnailNumber ) -> (SnailNumberId, Element) {
    no := pool[n]
    if _, ok := no.left.(int); ok {
        return n, no.left
    }
    return find_left_most( no.left.(SnailNumberId), pool )
}

find_closest_left :: proc( a: Element, parent_chain: []SnailNumberId, pool: []SnailNumber ) -> (SnailNumberId, Element) {
    // [1, [[2, 3], 4]]
    
    k := len(parent_chain)-1
    if k >= 0 {
        parent := pool[parent_chain[k]]
        if parent.left == a {
            return find_closest_left( parent.id, parent_chain[0:k], pool )
        }
        if _, ok := parent.left.(int); ok {
            return parent.id, parent.left
        }
        if id, ok := parent.left.(SnailNumberId); ok {
            return find_right_most( parent.left.(SnailNumberId), pool )
        }
    }

    return INVALID_ID, Element( int(INVALID_ID) )
}

find_closest_right :: proc( a: Element, parent_chain: []SnailNumberId, pool: []SnailNumber ) -> (SnailNumberId, Element) {
    // [1, [[2, 3], 4]]
    a := a
    parent_chain := parent_chain
    k := len(parent_chain)-1
    if k >= 0 {
        parent := pool[parent_chain[k]]
        if parent.right == a {
            return find_closest_right( parent.id, parent_chain[0:k], pool )
        }
        if _, ok := parent.right.(int); ok {
            return parent.id, parent.right
        }
        if id, ok := parent.right.(SnailNumberId); ok {
            return find_left_most( parent.right.(SnailNumberId), pool )
        }
    }

    return INVALID_ID, Element( int(INVALID_ID) )
}

reduce :: proc( a: SnailNumberId, pool: []SnailNumber ) -> bool {
    stack: [dynamic]SnailNumberId ; defer delete( stack )
    treated: map [SnailNumberId] bool
    append( &stack, a )

    // check for explosion
    for {
        top_most := stack[len(stack)-1]
        top_most_number := pool[top_most]
        switch a in top_most_number.left {
            case int: 
            case SnailNumberId:
                if a not_in treated do append( &stack, a )
                continue
        }

        switch b in top_most_number.right {
            case SnailNumberId:
                if b not_in treated do append( &stack, b )
                continue
            case int:
                // a and b are ints and we're at least 4 deep in the stack, we explode
                if len( stack ) > 4 {
                    cleft, cleft_elem := find_closest_left( top_most_number.left, stack[:], pool )
                    cright, cright_elem := find_closest_right( top_most_number.right, stack[:], pool )
                    if cleft != INVALID_ID {
                        if cleft_elem == pool[cleft].left {
                            pool[cleft].left = pool[cleft].left.(int) + top_most_number.left.(int)
                        } else {
                            assert( cleft_elem == pool[cleft].right )
                            pool[cleft].right = pool[cleft].right.(int) + top_most_number.left.(int)
                        }
                    }

                    if cright != INVALID_ID {
                        if cright_elem == pool[cright].left {
                            pool[cright].left = cright_elem.(int) + top_most_number.right.(int)
                        } else {
                            assert( cright_elem == pool[cright].right )
                            pool[cright].right = cright_elem.(int) + top_most_number.right.(int)
                        }
                    }

                    top_most_parent := &pool[stack[len(stack)-2]]
                    if top_most_parent.right == top_most {
                        top_most_parent.right = int( 0 )
                    } else {
                        assert( top_most_parent.left == top_most )
                        top_most_parent.left = int( 0 )
                    }

                    return true
                } else {
                    treated[pop( &stack )] = true
                }
        }
    }

    return false
}

part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    lines := strings.split(input, "\r\n")

    pool: [dynamic]SnailNumber
    numbers: [dynamic]SnailNumber
    for l in lines {
        id, _ := parse_number( transmute([]u8) l, 0, &pool )
        append( &numbers, pool[int(id)] )
    }

    no := add_numbers( numbers[0].id, numbers[1].id, &pool )
    print_number( no, pool[:] )
    fmt.println()
    fmt.println( reduce( no, pool[:] ) )
    print_number( no, pool[:] )
    fmt.println()
    fmt.println( reduce( no, pool[:] ) )
    print_number( no, pool[:] )
    fmt.println()

    fmt.println("==== Part 1 End ====")
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    lines := strings.split(input, "\r\n")



    fmt.println("==== Part 2 End ====")
}

main :: proc() {
    part1()
    // part2()
}