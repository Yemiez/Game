#pragma once
#include "../formatting/utf.h"
#include "../tokenizing/token.h"
#include "../tokenizing/iterator.h"
#include <algorithm>
#include <unordered_set>
#include <fstream>

namespace osharp { namespace spelling {

	struct file_opt { };
	struct str_opt { };

	constexpr static file_opt FROM_FILE;
	constexpr static str_opt FROM_STRING;

	template<typename _Codec>
	class dictionary
	{
	public:
		using codec_cvt = _Codec;
		using string_type = typename codec_cvt::string_type;

	public:
		dictionary( string_type file, file_opt )
		{ 
			load_file( file );
		}

		dictionary( string_type str, str_opt )
		{ 
			load_string( str );
		}

		dictionary( )
		{ }

		void load_file( const string_type &file )
		{
			using value_type = typename string_type::value_type;
			return load_string( { std::istreambuf_iterator<value_type>( std::basic_ifstream<value_type>( file ) ),
			                      std::istreambuf_iterator<value_type>( ) } );
		}

		void load_string( const string_type &str )
		{
			tokenizing::code_iterator<codec_cvt> it{ str.begin( ), str.end( ) };
			tokenizing::alphabet<codec_cvt> alphabet;
			while ( it )
			{
				auto token = tokenizing::single_token<codec_cvt>::parse_single( it, alphabet );
				if ( token.is_identifier( ) )
					words_.insert( codec_cvt::to_lower( token.get_block( ) ) );
			}
		}

		void clear( )
		{
			words_.clear( );
		}

		const std::unordered_set<string_type> &get_words( ) const
		{
			return words_;
		}

		bool lookup( const string_type &word ) const
		{
			return words_.count( codec_cvt::to_lower( word ) ) != 0;
		}

	private:
		std::unordered_set<string_type> words_;
	};


} }