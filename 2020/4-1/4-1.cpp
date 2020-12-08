 #include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../basics/basics.h"
#include "../basics/basics.cpp"
#include "../basics/file_parser.h"

struct Field
{
    bool present;
    union
    {
        int vi;
        uint vui;
        char vstr[12];
    } content;
};

struct PassportData
{
    Field byr; Field iyr; Field eyr;
    Field hgt; char hgt_unit[4];
    Field hcl; Field ecl;
    Field pid; Field cid;
};

int main( int argc, char* argv[] )
{
    std::ifstream file( "4-1/input.txt" );
    defer{ file.close(); };

    constexpr int height = 323;
    constexpr int width = 31;
    std::vector<PassportData> datas;

    // parse data
    if( file.is_open() )
    {
        std::string line;
        PassportData currentData;
        memset( &currentData, 0, sizeof( PassportData ) );

        while( std::getline( file, line ) )
        {
            if( is_line_empty_or_whitespace( line.c_str() ) || is_new_line( *line.c_str() ) )
            {
                datas.push_back( currentData );
                memset( &currentData, 0, sizeof( PassportData ) );
                continue;
            }

            char data[256];
            data[0] = '\0';
            char field[4];
            field[3] = '\0';
            const char* it = line.c_str();
            while( !is_eof_or_new_line( *it ) )
            {
                it = chomp_empty_space( it );
                field[0] = *(it++);
                field[1] = *(it++);
                field[2] = *(it++);
                assert( *it == ':' );
                ++it; // slip ':'
                if( I_strcmp( field, "byr" ) )
                {
                    currentData.byr.present = true;
                    try_parse_to_int( it, currentData.byr.content.vi );
                }
                else if( I_strcmp( field, "iyr" ) )
                {
                    currentData.iyr.present = true;
                    try_parse_to_int( it, currentData.iyr.content.vi );
                }
                else if( I_strcmp( field, "eyr" ) )
                {
                    currentData.eyr.present = true;
                    try_parse_to_int( it, currentData.eyr.content.vi );
                }
                else if( I_strcmp( field, "hgt" ) )
                {
                    currentData.hgt.present = true;
                    try_parse_to_int( it, currentData.hgt.content.vi );
                    while( is_number( *it ) ) ++it;
                    char* hgt_unit_it = currentData.hgt_unit;
                    while( !( is_eof_or_new_line( *it ) || is_empty_space( *it ) ) )
                        *(hgt_unit_it++) = *(it++);
                    *hgt_unit_it = '\0';
                }
                else if( I_strcmp( field, "hcl" ) )
                {
                    currentData.hcl.present = true;
                    char* hcl_it = currentData.hcl.content.vstr;
                    while( !( is_eof_or_new_line( *it ) || is_empty_space( *it ) ) )
                        *(hcl_it++) = *(it++);
                    *hcl_it = '\0';
                }
                else if( I_strcmp( field, "ecl" ) )
                {
                    currentData.ecl.present = true;
                    char* ecl_it = currentData.ecl.content.vstr;
                    while( !( is_eof_or_new_line( *it ) || is_empty_space( *it ) ) )
                        *(ecl_it++) = *(it++);
                    *ecl_it = '\0';
                }
                else if( I_strcmp( field, "pid" ) )
                {
                    currentData.pid.present = true;
                    char* pid_it = currentData.pid.content.vstr;
                    while( !( is_eof_or_new_line( *it ) || is_empty_space( *it ) ) && ( pid_it - currentData.pid.content.vstr ) <= 9 )
                        *(pid_it++) = *(it++);
                    *pid_it = '\0';
                }
                else if( I_strcmp( field, "cid" ) )
                {
                    currentData.cid.present = true;
                    try_parse_to_uint( it, currentData.cid.content.vui );
                }

                while( !( is_empty_space( *it ) || is_eof_or_new_line( *it ) ) ) ++it;
            }
        }

        // stupid patching code cause i miss the last entry...
        if( currentData.byr.present || currentData.iyr.present || currentData.eyr.present
            || currentData.hgt.present || currentData.hcl.present || currentData.ecl.present
            || currentData.pid.present || currentData.cid.present )
        {
            datas.push_back( currentData );
        }
    }

    // check validity
    int passport_valid_count = 0;
    std::vector<PassportData> invalid_passports;
    #define VALIDATE( cond ) if( !( cond ) ) { invalid_passports.push_back( data ); continue; }
    const char* valid_eye_colors = "amb blu brn gry grn hzl oth";
    for( const PassportData& data : datas )
    {
        if( data.byr.present && data.iyr.present && data.eyr.present
            && data.hgt.present && data.hcl.present && data.ecl.present
            && data.pid.present )
        {
            VALIDATE( data.byr.content.vi >= 1920 && data.byr.content.vi <= 2002 );
            VALIDATE( data.iyr.content.vi >= 2010 && data.iyr.content.vi <= 2020 );
            VALIDATE( data.eyr.content.vi >= 2020 && data.eyr.content.vi <= 2030 );
            VALIDATE( 
                   ( I_stricmp( data.hgt_unit, "cm" ) && data.hgt.content.vi >=  150 && data.hgt.content.vi <= 193 )
                || ( I_stricmp( data.hgt_unit, "in" ) && data.hgt.content.vi >=  59 && data.hgt.content.vi <= 76 )
            );
            
            VALIDATE( data.hcl.content.vstr[0] == '#' 
                && is_hex_char( data.hcl.content.vstr[1] )
                && is_hex_char( data.hcl.content.vstr[2] )
                && is_hex_char( data.hcl.content.vstr[3] )
                && is_hex_char( data.hcl.content.vstr[4] )
                && is_hex_char( data.hcl.content.vstr[5] )
                && is_hex_char( data.hcl.content.vstr[6] ) );

            VALIDATE( I_strispresent( valid_eye_colors, data.ecl.content.vstr ) );
            VALIDATE( 
                   is_number( data.pid.content.vstr[0] ) 
                && is_number( data.pid.content.vstr[1] )
                && is_number( data.pid.content.vstr[2] )
                && is_number( data.pid.content.vstr[3] )
                && is_number( data.pid.content.vstr[4] )
                && is_number( data.pid.content.vstr[5] )
                && is_number( data.pid.content.vstr[6] )
                && is_number( data.pid.content.vstr[7] )
                && is_number( data.pid.content.vstr[8] )
                && is_eof( data.pid.content.vstr[9] )
                );
            
            passport_valid_count++;
        }
        else
            VALIDATE( false );

    }
    Com_Println( "Amount of valid passport: %i.", passport_valid_count );

    return 1;
}