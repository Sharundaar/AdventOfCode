package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"
import "core:os"
import "core:intrinsics"
import "core:mem"
import stb_image "vendor:stb/image"
import c "core:c/libc"

input :: string(#load( "input.txt" ))

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

write_paper_grid :: proc( dots: []Dot ) {
    max : [2]int
    for dot in dots {
        if dot.x > max.x do max.x = dot.x
        if dot.y > max.y do max.y = dot.y
    }

    filename := strings.clone_to_cstring( "thing.png" )
    w, h, comp : c.int = c.int((max.x+1)*2), c.int((max.y+1)*2), 3
    data := make( []u8, int(h) * int(w) * int(comp) )

    for d in dots {
        i := d.y * 2 * int(w) * int(comp) + d.x * 2 * int(comp)
        i1 := i
        i2 := i + int( comp )
        i3 := i + int( w ) * int( comp )
        i4 := i2 + i3 - i1
        is := []int { i1, i2, i3, i4 }
        for k in is {
            data[k + 0] = 255
            data[k + 1] = 255
            data[k + 2] = 255
        }
    }

    stb_image.write_png( filename, w, h, comp, raw_data(data), 0 )
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
                found := slice.contains( dots, Dot{ x, y } )
                if found do write_rune_builder( &b, '#' )
                else do     write_rune_builder( &b, '.' )
            }
            write_rune_builder( &b, '\n' )
        }
        fmt.println( to_string( b ) )
    }
    print_grid( dots[:] )
    write_paper_grid( dots[:] )

    fmt.println("==== Part 1-2 End ====")
}

TRACKING_MEM :: false
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