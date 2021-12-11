package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"
import "core:os"
import "core:intrinsics"

input :: string(#load( "input.txt" ))

part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    lines := strings.split(input, "\n")
    energies : [10*10]int
    for y in 0..<10 {
        for x in 0..<10 {
            energies[10*y+x] = int(lines[y][x] - '0')
        }
    }

    step_count := 100
    flash_count := 0
    for _  in 0..<step_count {
        // increase all energies by one
        for i in 0..<len(energies) {
            energies[i] += 1
        }

        // trigger flashes
        flashed : [10*10]bool
        for {
            flash_happened := false
            for y in 0..<10 do for x in 0..<10 {
                    if energies[10*y+x] > 9 && !flashed[10*y+x] {
                        for ny in y-1..y+1 {
                            for nx in x-1..x+1 {
                                if nx == x && ny == y do continue
                                if nx >= 0 && nx < 10 && ny >= 0 && ny < 10 {
                                    energies[10*ny+nx] += 1
                                }
                            }
                        }
                        flashed[10*y+x] = true
                        flash_happened = true
                        flash_count += 1
                    }
                }

            if !flash_happened do break
        }

        for i in 0..<len(flashed) {
            if flashed[i] do energies[i] = 0
        }
    }
    fmt.println(energies)
    fmt.println(flash_count)

    fmt.println("==== Part 1 End ====")
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    lines := strings.split(input, "\n")
    energies : [10*10]int
    for y in 0..<10 {
        for x in 0..<10 {
            energies[10*y+x] = int(lines[y][x] - '0')
        }
    }

    step := 0
    for {
        // increase all energies by one
        for i in 0..<len(energies) {
            energies[i] += 1
        }

        flash_count := 0
        // trigger flashes
        flashed : [10*10]bool
        for {
            flash_happened := false
            for y in 0..<10 do for x in 0..<10 {
                if energies[10*y+x] > 9 && !flashed[10*y+x] {
                    for ny in y-1..y+1 {
                        for nx in x-1..x+1 {
                            if nx == x && ny == y do continue
                            if nx >= 0 && nx < 10 && ny >= 0 && ny < 10 {
                                energies[10*ny+nx] += 1
                            }
                        }
                    }
                    flashed[10*y+x] = true
                    flash_happened = true
                    flash_count += 1
                }
            }

            if !flash_happened do break
        }

        for i in 0..<len(flashed) {
            if flashed[i] do energies[i] = 0
        }

        step += 1
        if flash_count == len(flashed) {
            break
        }
    }
    fmt.println(step)
    fmt.println("==== Part 2 End ====")
}

main :: proc() {
    part1()
    part2()
}