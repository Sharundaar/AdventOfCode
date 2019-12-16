#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "../helpers/types.h"

using namespace std;

enum class Opcode
{
    ADD = 1,
    MUL = 2,
    INPUT = 3,
    OUTPUT = 4,
    JMP_TRUE = 5,
    JMP_FALSE = 6,
    LT = 7,
    EQ = 8,
    EXIT = 99,
};

enum class ParamMode
{
    POSITION = 0,
    IMMEDIATE = 1,
};

#define MAX_PARAMS 3

int get_value( vector<int>& tape, int val, ParamMode mode )
{
    switch( mode )
    {
        case ParamMode::POSITION:
            return tape[val];
        case ParamMode::IMMEDIATE:
            return val;
    }

    return 0;
}

int get_input( vector<int>& tape )
{
    return 5;
}

void output( vector<int>& tape, int value )
{
    cout << value << endl;
}

template<typename InputFunc, typename OutputFunc>
bool step( vector<int>& tape, int& pc, InputFunc inputFunc, OutputFunc outputFunc )
{
    int code = tape[pc];
    Opcode opcode = static_cast<Opcode>( code - ( code / 100 ) * 100 );
    ParamMode modes[MAX_PARAMS];
    int params = code / 100;
    for( int i = MAX_PARAMS - 1; i >= 0; --i )
    {
        int powerOfTen = pow( 10, i );
        int mode = params / powerOfTen;
        modes[i] = static_cast<ParamMode>( mode );
        params -= mode * powerOfTen;
    }


    switch( opcode )
    {
    case Opcode::EXIT:
        return false;
    
    case Opcode::ADD:
        {
            int param1 = get_value( tape, tape[pc+1], modes[0] );
            int param2 = get_value( tape, tape[pc+2], modes[1] );
            int output_pos = tape[pc+3];
            int sum = param1 + param2;
            tape[output_pos] = sum;
            pc += 4;
        }
        break;
    case Opcode::MUL:
        {
            int param1 = get_value( tape, tape[pc+1], modes[0] );
            int param2 = get_value( tape, tape[pc+2], modes[1] );
            int output_pos = tape[pc+3];
            int prod = param1 * param2;
            tape[output_pos] = prod;
            pc += 4;
        }
        break;
    case Opcode::INPUT:
        {
            int param1 = get_value( tape, tape[pc+1], ParamMode::IMMEDIATE );
            tape[param1] = inputFunc( tape );
            pc += 2;
        }
        break;
    case Opcode::OUTPUT:
        {
            int param1 = get_value( tape, tape[pc+1], modes[0] );
            outputFunc( tape, param1 );
            pc += 2;
        }
        break;
    case Opcode::JMP_TRUE:
        {
            int param1 = get_value( tape, tape[pc+1], modes[0] );
            int param2 = get_value( tape, tape[pc+2], modes[1] );
            if( param1 != 0 )
                pc = param2;
            else
                pc += 3;
        }
        break;
    case Opcode::JMP_FALSE:
        {
            int param1 = get_value( tape, tape[pc+1], modes[0] );
            int param2 = get_value( tape, tape[pc+2], modes[1] );
            if( param1 == 0 )
                pc = param2;
            else
                pc += 3;
        }
        break;
    case Opcode::LT:
        {
            int param1 = get_value( tape, tape[pc+1], modes[0] );
            int param2 = get_value( tape, tape[pc+2], modes[1] );
            int output_pos = get_value( tape, tape[pc+3], ParamMode::IMMEDIATE );
            tape[output_pos] = param1 < param2 ? 1 : 0;
            pc += 4;
        }
        break;
    case Opcode::EQ:
        {
            int param1 = get_value( tape, tape[pc+1], modes[0] );
            int param2 = get_value( tape, tape[pc+2], modes[1] );
            int output_pos = get_value( tape, tape[pc+3], ParamMode::IMMEDIATE );
            tape[output_pos] = param1 == param2 ? 1 : 0;
            pc += 4;
        }
        break;

    default:
        cout << "Something went wrong. (pc: " << pc << " opcode: " << static_cast<int>( opcode ) << ")" << endl;
        return false;
    }

    return true;
}

void dump_tape( const vector<int>& tape )
{
    for( int i : tape )
    {
        cout << i << " ";
    }

    cout << endl;
}

int main()
{
    ifstream file( "input.txt" );
    if( file )
    {
        string line;
        vector<int> tape;
        tape.reserve( 10000 );

        while( getline( file, line, ',' ) )
        {     
            int val = atoi( line.c_str() );
            tape.push_back( val );
        }

        int pc = 0;
        vector<int> phases = { 5, 6, 7, 8, 9 };
        int AMP_COUNT = phases.size();
        auto next_phase = [AMP_COUNT]( vector<int> &phases ) {
            return next_permutation( phases.begin(), phases.end() );
        };
        auto output_phase = [&]( vector<int> &phases ) {
            cout << phases[0] << phases[1] << phases[2] << phases[3] << endl;
        };

        vector<int> max_phases;
        int max_amp_value = 0;
        while( true )
        {
            int next_amp_input = 0;
            int current_amp = 0;
            for( ;; )
            {
                vector<int> exec_tape( tape );
                int pc = 0;
                int input_count = 0;
                while( step( exec_tape, pc, [&]( vector<int>& tape ) -> int {
                    if( input_count == 0 )
                    {
                        input_count++;
                        return phases[current_amp];
                    }
                    else if( input_count == 1 )
                    {
                        input_count++;
                        return next_amp_input;
                    }
                    return -1;
                }, [&]( vector<int>& tape, int value ) -> void {
                    next_amp_input = value;
                } ) );

                current_amp = current_amp++ % AMP_COUNT;
            }

            if( next_amp_input > max_amp_value )
            {
                max_amp_value = next_amp_input;
                max_phases = phases;
            }
            if( !next_phase( phases ) ) break;
        }

        output_phase( max_phases );
        cout << max_amp_value << endl;
    }

    return 0;
}