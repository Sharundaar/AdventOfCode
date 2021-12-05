package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"

input :: string(#load( "input.txt" ))

vec2 :: distinct [2]int
vec2f :: distinct [2]f32

Line :: struct {
    p1: vec2,
    p2: vec2,
}

sign :: proc( n: int ) -> int {
    if n < 0 do return -1
    if n > 0 do return 1
    return 0
}

part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    lines_txt := strings.split(input, "\n") ; defer delete( lines_txt )
    lines : [dynamic]Line

    for line in &lines_txt {
        l: Line
        
        splt := strings.split( line, " -> " ) ; defer delete( splt )
        p1 := strings.split( splt[0], "," ) ; defer delete( p1 )
        l.p1.x = strconv.atoi( p1[0] )
        l.p1.y = strconv.atoi( p1[1] )

        p2 := strings.split( splt[1], "," ) ; defer delete( p2 )
        l.p2.x = strconv.atoi( p2[0] )
        l.p2.y = strconv.atoi( p2[1] )

        append( &lines, l )
    }

    LENGTH :: 1000
    bitmap: [LENGTH*LENGTH]u8
    for line in &lines {
        if !(line.p1.x == line.p2.x || line.p1.y == line.p2.y) do continue

        if line.p1.x == line.p2.x {
            dir := sign(line.p2.y - line.p1.y)
            x := line.p1.x
            for y := line.p1.y ; y != line.p2.y; y += dir {
                bitmap[y * LENGTH + x] += 1
            }
            bitmap[line.p2.y * LENGTH + line.p2.x] += 1
        } else {
            dir := sign(line.p2.x - line.p1.x)
            y := line.p1.y
            for x := line.p1.x ; x != line.p2.x; x += dir {
                bitmap[y * LENGTH + x] += 1
            }
            bitmap[line.p2.y * LENGTH + line.p2.x] += 1
        }
    }

    count : int
    for c in bitmap {
        if c > 1 do count += 1
    }
    fmt.println( count )

    fmt.println("==== Part 1 End ====")
}

length :: proc( v: vec2f ) -> f32 {
    return math.sqrt( v.x*v.x + v.y*v.y )
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    lines_txt := strings.split(input, "\n") ; defer delete( lines_txt )
    lines : [dynamic]Line

    for line in &lines_txt {
        l: Line
        
        splt := strings.split( line, " -> " ) ; defer delete( splt )
        p1 := strings.split( splt[0], "," ) ; defer delete( p1 )
        l.p1.x = strconv.atoi( p1[0] )
        l.p1.y = strconv.atoi( p1[1] )

        p2 := strings.split( splt[1], "," ) ; defer delete( p2 )
        l.p2.x = strconv.atoi( p2[0] )
        l.p2.y = strconv.atoi( p2[1] )

        append( &lines, l )
    }

    LENGTH :: 1000
    bitmap: [LENGTH*LENGTH]u8
    for line in &lines {
        dir := vec2{ sign( line.p2.x - line.p1.x ), sign( line.p2.y - line.p1.y ) }
        for p := line.p1 ;; {
            bitmap[p.y * LENGTH + p.x] += 1
            if p == line.p2 do break
            p += dir
        }
    }

    count : int
    for c in bitmap {
        if c > 1 do count += 1
    }
    fmt.println( count )

    fmt.println("==== Part 2 End ====")
}

main :: proc() {
    part1()
    part2()
}