package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"
import "core:os"
import "core:intrinsics"
import "core:unicode/utf8"

input :: string(#load( "input.txt" ))

find_index :: proc( c: u8, s: []u8 ) -> (int, bool) {
    for p, i in s {
        if p == c do return i, true
    }
    return 0, false
}

TOKEN_TYPE_COUNT :: 4
ChunkTokenCharsOpen :: []u8{ '(', '[', '{', '<' }
ChunkTokenCharsClose :: []u8{ ')', ']', '}', '>' }
ChunkTokenCorruptedPoints := []int{ 3, 57, 1197, 25137 }
ChunkTokenIncompletePoints := []int{ 1, 2, 3, 4 }

part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    lines := strings.split(input, "\n")

    corrupted_token_count : [TOKEN_TYPE_COUNT]int
    for line in &lines {
        stack: [dynamic]u8 ; defer if cap( stack ) > 0 do delete( stack )
        for r in line {
            rc, i := utf8.encode_rune( r )
            assert( i == 1 )
            c := rc[0]
            
            if idx, ok := find_index( c, ChunkTokenCharsOpen ); ok {
                append( &stack, c )
                continue
            }
            if idx, ok := find_index( c, ChunkTokenCharsClose ); ok {
                last := pop( &stack )
                last_idx, _ := find_index( last, ChunkTokenCharsOpen )
                if last_idx != idx {
                    corrupted_token_count[idx] += 1
                    break
                }
                continue
            }
        }
    }
    fmt.println( corrupted_token_count )
    points : int
    for c, i in corrupted_token_count {
        points += c * ChunkTokenCorruptedPoints[i]
    }
    fmt.println( points )

    fmt.println("==== Part 1 End ====")
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    lines := strings.split(input, "\n")

    scores: [dynamic]int
    line_loop: for line in &lines {
        stack: [dynamic]u8 ; defer if cap( stack ) > 0 do delete( stack )
        for r in line {
            rc, i := utf8.encode_rune( r )
            assert( i == 1 )
            c := rc[0]
            
            if idx, ok := find_index( c, ChunkTokenCharsOpen ); ok {
                append( &stack, c )
                continue
            }
            
            if idx, ok := find_index( c, ChunkTokenCharsClose ); ok {
                last := pop( &stack )
                last_idx, _ := find_index( last, ChunkTokenCharsOpen )
                if last_idx != idx {
                    continue line_loop // corrupted line, skip
                }
                continue
            }
        }

        if len( stack ) > 0 {
            line_points: int
            for {
                last, ok := pop_safe( &stack )
                if !ok do break
                last_idx, _ := find_index( last, ChunkTokenCharsOpen )
                line_points = line_points * 5 + ChunkTokenIncompletePoints[last_idx]
            }
            append( &scores, line_points )
        }
    }

    slice.sort( scores[:] )
    fmt.println( scores )
    fmt.println( scores[len(scores)/2] )

    fmt.println("==== Part 2 End ====")
}

main :: proc() {
    part1()
    part2()
}