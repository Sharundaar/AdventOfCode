#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

bool step( vector<int>& tape, int& pc )
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
            tape[param1] = get_input( tape );
            pc += 2;
        }
        break;
    case Opcode::OUTPUT:
        {
            int param1 = get_value( tape, tape[pc+1], modes[0] );
            output( tape, param1 );
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
        while( step( tape, pc ) );
    }

    return 0;
}