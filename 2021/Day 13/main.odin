package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"
import "core:os"
import "core:intrinsics"
import "core:mem"

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

Dot :: distinct [2]int
FoldSide :: enum { X, Y }
Fold :: struct {
    side: FoldSide,
    coord: int,
}

string_starts_with :: proc( s: string, starts_with: string ) -> bool {
    l_starts_with := len( starts_with )
    return len( s ) >= l_starts_with && s[0:l_starts_with] == starts_with
}

part1_and_2 :: proc() {
    fmt.println("==== Part 1-2 Begin ====")
    lines := strings.split(input, "\r\n") ; defer delete( lines )

    dots : [dynamic]Dot ; defer delete( dots )
    folds : [dynamic]Fold ; defer delete( folds )
    for line in &lines {
        if line == "" do continue
        if string_starts_with( line, "fold along" ) {
            splt := strings.split( line[len("fold along")+1:], "=" ) ; defer delete( splt )
            fold: Fold
            fold.side = .X if splt[0] == "x" else .Y
            fold.coord = strconv.atoi( splt[1] )
            append( &folds, fold )
        } else {
            splt := strings.split( line, "," ) ; defer delete( splt )
            dot : Dot
            dot.x = strconv.atoi( splt[0] )
            dot.y = strconv.atoi( splt[1] )
            append( &dots, dot )
        }
    }

    for fold, i in &folds {
        using fold
        switch side {
            case .X:
                for dot in &dots {
                    if dot.x > coord {
                        dist := dot.x - coord
                        dot.x = coord - dist
                    }
                }
            case .Y:
                for dot in &dots {
                    if dot.y > coord {
                        dist := dot.y - coord
                        dot.y = coord - dist
                    }
                }
        }
        if i == 0 {
            // dedup
            mSet : map [Dot] bool ; defer delete( mSet )
            for dot in &dots {
                mSet[dot] = true
            }
            fmt.println( len( mSet ) )
        }
    }

    print_grid :: proc( dots: []Dot ) {
        max : [2]int
        for dot in dots {
            if dot.x > max.x do max.x = dot.x
            if dot.y > max.y do max.y = dot.y
        }

        using strings
        b: Builder
        init_builder( &b, 0, 2048 ) ; defer destroy_builder( &b )
        for y := 0; y<=max.y; y += 1 {
            for x := 0; x <= max.x; x += 1 {
                found := find( dots, Dot{ x, y } )
                if found do write_rune_builder( &b, '#' )
                else do     write_rune_builder( &b, '.' )
            }
            write_rune_builder( &b, '\n' )
        }
        fmt.println( to_string( b ) )
    }
    print_grid( dots[:] )

    fmt.println("==== Part 1-2 End ====")
}

TRACKING_MEM :: true
main :: proc() {
    when TRACKING_MEM {
        track : mem.Tracking_Allocator
        mem.tracking_allocator_init(&track, context.allocator)
        defer mem.tracking_allocator_destroy(&track)
        context.allocator = mem.tracking_allocator(&track)
    }

    part1_and_2()

    when TRACKING_MEM {
        if len(track.allocation_map) > 0 {
            fmt.println()
            total : int
            for _, v in track.allocation_map {
                total += v.size
                fmt.printf("%v - leaked %v bytes\n", v.location, v.size)
            }
            fmt.println( "leaked", total, "bytes" )
        }
    }
}