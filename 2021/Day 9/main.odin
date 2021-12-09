package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"
import "core:os"
import "core:intrinsics"

input :: string(#load( "input.txt" ))

Coordinate :: struct {
    x, y: i32,
}

get_height :: proc( heightmap: []u8, width, height: u8, coord: Coordinate ) -> u8 {
    if coord.x < 0 || coord.x >= i32(width) || coord.y < 0 || coord.y >= i32(height) {
        return 99
    }
    return heightmap[coord.y * i32(width) + coord.x]
}

get_color :: proc( colors: []int, width, height: u8, coord: Coordinate ) -> int {
    if coord.x < 0 || coord.x >= i32(width) || coord.y < 0 || coord.y >= i32(height) {
        return -1
    }
    return colors[coord.y * i32(width) + coord.x]
}

part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    lines := strings.split(input, "\n")
    width := u8(len( lines[0] ) - 1)
    height := u8(len( lines ))
    heightmap := make([]u8, int(height) * int(width) )
    for i in 0..<i32(height) {
        for j in 0..<i32(width) {
            heightmap[i * i32(width) + j] = lines[i][j] - '0'
        }
    }

    low_points: [dynamic]Coordinate
    for y in 0..<i32(height) {
        for x in 0..<i32(width) {
            left, top, right, bottom : Coordinate
            left.x = x - 1 ; left.y = y
            right.x = x + 1 ; right.y = y
            top.x = x ; top.y = y - 1
            bottom.x = x ; bottom.y = y + 1

            height_pt := get_height( heightmap, width, height, { x, y } )
            theight := get_height( heightmap, width, height, top )
            rheight := get_height( heightmap, width, height, right )
            lheight := get_height( heightmap, width, height, left )
            bheight := get_height( heightmap, width, height, bottom )

            if height_pt < theight && height_pt < rheight && height_pt < lheight && height_pt < bheight {
                append( &low_points, Coordinate{ x, y })
            }
        }
    }

    risk := 0
    for pt in &low_points {
        h := 1 + int(get_height( heightmap, width, height, pt ))
        risk += h
    }
    fmt.println( risk )

    fmt.println("==== Part 1 End ====")
}

flow_upward :: proc( heightmap: []u8, width, height: u8, coord: Coordinate, colors: []int, color: int ) -> int {
    left, top, right, bottom : Coordinate
    left.x = coord.x - 1 ; left.y = coord.y
    right.x = coord.x + 1 ; right.y = coord.y
    top.x = coord.x ; top.y = coord.y - 1
    bottom.x = coord.x ; bottom.y = coord.y + 1

    count := 1
    colors[coord.y * i32(width) + coord.x] = color

    theight := get_height( heightmap, width, height, top )
    rheight := get_height( heightmap, width, height, right )
    lheight := get_height( heightmap, width, height, left )
    bheight := get_height( heightmap, width, height, bottom )
    if theight < 9 && get_color( colors, width, height, top ) <= 0 do count += flow_upward( heightmap, width, height, top, colors, color )
    if bheight < 9 && get_color( colors, width, height, bottom ) <= 0 do count += flow_upward( heightmap, width, height, bottom, colors, color )
    if rheight < 9 && get_color( colors, width, height, right ) <= 0 do count += flow_upward( heightmap, width, height, right, colors, color )
    if lheight < 9 && get_color( colors, width, height, left ) <= 0 do count += flow_upward( heightmap, width, height, left, colors, color )

    return count
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    lines := strings.split(input, "\n")
    width := u8(len( lines[0] ) - 1)
    height := u8(len( lines ))
    heightmap := make([]u8, int(height) * int(width) )
    for i in 0..<i32(height) {
        for j in 0..<i32(width) {
            heightmap[i * i32(width) + j] = lines[i][j] - '0'
        }
    }

    low_points: [dynamic]Coordinate
    for y in 0..<i32(height) {
        for x in 0..<i32(width) {
            left, top, right, bottom : Coordinate
            left.x = x - 1 ; left.y = y
            right.x = x + 1 ; right.y = y
            top.x = x ; top.y = y - 1
            bottom.x = x ; bottom.y = y + 1

            height_pt := get_height( heightmap, width, height, { x, y } )
            theight := get_height( heightmap, width, height, top )
            rheight := get_height( heightmap, width, height, right )
            lheight := get_height( heightmap, width, height, left )
            bheight := get_height( heightmap, width, height, bottom )

            if height_pt < theight && height_pt < rheight && height_pt < lheight && height_pt < bheight {
                append( &low_points, Coordinate{ x, y })
            }
        }
    }

    colors := make( []int, int(height) * int(width) )
    bassins : [dynamic]int
    for low_pt, idx in &low_points {
        append(&bassins, flow_upward( heightmap, width, height, low_pt, colors, idx + 1 ) )
    }
    slice.sort( bassins[:] )
    last := len( bassins )
    fmt.println( bassins[last - 1] * bassins[last - 2] * bassins[last - 3] )
    fmt.println("==== Part 2 End ====")
}

main :: proc() {
    // part1()
    part2()
}