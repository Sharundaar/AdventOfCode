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

Rule :: struct {
    input : [2]u8,
    output : u8,
}

part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    lines := strings.split(input, "\r\n")

    formula : [dynamic]u8
    for _, i in lines[0] {
        append(&formula, lines[0][i] )
    }

    rules : [dynamic]Rule
    for line in lines[2:] {
        splt := strings.split( line, " -> " )
        rule : Rule
        rule.input[0] = splt[0][0] ; rule.input[1] = splt[0][1]
        rule.output = splt[1][0]
        append( &rules, rule )
    }

    formula_next : [dynamic]u8
    for step in 0..<10 {
        clear( &formula_next )
        len_formula := len( formula )
        for i in 0..<len_formula {
            append( &formula_next, formula[i] )
            if i < len_formula - 1 {
                for rule in &rules {
                    using rule
                    if formula[i] == input[0] && formula[i+1] == input[1] {
                        append( &formula_next, output )
                    }
                }
            }
        }
        clear( &formula )
        append( &formula, ..formula_next[:] )

        when false {
            fmt.printf( "After step %d: ", step )
            for c in formula {
                fmt.printf( "%c", c )
            }
            fmt.println()
        }
    }

    elem_count_map : map [u8] int
    for c in formula {
        if c not_in elem_count_map do elem_count_map[c] = 0
        elem_count_map[c] += 1
    }

    least_common_count, least_common := max(int), u8(0)
    most_common_count, most_common := 0, u8(0)
    for k, v in elem_count_map {
        if v < least_common_count {
            least_common = k
            least_common_count = v
        }
        if v > most_common_count {
            most_common = k
            most_common_count = v
        }
    }

    fmt.println( most_common_count - least_common_count )

    fmt.println("==== Part 1 End ====")
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    lines := strings.split(input, "\r\n")

    formula_input := lines[0]
    rules_input := lines[2:]

    // init
    pairs : map [[2]u8] int ; delete( pairs )
    rules : [dynamic]Rule ; delete( rules )
    elem_count_map : map [u8] int ; delete( elem_count_map )
    for line in rules_input {
        splt := strings.split( line, " -> " ) ; defer delete( splt )
        rule : Rule
        rule.input[0] = splt[0][0] ; rule.input[1] = splt[0][1]
        rule.output = splt[1][0]

        // init individual character counts
        elem_count_map[rule.input[0]] = 0
        elem_count_map[rule.input[1]] = 0
        elem_count_map[rule.output] = 0

        // init pair count
        pairs[rule.input] = 0
        append( &rules, rule )
    }

    for _, i in formula_input {
        elem_count_map[formula_input[i]] += 1
        if i < len(formula_input) - 1 {
            pair := [2]u8 { formula_input[i], formula_input[i+1] }
            pairs[pair] += 1
        }
    }

    next_pairs : map[[2]u8]int
    for step in 0..<40 {
        for k, v in pairs {
            next_pairs[k] = v
        }
        for rule in &rules {
            using rule
            pair_1 := [2]u8{ input[0], output }
            pair_2 := [2]u8{ output, input[1] }
            count := pairs[input]
            next_pairs[input] -= pairs[input]
            next_pairs[pair_1] += pairs[input]
            next_pairs[pair_2] += pairs[input]
            elem_count_map[output] += pairs[input]
        }
        for k, v in next_pairs {
            pairs[k] = v
        }
    }

    least_common_count, least_common := max(int), u8(0)
    most_common_count, most_common := 0, u8(0)
    for k, v in elem_count_map {
        if v < least_common_count {
            least_common = k
            least_common_count = v
        }
        if v > most_common_count {
            most_common = k
            most_common_count = v
        }
    }

    fmt.println( most_common_count - least_common_count )
    fmt.println("==== Part 2 End ====")
}

TRACKING_MEM :: false

main :: proc() {
    when TRACKING_MEM {
        track : mem.Tracking_Allocator
        mem.tracking_allocator_init(&track, context.allocator)
        defer mem.tracking_allocator_destroy(&track)
        context.allocator = mem.tracking_allocator(&track)
    }

    part1()
    part2()

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