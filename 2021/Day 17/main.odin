package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"
import "core:os"
import "core:intrinsics"

input :: string(#load( "input.txt" ))

vec2 :: distinct [2]int
Area :: struct {
    min: vec2,
    max: vec2,
}

TargetArea_Input :: Area {
    { 156, -110 },
    { 202, -69 },
}

TargetArea_Test :: Area {
    { 20, -10 },
    { 30, -5 },
}

TargetArea :: TargetArea_Input

in_area :: proc( p: vec2, a: Area ) -> bool {
    return p.x >= a.min.x && p.x <= a.max.x && p.y >= a.min.y && p.y <= a.max.y
}

part1_and_2 :: proc() {
    fmt.println("==== Part 1 Begin ====")

    ps := proc( step: int, v0: vec2 ) -> vec2 {
        if step == 0 do return vec2{} // p0 is (0, 0)
        sum := (step*(step-1)) / 2
        drag := vec2 { min( v0.x, sum ), sum } // accumulated drag at step 2
        return (step-1) * v0 - drag
    }

    v0_count : int
    for y in -1000..1000 do for x in 0..1000 {
        p := vec2{}
        v0 := vec2{ x, y }
        v := v0
        s := 0
        inside := false
        highest_y := 0
        for {
            s += 1
            p += v
            v.y -= 1
            v.x -= 1
            v.x = max( v.x, 0 )
            if p.y > highest_y do highest_y = p.y
            if in_area( p, TargetArea ) {
                inside = true
                break
            }
            if p.x > TargetArea.max.x || p.y < TargetArea.min.y do break
        }
        if inside {
            v0_count += 1
            fmt.println( v0 )
        }
    }
    fmt.println( v0_count )


    fmt.println("==== Part 1 End ====")
}

main :: proc() {
    part1_and_2()
}