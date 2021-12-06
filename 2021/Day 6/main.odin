package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"

input :: string(#load( "input.txt" ))

Bucket :: struct {
    count: int,
    new: int,
}

part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    numbers := slice.mapper( strings.split( input, "," ), strconv.atoi ) ; defer delete( numbers )
    buckets : [9]Bucket
    for num in numbers {
        buckets[num].count += 1
    }

    for day := 0; day < 80; day += 1 {
        for i:=0; i<len(buckets); i += 1 {
            if i == 0 do buckets[8].new += buckets[0].count
            new_bucket_idx := i - 1
            if new_bucket_idx < 0 do new_bucket_idx = 6
            buckets[new_bucket_idx].new += buckets[i].count
            buckets[i].count = 0
        }
        for i:=0; i<len(buckets); i += 1 {
            buckets[i].count = buckets[i].new
            buckets[i].new = 0
        }
    }
    count : int
    for b in buckets {
        count += b.count
    }
    fmt.println( count )

    fmt.println("==== Part 1 End ====")
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    numbers := slice.mapper( strings.split( input, "," ), strconv.atoi ) ; defer delete( numbers )
    buckets : [9]Bucket
    for num in numbers {
        buckets[num].count += 1
    }

    for day := 0; day < 256; day += 1 {
        for i:=0; i<len(buckets); i += 1 {
            if i == 0 do buckets[8].new += buckets[0].count
            new_bucket_idx := i - 1
            if new_bucket_idx < 0 do new_bucket_idx = 6
            buckets[new_bucket_idx].new += buckets[i].count
            buckets[i].count = 0
        }
        for i:=0; i<len(buckets); i += 1 {
            buckets[i].count = buckets[i].new
            buckets[i].new = 0
        }
    }
    count : int
    for b in buckets {
        count += b.count
    }
    fmt.println( count )


    fmt.println("==== Part 2 End ====")
}

main :: proc() {
    part1()
    part2()
}