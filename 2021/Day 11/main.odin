package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"
import "core:os"
import "core:intrinsics"

input :: string(#load( "input.txt" ))

contains_slice :: proc( container, containee: $T/[]$E ) -> bool where intrinsics.type_is_comparable(E) {
	n := len( containee )
	if n > len( container ) {
		return false
	}
	for i in 0..<n {
		if !find( container, containee[i] ) {
			return false
		}
	}
	return true
}

find :: proc{
    slice.contains,
    contains_slice,
}

BMPHeader :: struct {
    header: [2]u8,
    file_size: u32,
    reserved: u32,
    image_data_offset: u32,
}

DIBHeader :: struct {
    header_size: u32,
    width: u16,
    height: u16,
    color_planes_count: u16,
    bpp: u16,
    compression: u32,
    image_size: u32,
    hor_resolution: i32,
    vert_resolution: i32,
    color_in_palette_count: u32,
    important_color_count: u32,
}



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
        for y in 0..<10 {
            for x in 0..<10 {
                energies[10*y+x] += 1
            }
        }

        // trigger flashes
        flashed : [10*10]bool
        for {
            flash_happened := false
            for y in 0..<10 {
                for x in 0..<10 {
                    if energies[10*y+x] > 9 && !flashed[10*y+x] {
                        flashed[10*y+x] = true
                        flash_happened = true
                        flash_count += 1
                        for ny in y-1..y+1 {
                            for nx in x-1..x+1 {
                                if nx == x && ny == y do continue
                                if nx >= 0 && nx < 10 && ny >= 0 && ny < 10 {
                                    energies[10*ny+nx] += 1
                                }
                            }
                        }
                    }
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
        for y in 0..<10 {
            for x in 0..<10 {
                energies[10*y+x] += 1
            }
        }

        flash_count := 0
        // trigger flashes
        flashed : [10*10]bool
        for {
            flash_happened := false
            for y in 0..<10 {
                for x in 0..<10 {
                    if energies[10*y+x] > 9 && !flashed[10*y+x] {
                        flashed[10*y+x] = true
                        flash_happened = true
                        flash_count += 1
                        for ny in y-1..y+1 {
                            for nx in x-1..x+1 {
                                if nx == x && ny == y do continue
                                if nx >= 0 && nx < 10 && ny >= 0 && ny < 10 {
                                    energies[10*ny+nx] += 1
                                }
                            }
                        }
                    }
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