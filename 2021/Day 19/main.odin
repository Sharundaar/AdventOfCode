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

Scan :: [dynamic][3]int

string_starts_with :: proc( s: string, starts_with: string ) -> bool {
    l_starts_with := len( starts_with )
    return len( s ) >= l_starts_with && s[0:l_starts_with] == starts_with
}

part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    lines := strings.split(input, "\r\n")

    scanner_str :: "--- scanner "

    scans : [dynamic]Scan

    line_it := 0
    for line_it < len( lines ) {
        if string_starts_with( lines[line_it], scanner_str ) {
            append( &scans, [dynamic][3]int{} )
        } else if lines[line_it] == "" {
        } else {
            splt := strings.split( lines[line_it], "," )
            assert( len(splt) == 3 )
            v := [3]int {
                strconv.atoi( splt[0] ),
                strconv.atoi( splt[1] ),
                strconv.atoi( splt[2] ),
            }
            append( &scans[len(scans)-1], v )
        }
        line_it += 1
    }

    // store distance between all pair of points for each scan
    distance_matrices : [dynamic][][]f64
    dist :: proc( p1, p2: [3]int ) -> f64 {
        return math.sqrt( f64((p2.x-p1.x) * (p2.x-p1.x) + (p2.y-p1.y) * (p2.y-p1.y) + (p2.z-p1.z) * (p2.z-p1.z)) )
    }
    for scan, i in &scans {
        distance_matrix := make( [][]f64, len( scan ) )
        for k in 0..<len( scan ) do distance_matrix[k] = make( []f64, len(scan) )
        append( &distance_matrices, distance_matrix )
        for p1, i1 in &scan {
            for p2, i2 in &scan {
                distance_matrix[i1][i2] = dist( p1, p2 )
            }
        }
    }

    /*
    for mat, i in distance_matrices {
        fmt.printf( "--- matrix %d ---\n", i )
        for row in mat {
            fmt.printf( "[" )
            for column in row {
                fmt.printf( " %.2f", column )
            }
            fmt.printf( " ]" )
            fmt.println()
        }
    }
    */

    all_points : [dynamic][3]int
    for scan in scans {
        for pt in scan {
            // append(&all_points, pt)
            fmt.printf( "%d %d %d\n", pt.x, pt.y, pt.z )
        }
        fmt.println()
    }

    for p in all_points {
        
    }
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