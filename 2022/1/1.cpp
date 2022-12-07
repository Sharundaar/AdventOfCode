#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "../basics/basics.h"
#include "../basics/basics.cpp"
#include "../basics/file_parser.h"

struct Elf
{
    int calories;
};

int main( int argc, char* argv[] )
{
    std::ifstream file( "input.txt" );
    defer{ file.close(); };

    if( file.is_open() )
    {
        std::string line;
        
        std::vector<Elf> elfs;

        int current_cal = 0;
        while( std::getline( file, line ) )
        {
            if( is_line_empty_or_whitespace( line.c_str() ) )
            {
                elfs.push_back({current_cal});
                current_cal = 0;
                continue;
            }

            int data;
            sscanf_s( line.c_str(), "%i", &data );
            current_cal += data;
        }

        std::sort(elfs.begin(), elfs.end(), [](const Elf& a, const Elf& b) { return a.calories > b.calories; });
        // Com_Println("Max calory count found for elf %d at %d.", max_elf, max_calory_count);
        int sum_3 = 0;
        for(int i=0; i<3; ++i)
        {
            sum_3 += elfs[i].calories;
        }
        Com_Println("Total calory for 3 biggest is %d.", sum_3);
    }

    return 1;
}