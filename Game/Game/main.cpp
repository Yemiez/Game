#include <ybf.h>
#include <osharp.h>
#include <thread>
#include <array>
#include <queue>
#include <thread>
#include <fstream>
#include "game/Game.h"

// good scaling for loading:
// 0.136974603

struct x{};

volatile static x y{};


struct Person
{
    std::string Name,
        Surname,
        City;

    Person( ) = default;
    Person( std::string name, std::string sur, std::string city )
        : Name( std::move( name ) ),
        Surname( std::move( sur ) ),
        City( std::move( city ) )
    { }
};

void main( )
{
    std::vector<int> integers;

    namespace cvt = osharp::cvt;
    namespace fmt = osharp::formats;
    namespace tok = osharp::tokenizing;
    using codec = cvt::codec_cvt<fmt::utf_8>;

    std::string file_contents{ "Donald Trump Washington Sand Nigger Egypt Literal Shit Toilet" };
    tok::code_iterator<codec> code_iterator{ file_contents.begin( ), file_contents.end( ) };

    std::vector<Person> people;
    while ( code_iterator )
    {
        auto token = tok::single_token<codec>::parse_single( code_iterator, tok::alphabet<codec>{ } );
        if ( token.is_identifier( ) ) // name
        {
            auto name = token.get_block( );
            token = tok::single_token<codec>::parse_single( code_iterator, tok::alphabet<codec>{} );
            if ( token.is_identifier( ) ) // surname
            {
                auto surname = token.get_block( );
                token = tok::single_token<codec>::parse_single( code_iterator, tok::alphabet<codec>{} );
                if ( token.is_identifier( ) ) // city
                {
                    auto city = token.get_block( );
                    people.emplace_back( std::move( name ), std::move( surname ), std::move( city ) );
                }
            }
        }
    }

    for ( auto &person : people )
        std::cout << "Person " << person.Name << " " << person.Surname << " is from " << person.City << '\n';
}

