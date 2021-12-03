package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"

input :: string(#load( "input.txt" ))

part1 :: proc() {
    using strconv

    BIT_COUNT :: 12
    one_count : [BIT_COUNT]int
    lines := strings.split(input, "\n")
    line_count := len(lines)
    for line in lines {
        for i := 0; i<len(one_count); i+= 1 {
            if line[i] == '1' do one_count[i] += 1
        }
    }

    gamma, epsilon: int
    for i := 0; i<len(one_count); i += 1 {
        if one_count[i] > line_count - one_count[i] {
            gamma |= (1 << u32( BIT_COUNT - 1 - i ))
        } else {
            epsilon |= (1 << u32( BIT_COUNT - 1 - i ))
        }
    }

    fmt.println( gamma * epsilon )
}

part2_rec :: proc( input_lines: []string, consider: []bool, idx: int, most_consider: bool ) {
    one_count, line_count: int
    for i:=0; i<len(input_lines); i+=1 {
        if !consider[i] do continue

        if input_lines[i][idx] == '1' do one_count += 1
        line_count += 1
    }
    if line_count == 1 do return;

    bit: u8 = '0'
    if one_count >= line_count - one_count {
        bit = '1'
    }

    if !most_consider {
        if bit == '1' do bit = '0'
        else do bit = '1'
    }

    for i:=0; i<len(input_lines); i+=1 {
        if consider[i] && input_lines[i][idx] != bit {
            consider[i] = false
        }
    }

    consider_count : int
    for i:=0; i<len(input_lines); i+=1 {
        if consider[i] do consider_count += 1
    }
    if consider_count > 1 do part2_rec( input_lines, consider, idx + 1, most_consider )
}

part2 :: proc() {
    using strconv

    lines := strings.split(input, "\n")
    get_line_and_reset_array :: proc( consider: []bool, lines: []string ) -> string {
        str: string
        for i:=0; i<len(consider); i+=1 {
            if consider[i] {
                str = lines[i]
            }
            consider[i] = true
        }
        return str
    }

    consider := make([]bool, len(lines))
    get_line_and_reset_array( consider, lines )

    part2_rec( lines[:], consider, 0, true )
    ox_rating, _ := parse_int( get_line_and_reset_array( consider, lines ), 2 )

    part2_rec( lines[:], consider, 0, false )
    co2_rating, _ := parse_int( get_line_and_reset_array( consider, lines ), 2 )

    fmt.println( ox_rating * co2_rating )
}

main :: proc() {
    part1()
    part2()
}