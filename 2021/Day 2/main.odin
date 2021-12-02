package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"

input :: string(#load( "input.txt" ))

part1 :: proc() {
    lines := strings.split(input, "\n")
    depth : int = 0
    hor : int = 0

    for line in lines {
        splt := strings.split( line, " " )
        dir := splt[0]
        switch dir {
            case "forward": hor += strconv.atoi(splt[1])
            case "up": depth -= strconv.atoi(splt[1])
            case "down": depth += strconv.atoi(splt[1])
        }
    }
    fmt.println( depth * hor )
}

part2 :: proc() {
    lines := strings.split(input, "\n")
    depth : int = 0
    hor : int = 0
    aim : int = 0

    for line in &lines {
        splt := strings.split( line, " " )
        dir := splt[0]
        switch dir {
            case "forward": hor += strconv.atoi(splt[1]) ; depth += aim * strconv.atoi(splt[1])
            case "up": aim -= strconv.atoi(splt[1])
            case "down": aim += strconv.atoi(splt[1])
        }
    }
    fmt.println( depth * hor )
}

main :: proc() {
    part1()
    part2()
}