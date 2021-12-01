package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"

input :: string(#load( "input.txt" ))

main :: proc() {
    measurements := slice.mapper(strings.split(input, "\n"), strconv.atoi)

    count_increase :: proc( measurements: []int ) -> int {
        increasing_count := 0
        for measurement, i in measurements[1:] {
            if measurements[i] < measurement {
                increasing_count += 1
            }
        }
        return increasing_count
    }
    fmt.println( count_increase( measurements ) )

    measurements_window := make( [dynamic]int, 0, len(measurements)-2 )
    for i := 1; i<len(measurements)-1; i += 1 {
        append( &measurements_window, measurements[i-1] + measurements[i] + measurements[i+1] )
    }
    fmt.println( count_increase( measurements_window[:] ) )
}