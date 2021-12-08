package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"
import "core:os"

input :: string(#load( "input.txt" ))

// lines := strings.split(input, "\n")
// numbers := slice.mapper( strings.split( input, "," ), strconv.atoi ) ; defer delete( numbers )

Entry :: struct {
    input: [10][dynamic]u8,
    output: [4][dynamic]u8,
}

Bucket :: int
part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    lines := strings.split(input, "\n")
    entries: [dynamic]Entry
    for line in &lines {
        fields := strings.fields( line )
        entry: Entry
        for i in 0..<10 {
            for f in fields[i] {
                append(&entry.input[i], u8(f))
            }
            slice.sort( entry.input[i][:] )
        }
        assert( fields[10] == "|" )
        for i in 11..=14 {
            for f in fields[i] {
                append(&entry.output[i-11], u8(f))
            }
            slice.sort( entry.output[i-11][:] )
        }
        append(&entries, entry)
    }

    count : int
    for entry in &entries {
        for o in &entry.output {
            //          8                 1               4                 7
            if len( o ) == 7 || len( o ) == 2 || len( o ) == 4 || len( o ) == 3 
            {
                count += 1
            }
        }
    }
    fmt.println( count )

    fmt.println("==== Part 1 End ====")
}

find :: proc( src: []u8, v: u8 ) -> bool {
    for s in src {
        if s == v do return true
    }
    return false
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")

    lines := strings.split(input, "\n")
    entries: [dynamic]Entry
    for line in &lines {
        fields := strings.fields( line )
        entry: Entry
        for i in 0..<10 {
            for f in fields[i] {
                append(&entry.input[i], u8(f))
            }
            slice.sort( entry.input[i][:] )
        }
        assert( fields[10] == "|" )
        for i in 11..=14 {
            for f in fields[i] {
                append(&entry.output[i-11], u8(f))
            }
            slice.sort( entry.output[i-11][:] )
        }
        append(&entries, entry)
    }

    count : int
    for entry in &entries {
        patterns : [10][dynamic]u8
        // first pass, find 1 4 7 8
        for i in &entry.input {
            if len( i ) == 7 do patterns[8] = i
            else if len( i ) == 2 do patterns[1] = i
            else if len( i ) == 4 do patterns[4] = i
            else if len( i ) == 3 do patterns[7] = i
        }

        // deduce 6, 9 and 0 thanks to 1 and 4
        for i in &entry.input {
            if len( i ) == 6 {
                found_one : int
                found_four : int
                for one_spot in &patterns[1] {
                    for spot in &i {
                        if spot == one_spot do found_one += 1
                    }
                }
                for four_spot in &patterns[4] {
                    for spot in &i {
                        if spot == four_spot do found_four += 1
                    }
                }
                if found_one == len(patterns[1]) {
                    if found_four == len( patterns[4] ) {
                        patterns[9] = i
                    } else {
                        patterns[0] = i
                    }
                } 
                else {
                    patterns[6] = i
                }
            }
        }

        // my notation for spot1/2
        //   000
        //  5   1
        //  5   1
        //   666
        //  4   2
        //  4   2
        //   333

        spot_1 : u8
        spot_2 : u8

        // spot_1 is in 1 but not in 6
        // spot_2 is in 1 and 6
        for p in patterns[1] {
            if find( patterns[6][:], p ) {
                spot_2 = p
            } else {
                spot_1 = p
            }
        }

        // deduce 2, 3 and 5 thanks to 1 and 6
        for i in &entry.input {
            if len( i ) == 5 {
                if !find( i[:], spot_1 ) { // has to be 5
                    patterns[5] = i
                } else {
                    if find( i[:], spot_2 ) {
                        patterns[3] = i
                    } else {
                        patterns[2] = i
                    }
                }
            }
        }

        result : int
        for o in &entry.output {
            for i in 0..<len(patterns) {
                if slice.equal( o[:], patterns[i][:] ) {
                    result += i
                    result *= 10
                    break
                }
            }
        }
        result /= 10 // I'm *= 10 too much
        count += result
    }

    fmt.println( count )
    fmt.println("==== Part 2 End ====")
}

main :: proc() {
    part1()
    part2()
}