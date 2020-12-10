package main

import "core:strings"
import "core:strconv"
import "core:fmt"
import "core:os"
import "core:unicode/utf8"

PassportEntry :: struct {
    byr, iyr, eyr : int,
    hgt : string,
    hcl, ecl := string,
    pid, cid := u64,
}

main :: proc() {
    file := string( #load( "input.txt" ) );

    for line in strings.split( file, "\r\n" ) {
        if len( line ) == 0 { continue; }
    }
}