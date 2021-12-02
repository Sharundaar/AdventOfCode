package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"

input :: string(#load( "input.txt" ))

part1 :: proc() {
    using strings, strconv

    lines := split(input, "\n")
    depth, hor : int

    for line in lines {
        splt := split( line, " " )
        dir := splt[0]
        switch dir {
            case "forward": hor += atoi(splt[1])
            case "up": depth -= atoi(splt[1])
            case "down": depth += atoi(splt[1])
        }
    }
    fmt.println( depth * hor )
}

part2 :: proc() {
    using strings, strconv

    lines := split(input, "\n")
    depth, hor, aim: int

    for line in &lines {
        splt := split( line, " " )
        dir := splt[0]
        switch dir {
            case "forward": hor += atoi(splt[1]) ; depth += aim * atoi(splt[1])
            case "up": aim -= atoi(splt[1])
            case "down": aim += atoi(splt[1])
        }
    }
    fmt.println( depth * hor )
}

main :: proc() {
    part1()
    part2()
}