package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"
import "core:os"
import "core:intrinsics"
import stb_image "vendor:stb/image"
import c "core:c/libc"

input :: string(#load( "input.txt" ))
vec2 :: distinct [2]int

part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    lines := strings.split(input, "\r\n")

    W, H :: 100, 100

    risk_levels : [H][W]int
    for line, y in lines {
        for c, x in line {
            risk_levels[y][x] = int(c - '0')
        }
    }

    
    start := vec2 { 0, 0 }
    dest := vec2{ W-1, H-1 }
    h :: proc( pos, dest: vec2, risk_levels: [H][W]int ) -> int {
        dist := (dest - pos)
        return (abs(dist.x) + abs(dist.y)) + risk_levels[pos.y][pos.x]
    }

    d :: proc( pos: vec2, risk_levels: [H][W]int ) -> int {
        return 1 + risk_levels[pos.y][pos.x]
    }
    
    openSet : [dynamic]vec2
    append( &openSet, start )
    cameFrom : map [vec2] vec2
    gScore : map [vec2] int
    gScore[start] = 0
    fScore : map [vec2] int
    fScore[start] = h( start, dest, risk_levels )

    dest_reached := false
    for len( openSet ) > 0 {
        // find lowest fScore in openSet
        current, current_score := vec2{}, max( int )
        for pos in openSet {
            fs := fScore[pos] if pos in fScore else max(int)
            if fs < current_score {
                current = pos
                current_score = fs
            }
        }
        if current == dest {
            dest_reached = true
            break // to be impl
        }

        // remove current from openSet
        for v, i in openSet {
            if v == current {
                unordered_remove( &openSet, i )
                break
            }
        }

        neighbors := []vec2 {
            { current.x - 1, current.y },
            { current.x + 1, current.y },
            { current.x, current.y - 1 },
            { current.x, current.y + 1 },
        }
        for n in neighbors {
            if n.x < 0 || n.x >= len(risk_levels[0]) || n.y < 0 || n.y >= len(risk_levels) do continue
            tentative_gScore := gScore[current] + h( n, current, risk_levels )
            if n not_in gScore || tentative_gScore < gScore[n] {
                cameFrom[n] = current
                gScore[n] = tentative_gScore
                fScore[n] = tentative_gScore + h( n, dest, risk_levels )
                if !slice.contains( openSet[:], n ) do append( &openSet, n )
            }
        }
        // fmt.println( openSet )
    }

    assert(dest_reached)
    total_path : [dynamic]vec2
    append( &total_path, dest )
    current := dest
    for {
        if current in cameFrom {
            current = cameFrom[current]
            append( &total_path, current )
            continue
        }
        break
    }

    slice.reverse( total_path[:] )

    total_risk := 0
    for p in total_path {
        if p == start do continue
        total_risk += risk_levels[p.y][p.x]
    }
    fmt.println( total_risk )

    fmt.println("==== Part 1 End ====")
}

write_risk_levels_with_path :: proc( risks: [][]int, path: []vec2 ) {
    filename := strings.clone_to_cstring( "thing.png" )
    w, h, comp : c.int = c.int(len(risks[0])), c.int(len(risks)), 3
    data := make( []u8, int(h) * int(w) * int(comp) )
    for y in 0..<int(h) do for x in 0..<int(w) {
        color := u8(f32(risks[y][x] - 1) / 8 * 255.0)
        data[ y * int(w) * int(comp) + x * int(comp) + 0 ] = color
        data[ y * int(w) * int(comp) + x * int(comp) + 1 ] = color
        data[ y * int(w) * int(comp) + x * int(comp) + 2 ] = color
    }

    for p in path {
        i := p.y * int(w) * int(comp) + p.x * int(comp)
        data[i + 0] = 255
        data[i + 1] = 0
        data[i + 2] = 0
    }

    stb_image.write_png( filename, w, h, comp, raw_data(data), 0)
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    lines := strings.split(input, "\r\n")
    TW, TH :: 100, 100
    W, H :: 500, 500


    risk_levels : [][]int = make( [][]int, H )
    for k in 0..<H {
        risk_levels[k] = make( []int, W )
    }
    
    for line, y in lines {
        for c, x in line {
            risk_levels[y][x] = int(c - '0')
        }
    }

    for y in 0..<W do for x in 0..<H {
        tile := vec2{ x / TW, y / TH }
        if tile.x == 0 && tile.y == 0 do continue

        increase := tile.x + tile.y
        initial_tile_coords := vec2{ x % TW, y % TH }
        risk_levels[y][x] = (risk_levels[initial_tile_coords.y][initial_tile_coords.x] + increase)
        if risk_levels[y][x] > 9 {
            risk_levels[y][x] -= 9
        }
    }

    start := vec2 { 0, 0 }
    dest := vec2{ W-1, H-1 }
    h :: proc( pos, dest: vec2, risk_levels: [][]int ) -> int {
        dist := (dest - pos)
        return (abs(dist.x) + abs(dist.y) - 1)
    }

    d :: proc( a, b: vec2, risk_levels: [][]int ) -> int {
        return risk_levels[a.y][a.x]
    }

    openSet : [dynamic]vec2
    append( &openSet, start )
    cameFrom : map [vec2] vec2
    gScore : map [vec2] int
    gScore[start] = 0
    fScore : map [vec2] int
    fScore[start] = h( start, dest, risk_levels )

    dest_reached := false
    for len( openSet ) > 0 {
        // find lowest fScore in openSet
        current, current_score := vec2{}, max( int )
        for pos in openSet {
            fs := fScore[pos] if pos in fScore else max(int)
            if fs < current_score {
                current = pos
                current_score = fs
            }
        }
        if current == dest {
            dest_reached = true
            break // to be impl
        }

        // remove current from openSet
        for v, i in openSet {
            if v == current {
                unordered_remove( &openSet, i )
                break
            }
        }

        neighbors := []vec2 {
            { current.x - 1, current.y },
            { current.x + 1, current.y },
            { current.x, current.y - 1 },
            { current.x, current.y + 1 },
        }
        for n in neighbors {
            if n.x < 0 || n.x >= len(risk_levels[0]) || n.y < 0 || n.y >= len(risk_levels) do continue
            tentative_gScore := gScore[current] + d( n, current, risk_levels )
            if n not_in gScore || tentative_gScore < gScore[n] {
                cameFrom[n] = current
                gScore[n] = tentative_gScore
                fScore[n] = tentative_gScore + h( n, dest, risk_levels )
                if !slice.contains( openSet[:], n ) do append( &openSet, n )
            }
        }
    }

    assert(dest_reached)
    total_path : [dynamic]vec2
    append( &total_path, dest )
    current := dest
    for {
        if current in cameFrom {
            current = cameFrom[current]
            append( &total_path, current )
            continue
        }
        break
    }

    slice.reverse( total_path[:] )

    total_risk := 0
    for p in total_path {
        if p == start do continue
        total_risk += risk_levels[p.y][p.x]
    }
    fmt.println( total_risk )
    write_risk_levels_with_path( risk_levels, total_path[:] )
    fmt.println("==== Part 2 End ====")
}

main :: proc() {
    part1()
    part2()
}