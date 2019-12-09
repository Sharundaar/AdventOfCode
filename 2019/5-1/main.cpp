#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool step( vector<int>& tape, int& pc )
{
    int opcode = tape[pc];
    switch( opcode )
    {
    case 99:
        return false;
    
    case 1:
    {
        int sum = tape[tape[pc+1]] + tape[tape[pc+2]];
        tape[tape[pc+3]] = sum;
    }
        break;
    case 2:
        {
            int prod = tape[tape[pc+1]] * tape[tape[pc+2]];
            tape[tape[pc+3]] = prod;    
        }
        break;
    
    default:
        cout << "Something went wrong. (pc: " << pc << " opcode: " << opcode << ")" << endl;
        return false;
    }

    pc += 4;
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

        while( getline( file, line, ',' ) )
        {     
            int val = atoi( line.c_str() );
            tape.push_back( val );
        }

        vector<int> tape_saved = tape;

        for( int noun = 0; noun < 100; ++noun )
        {
            for( int verb = 0; verb < 100; ++verb )
            {
                tape = tape_saved;

                tape[1] = noun;
                tape[2] = verb;
                
                int pc = 0;
                while( step( tape, pc ) );

                int result = tape[0];
                if( result == 19690720 )
                {
                    cout << 100 * noun + verb << " (noun: " << noun << " verb: " << verb << ")" << endl;
                    return 0;
                }
            }
        }
    }

    return 0;
}