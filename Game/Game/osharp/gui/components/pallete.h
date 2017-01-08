#pragma once
#include "../../formatting/utf.h"
#include <vector>
#include <unordered_map>

namespace osharp { namespace gui { namespace components {

	union Color
	{
		std::uint32_t color;
		std::uint8_t R, G, B, A;
	};

	template<typename _Codec = cvt::codec_cvt<>>
	class color_palette
	{
	public:
		using codec = _Codec;
		using string_type = typename codec::string_type;
		using color_type = std::uint32_t;

	public:
		color_palette( string_type name, color_type color )
			: colors_( { { std::move( name ), { color } } } )
		{}
		color_palette( string_type name, std::vector<color_type> colors )
			: colors_( { { std::move( name ), std::move( colors ) } } )
		{ }
		color_palette( std::map<string_type, std::vector<color_type>> colors )
			: colors_( std::move( colors ) )
		{ }

		color_type get_single( const string_type &elem ) const
		{
			return colors_.at( elem )[0];
		}

		const std::vector<color_type> &get( const string_type &elem ) const
		{
			return colors_.at( elem );
		}

		color_type get_single( const string_type &elem, const std::size_t &index ) const
		{
			return colors_.at( elem )[index];
		}

	private:
		std::unordered_map<string_type, std::vector<color_type>> colors_;
	};

} } }