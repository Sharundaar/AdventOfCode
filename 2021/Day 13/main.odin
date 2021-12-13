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

Dot :: distinct [2]int
FoldSide :: enum { X, Y }
Fold :: struct {
    side: FoldSide,
    coord: int,
}

part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    lines := strings.split(input, "\r\n")

    dots : [dynamic]Dot
    folds : [dynamic]Fold
    fold_along := "fold along"
    for line in &lines {
        if line == "" do continue
        if len( line ) >= len( fold_along ) && line[0:len(fold_along)] == fold_along {
            splt := strings.split( line[len(fold_along)+1:], "=" ) ; defer delete( splt )
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

    for fold in folds {
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
    }

    // dedup
    mSet : map [Dot] bool
    for dot in &dots {
        mSet[dot] = true
    }
    count := 0
    for dot, _ in mSet {
        count += 1
        fmt.println( dot )
    }
    fmt.println(count)

    print_grid :: proc( dots: []Dot ) {
        max : [2]int
        for dot in dots {
            if dot.x > max.x do max.x = dot.x
            if dot.y > max.y do max.y = dot.y
        }

        for y := 0; y<=max.y; y += 1 {
            for x := 0; x <= max.x; x += 1 {
                found := find( dots, Dot{ x, y } )
                if found do fmt.printf( "#" )
                else do  fmt.printf( "." )
            }
            fmt.println()
        }
    }
    print_grid( dots[:] )

    fmt.println(len(mSet))

    fmt.println("==== Part 1 End ====")
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    lines := strings.split(input, "\n")

    fmt.println("==== Part 2 End ====")
}

main :: proc() {
    part1()
    // part2()
}