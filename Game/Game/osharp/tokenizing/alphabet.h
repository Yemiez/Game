#pragma once
#include "../standard_includes.h"

namespace osharp { namespace tokenizing {

	template<typename _Codec>
	class alphabet
	{
	public:
		using codec_cvt = _Codec;
		using string_type = typename codec_cvt::string_type;

	public:
		alphabet( )
		{ }
		virtual ~alphabet( )
		{ }

		virtual bool is_keyword( const string_type &key ) const
		{
			return false;
		}
	};

	template<typename _Codec = cvt::codec_cvt<>>
	class osharp_alphabet
		: public alphabet<_Codec>
	{
	public:
		using codec_cvt = _Codec;
		using base_type = alphabet<_Codec>;
		using string_type = typename base_type::string_type;

	public:
		osharp_alphabet( )
		{ }
		~osharp_alphabet( )
		{ }

		virtual bool is_keyword( const string_type &key ) const
		{
			static std::vector<string_type> keywords
			{
				{
					codec_cvt::cvt( "var" ),
					codec_cvt::cvt( "auto" ),
					codec_cvt::cvt( "dyn" ),
					codec_cvt::cvt( "public" ),
					codec_cvt::cvt( "private" ),
					codec_cvt::cvt( "const" ),
					codec_cvt::cvt( "volatile" ),
					codec_cvt::cvt( "group" ),
					codec_cvt::cvt( "method" ),
					codec_cvt::cvt( "if" ),
					codec_cvt::cvt( "else" ),
					codec_cvt::cvt( "while" ),
					codec_cvt::cvt( "continue" ),
					codec_cvt::cvt( "break" ),
					codec_cvt::cvt( "this" ),
					codec_cvt::cvt( "return" ),
					codec_cvt::cvt( "specialize" ),
					codec_cvt::cvt( "decltype" ),
					codec_cvt::cvt( "typeid" ),
                }
			};
			for ( auto &x : keywords )
			{
				if ( x == key )
				{
					return true;
				}
			}
			return false;
		}
	};


} }