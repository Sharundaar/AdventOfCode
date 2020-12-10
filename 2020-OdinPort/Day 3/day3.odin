package main

import "core:strings"
import "core:strconv"
import "core:fmt"
import "core:os"
import "core:unicode/utf8"

PointType :: enum { empty, tree }


xor :: proc( a : bool, b : bool ) -> bool {
    return ( a && !b ) || ( !a && b );
}

main :: proc() {
    

    file := string( #load( "input.txt" ) );
    world : [dynamic]PointType;
    width := 0;
    for line in strings.split( file, "\r\n" ) {
        if len( line ) == 0 { continue; }
        if width == 0 { width = len( line ); }
        assert( width == len( line ) ); // making sure all line have the same width
        for c in line {
            switch c {
                case '#':
                    append( &world, PointType.tree );
                case '.':
                    append( &world, PointType.empty );
            }
        }
    }
    height := len( world ) / width;
    slopes := [] [2]int {
        [2]int{1, 1},
        [2]int{3, 1},
        [2]int{5, 1},
        [2]int{7, 1},
        [2]int{1, 2},
    };

    final_product := 1;
    for slope in slopes {
        x, y := 0, 0;
        tree_encountered := 0;
        for y < height {
            idx := (x % width) + y * width;
            if world[idx] == PointType.tree { tree_encountered += 1; }
            x += slope.x;
            y += slope.y;
        }
        fmt.printf( "Tree encountered for slope {{{},{}}} : {}\n", slope.x, slope.y, tree_encountered );
        final_product *= tree_encountered;
    }
    fmt.printf( "Final product: {}\n", final_product );
}