package main

import "core:strings"
import "core:strconv"
import "core:fmt"
import "core:os"

main :: proc() {
    file := string( #load( "input.txt" ) );
    values : [dynamic]int;
    for line in strings.split( file, "\r\n" ) {
        n, ok := strconv.parse_int( line );
        if ok { append( &values, n ); }
    }

    value_count := len( values );
    loop1:
    for i in 0..<value_count-1 {
        for j in i+1..<value_count {
            if values[i] + values[j] == 2020 {
                fmt.println( "Numbers that sums to 2020 :", values[i], values[j] );
                fmt.println( "Product is :", values[i] * values[j] );
                break loop1;
            }
        }
    }

    loop2:
    for i in 0..<value_count-2 {
        for j in i+1..<value_count-1 {
            for k in j+1..<value_count {
                if values[i] + values[j] + values[k] == 2020 {
                    fmt.println( "Numbers that sums to 2020 :", values[i], values[j], values[k] );
                    fmt.println( "Product is :", values[i] * values[j] * values[k] );
                    break loop2;
                }
            }
        }
    }
}