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

find_index :: proc( c: u8, s: []u8 ) -> int {
    for p, i in s {
        if p == c do return i
    }
    return -1
}

ChunkToken :: enum {
    Parenthesis,
    SquareBracket,
    Bracket,
    GTLT,
    Count,
}

ChunkTokenCharsOpen :: []u8{ '(', '[', '{', '<' }
ChunkTokenCharsClose :: []u8{ ')', ']', '}', '>' }
ChunkTokenPoints := []int{ 3, 57, 1197, 25137 }
ChunkTokenIncompletePoints := []int{ 1, 2, 3, 4 }
part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    lines := strings.split(input, "\n")

    chunk_token_counter : [ChunkToken.Count]int
    corrupted_count : int
    corrupted_token_count : [ChunkToken.Count]int
    stack: [dynamic]u8
    for line in &lines {
        for r in line {
            rc, i := utf8.encode_rune( r )
            assert( i == 1 )
            c := rc[0]
            
            idx := find_index( c, ChunkTokenCharsOpen )
            if idx != -1 {
                append( &stack, c )
                continue
            }
            idx = find_index( c, ChunkTokenCharsClose )
            if idx != -1 {
                last := stack[len(stack)-1]
                last_idx := find_index( last, ChunkTokenCharsOpen )
                if last_idx != idx {
                    corrupted_count += 1
                    corrupted_token_count[idx] += 1
                    break
                } else {
                    pop( &stack )
                }
                continue
            }
            
            if r == '\n' || r == '\r' do continue
            fmt.println( "wrong char", c )
            assert( false )
        }
    }
    fmt.println( corrupted_count )
    fmt.println( corrupted_token_count )
    points : int
    for c, i in corrupted_token_count {
        points += c * ChunkTokenPoints[i]
    }
    fmt.println( points )

    fmt.println("==== Part 1 End ====")
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    lines := strings.split(input, "\n")

    scores: [dynamic]int
    line_loop: for line in &lines {
        stack: [dynamic]u8 ; defer delete( stack )
        for r in line {
            rc, i := utf8.encode_rune( r )
            assert( i == 1 )
            c := rc[0]
            
            idx := find_index( c, ChunkTokenCharsOpen )
            if idx != -1 {
                append( &stack, c )
                continue
            }
            idx = find_index( c, ChunkTokenCharsClose )
            if idx != -1 {
                last := stack[len(stack)-1]
                last_idx := find_index( last, ChunkTokenCharsOpen )
                if last_idx != idx {
                    continue line_loop // corrupted line, skip
                } else {
                    pop( &stack )
                }
                continue
            }
            
            if r == '\n' || r == '\r' do continue
            fmt.println( "wrong char", c )
            assert( false )
        }

        if len( stack ) > 0 {
            line_points: int
            for len(stack) > 0 {
                last := stack[len(stack)-1]
                last_idx := find_index( last, ChunkTokenCharsOpen )
                pop( &stack )
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