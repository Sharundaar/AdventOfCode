package main
import "core:fmt"
import "core:strings"
import "core:slice"
import "core:strconv"

input :: string(#load( "input.txt" ))

Board :: struct {
    grid: [5][5]u8,
    marked: [5][5]bool,
    won: bool,
}

is_winning_board :: proc( using board: ^Board ) -> bool {
    for x := 0; x < 5; x += 1 {
        column_wins := true
        for y := 0; y < 5; y += 1 { // check columns
            if !marked[y][x] {
                column_wins = false
                break
            }
        }
        if column_wins do return true
    }

    for y := 0; y < 5; y += 1 { // check columns
        row_wins := true
        for x := 0; x < 5; x += 1 {
            if !marked[y][x] {
                row_wins = false
                break
            }
        }
        if row_wins do return true
    }

    return false
}

compute_score :: proc( using board: ^Board ) -> int {
    score : int
    for x := 0; x < 5; x += 1 {
        for y := 0; y < 5; y += 1 {
            if !marked[y][x] {
                score += int( grid[y][x] )
            }
        }
    }
    return score
}

parse_boards :: proc( lines: []string ) -> (boards: [dynamic]Board) {
    line_idx := 1
    for line_idx := 1; line_idx < len(lines); line_idx += 1 {
        if len( lines[line_idx] ) <= 1 do continue
        board := Board{}
        for i := 0 ; i<5; i += 1 {
            line := lines[line_idx + i]
            nums := strings.fields( line ) ; defer delete( nums )
            for num, k in &nums {
                board.grid[i][k] = atou8( num )
            }
        }
        line_idx += 5
        append( &boards, board )
    }

    return
}

atou8 :: proc( s: string ) -> u8 {
    return u8( strconv.atoi( s ) )
}

part1 :: proc() {
    using strconv
    lines := strings.split(input, "\n") ; defer delete( lines )

    numbers := slice.mapper( strings.split( lines[0], ","), atou8 )
    boards: [dynamic]Board = parse_boards( lines )
    for number in numbers {
        for board in &boards {
            main: for y := 0; y < 5; y += 1 {
                for x := 0; x < 5; x += 1 {
                    if board.grid[y][x] == number {
                        board.marked[y][x] = true
                        break main
                    }
                }
            }

            if is_winning_board( &board ) {
                score := compute_score( &board )
                fmt.println( score * int( number ) )
                return
            }
        }
    }

    fmt.println( "No winner found" )
}

part2 :: proc() {
    using strconv
    lines := strings.split(input, "\n")

    numbers := slice.mapper( strings.split( lines[0], ","), atou8 )
    boards := parse_boards( lines )

    board_won_count : int
    for number in numbers {
        for board in &boards {
            if board.won do continue

            main: for y := 0; y < 5; y += 1 {
                for x := 0; x < 5; x += 1 {
                    if board.grid[y][x] == number {
                        board.marked[y][x] = true
                        break main
                    }
                }
            }

            if is_winning_board( &board ) {
                board.won = true
                board_won_count += 1
                if board_won_count == len( boards ) {
                    score := compute_score( &board )
                    fmt.println( score * int( number ) )
                    return
                }
            }
        }
    }

    fmt.println( "No last winner found" )
}

main :: proc() {
    part1()
    part2()
}