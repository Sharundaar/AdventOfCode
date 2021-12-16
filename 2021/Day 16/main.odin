package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"
import "core:math"
import "core:os"
import "core:intrinsics"

input :: string(#load( "input.txt" ))

Packet :: struct {
    ver: u8,
    tp: u8,
    lit: int, // valid if tp == 4
    subpackets: [dynamic]Packet, // valid if tp != 4
}

to_num :: proc( c: u8 ) -> (u8, bool) {
    switch c {
        case '0'..'9': return u8( c - '0' ), true
        case 'A'..'F': return u8(10) + ( c - 'A' ), true
    }

    return 0, false
}

// returns 0 or 1 depending on the bit found at offset
get_bit :: proc( offset: int, msg: []u8 ) -> (u8, bool) {
    idx := offset / 4
    if idx >= len( msg ) do return 0, false
    n, ok := to_num( msg[idx] )
    if !ok do return 0, false
    return 1 if (n & ( 1 << (3 - u64(offset % 4) ))) > 0 else 0, true
}

// returns the version + the start of the next element
extract_value :: proc( from: int, size: int, msg: []u8 ) -> ( value: int, end: int ) {
    size := size - 1
    val := 0
    end = from
    for size >= 0 {
        b, ok := get_bit( end, msg ) ; assert( ok )
        val += int(int(b) << u64(size))
        end += 1
        size -= 1
    }
    value = val
    return value, end
}

extract_value_u8 :: proc( from: int, size: int, msg: []u8 ) -> ( value: u8, end: int ) {
    assert( size <= 8 )
    v, e := extract_value( from, size, msg )
    value = u8(v)
    end = e
    return
}

align_on_4bit :: proc( index: int ) -> int {
    if index % 4 == 0 do return index
    if index % 4 == 1 do return index + 3
    if index % 4 == 2 do return index + 2
    if index % 4 == 3 do return index + 1
    unreachable()
}

parse_packet :: proc( from: int, msg: []u8, count : int = 0 ) -> (packet: Packet, end_ret: int) {
    ver, tp : u8
    end : int
    ver, end = extract_value_u8( from, 3, msg )
    tp, end = extract_value_u8( end, 3, msg )

    packet.ver, packet.tp = ver, tp
    switch packet.tp {
        case 4:
            for {
                p, num : int
                p, end = extract_value( end, 1, msg )
                num, end = extract_value( end, 4, msg )
                packet.lit = packet.lit << 4
                packet.lit += num

                if p == 0 {
                    break
                } 
            }
            fmt.println( packet.ver, packet.tp, packet.lit, count )
        case:
            length_type : u8
            length_type, end = extract_value_u8( end, 1, msg )
            switch length_type {
                case 0:
                    length_subpackets : int
                    length_subpackets, end = extract_value( end, 15, msg )
                    fmt.println( packet.ver, packet.tp, length_type, length_subpackets, count )
                    total_read : int
                    for total_read < length_subpackets {
                        subpacket, end2 := parse_packet( end + total_read, msg, count + 1 )
                        total_read = end2 - end
                        append( &packet.subpackets, subpacket )
                    }
                    assert( total_read == length_subpackets )
                    end += total_read
                case 1:
                    num_subpackets : int
                    num_subpackets, end = extract_value( end, 11, msg )
                    fmt.println( packet.ver, packet.tp, length_type, num_subpackets, count )
                    total_read : int
                    for total_read < num_subpackets {
                        subpacket : Packet
                        subpacket, end = parse_packet( end, msg, count + 1  )
                        total_read += 1
                        append( &packet.subpackets, subpacket )
                    }
                    assert( total_read == num_subpackets )
                case: unreachable()
            }
    }
    
    end_ret = end
    return
}

sum_versions :: proc( packet: ^Packet ) -> int {
    sum : int = int( packet.ver )
    for p in &packet.subpackets {
        sum += sum_versions( &p )
    }
    return sum
}

part1 :: proc() {
    fmt.println("==== Part 1 Begin ====")
    lines := strings.split(input, "\r\n")

    // skipping first 2 zeroes
    msg := transmute([]u8) lines[0][:]

    it := 0
    packet: Packet
    packet, it = parse_packet( it, msg )
    fmt.println( packet )
    fmt.println( sum_versions( &packet ) )


    /*
    packets: [dynamic]Packet
    i := 0
    for i < len(msg) {
        packet: Packet
        c1, c2 := msg[i], msg[i+1]
        n1, n2 := to_num(c1), to_num(c2)
        ver := n1

    }
    */
    
    fmt.println("==== Part 1 End ====")
}

nmin :: proc( a, b: int ) -> int do return a if a < b else b
nmax :: proc( a, b: int ) -> int do return a if a > b else b

resolve_packet :: proc( packet: ^Packet ) -> int {
    res : int

    switch packet.tp {
        case 4:
            res = packet.lit
        case 0:
            for p in &packet.subpackets do res += resolve_packet( &p )
        case 1:
            res = 1
            for p in &packet.subpackets do res *= resolve_packet( &p )
        case 2:
            res = max(int)
            for p in &packet.subpackets do res = nmin( res, resolve_packet( &p ) )
        case 3:
            for p in &packet.subpackets do res = nmax( res, resolve_packet( &p ) )
        case 5:
            if resolve_packet( &packet.subpackets[0] ) > resolve_packet( &packet.subpackets[1] ) do res = 1
        case 6:
            if resolve_packet( &packet.subpackets[0] ) < resolve_packet( &packet.subpackets[1] ) do res = 1
        case 7:
            if resolve_packet( &packet.subpackets[0] ) == resolve_packet( &packet.subpackets[1] ) do res = 1
    }
    return res
}

part2 :: proc() {
    fmt.println("==== Part 2 Begin ====")
    lines := strings.split(input, "\r\n")

    msg := transmute([]u8) lines[0][:]
    it := 0
    packet: Packet
    packet, it = parse_packet( it, msg )
    fmt.println( resolve_packet( &packet ) )


    fmt.println("==== Part 2 End ====")
}

main :: proc() {
    part1()
    part2()
}