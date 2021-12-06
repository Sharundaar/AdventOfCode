package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"

input :: string(#load( "input.txt" ))

Bucket :: int
part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    numbers := slice.mapper( strings.split( input, "," ), strconv.atoi ) ; defer delete( numbers )
    buckets : [9]Bucket
    for num in numbers {
        buckets[num] += 1
    }

    for day := 0; day < 80; day += 1 {
        new := buckets[0]
        buckets[0] = buckets[1]
        buckets[1] = buckets[2]
        buckets[2] = buckets[3]
        buckets[3] = buckets[4]
        buckets[4] = buckets[5]
        buckets[5] = buckets[6]
        buckets[6] = new + buckets[7]
        buckets[7] = buckets[8]
        buckets[8] = new
    }
    count : int
    for b in buckets {
        count += b
    }
    fmt.println( count )

    fmt.println("==== Part 1 End ====")
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    numbers := slice.mapper( strings.split( input, "," ), strconv.atoi ) ; defer delete( numbers )
    buckets : [9]Bucket
    for num in numbers {
        buckets[num] += 1
    }

    for day := 0; day < 256; day += 1 {
        new := buckets[0]
        buckets[0] = buckets[1]
        buckets[1] = buckets[2]
        buckets[2] = buckets[3]
        buckets[3] = buckets[4]
        buckets[4] = buckets[5]
        buckets[5] = buckets[6]
        buckets[6] = new + buckets[7]
        buckets[7] = buckets[8]
        buckets[8] = new
    }
    count : int
    for b in buckets {
        count += b
    }
    fmt.println( count )

    fmt.println("==== Part 2 End ====")
}

main :: proc() {
    part1()
    part2()
}