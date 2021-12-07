package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"
import "core:os"

input :: string(#load( "input.txt" ))

Bucket :: int
part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    numbers := slice.mapper( strings.split( input, "," ), strconv.atoi ) ; defer delete( numbers )
    slice.sort( numbers )
    min_val := numbers[0]
    max_val := numbers[len(numbers)-1]
    fuel_min := max(int)
    for test := min_val; test <= max_val; test += 1 {
        fuel : int
        for n in numbers {
            fuel += abs( n - test )
        }
        if fuel < fuel_min do fuel_min = fuel
    }
    fmt.println( fuel_min )


    fmt.println("==== Part 1 End ====")
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    numbers := slice.mapper( strings.split( input, "," ), strconv.atoi ) ; defer delete( numbers )
    slice.sort( numbers )
    min_val := numbers[0]
    max_val := numbers[len(numbers)-1]
    values : [dynamic]int
    fuel_min := max(int)
    for test := min_val; test <= max_val; test += 1 {
        fuel : int
        for n in numbers {
            dist := abs( n - test )
            fuel += dist * (dist + 1) / 2
        }
        append(&values, fuel)
        if fuel < fuel_min do fuel_min = fuel
    }
    fmt.println( fuel_min )

    fmt.println("==== Part 2 End ====")
}

main :: proc() {
    part1()
    part2()
}