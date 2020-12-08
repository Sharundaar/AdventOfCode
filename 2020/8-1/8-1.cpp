 #include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../basics/basics.h"
#include "../basics/basics.cpp"
#include "../basics/file_parser.h"

enum class InstructionType
{
    ERROR,
    NOP,
    ACC,
    JMP
};

struct Instruction
{
    InstructionType type;
    int param;
    bool has_been_run = false;
};

struct Processor
{
    int pc = 0;
    int acc = 0;
    std::vector<Instruction> instructions;
};

InstructionType instruction_str_to_type( const char* str )
{
    if( I_stricmp( "nop", str ) )
        return InstructionType::NOP;
    else if( I_stricmp( "acc", str ) )
        return InstructionType::ACC;
    else if( I_stricmp( "jmp", str ) )
        return InstructionType::JMP;
    return InstructionType::ERROR;
}




int main( int argc, char* argv[] )
{
    std::ifstream file( "input.txt" );
    defer{ file.close(); };
    
    // parse data
    if( file.is_open() )
    {
        Processor initial_processor;

        std::string line;
        std::string buff;
        while( std::getline( file, line ) )
        {
            if( is_line_empty_or_whitespace( line.c_str() ) || is_new_line( line[0] ) )
                continue;

            const char* it = line.c_str();
            while( *it != ' ' ) ++it;
            buff.assign( line.c_str(), it );
            int param;
            try_parse_to_int( it, param );
            auto type = instruction_str_to_type( buff.c_str() );
            assert( type != InstructionType::ERROR );
            initial_processor.instructions.push_back( { type, param } );
        }

        Processor copied_processor = initial_processor;
        int instruction_count = initial_processor.instructions.size();

        auto reset_processor = []( Processor& dst, const Processor& src )
        {
            memcpy( dst.instructions.data(), src.instructions.data(), src.instructions.size() * sizeof( Instruction ) );
            dst.acc = 0;
            dst.pc = 0;
        };

        for( int i=0; i<instruction_count; ++i )
        {
            if( copied_processor.instructions[i].type == InstructionType::ACC )
                continue;
            copied_processor.instructions[i].type = copied_processor.instructions[i].type == InstructionType::NOP ? InstructionType::JMP : InstructionType::NOP;

            bool terminated_correctly = false;
            while( true )
            {
                if( copied_processor.pc == instruction_count )
                {
                    terminated_correctly = true;
                    break;
                }
                if( copied_processor.pc < 0 || copied_processor.pc > instruction_count )
                    break;

                auto& instruction = copied_processor.instructions[copied_processor.pc];
                if( instruction.has_been_run )
                {
                    Com_Println( "Loop detected, acc %d.", copied_processor.acc );
                    break;
                }
                switch( instruction.type )
                {
                case InstructionType::NOP:
                    copied_processor.pc++;
                    break;
                case InstructionType::ACC:
                    copied_processor.acc += instruction.param;
                    copied_processor.pc++;
                    break;
                case InstructionType::JMP:
                    copied_processor.pc += instruction.param;
                    break;
                }
                instruction.has_been_run = true;
            }

            if( terminated_correctly )
            {
                Com_Println( "Terminated correctly by changing line %d, acc=%d", i, copied_processor.acc );
                break;
            }
            reset_processor( copied_processor, initial_processor );
        }
    }


    return 1;
}