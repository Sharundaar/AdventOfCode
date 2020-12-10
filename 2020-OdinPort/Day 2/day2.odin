package main

import "core:strings"
import "core:strconv"
import "core:fmt"
import "core:os"
import "core:unicode/utf8"

Entry :: struct {
    low, high: int,
    char: rune,
    password : string,
}

xor :: proc( a : bool, b : bool ) -> bool {
    return ( a && !b ) || ( !a && b );
}

main :: proc() {
    

    file := string( #load( "input.txt" ) );
    entries : [dynamic]Entry;
    for line in strings.split( file, "\r\n" ) {
        if len( line ) == 0 { continue; }

        e : Entry = ---;
        splt := strings.split( line, " " );
        assert( len( splt ) == 3 );

        high_and_low := strings.split( splt[0], "-" );
        assert( len( high_and_low ) == 2 );
        ok := false;

        e.low, ok = strconv.parse_int( high_and_low[0] );
        assert( ok );

        e.high, ok = strconv.parse_int( high_and_low[1] );
        assert( ok );

        e.char = utf8.rune_at( splt[1], 0 );
        e.password = strings.clone( splt[2] );

        append( &entries, e );
    }

    // part 1
    valid_password_count := 0;
    for entry in entries {
        letter_count := 0;
        for c in entry.password {
            if c == entry.char { letter_count += 1; }
        }
        if entry.low <= letter_count && entry.high >= letter_count {
            valid_password_count += 1;
        }
    }
    fmt.println( "Valid password count found:", valid_password_count );

    // part 2
    valid_password_count = 0;
    for entry in entries {
        letter_count := 0;
        if xor( utf8.rune_at( entry.password, entry.low-1 ) == entry.char, utf8.rune_at( entry.password, entry.high-1 ) == entry.char ) {
            valid_password_count += 1;
        }
    }
    fmt.println( "Valid password count found:", valid_password_count );

}