#pragma once
#include "../../formatting/utf.h"
#include <vector>
#include <unordered_map>

namespace osharp { namespace gui { namespace components {

	struct ColorRGBA
	{
		union
		{
			std::uint32_t color, 
				raw;
			struct
			{
				std::uint8_t B, G, R, A;
			};
		};

		ColorRGBA( std::uint32_t color )
			: color( color )
		{}
		ColorRGBA( std::uint8_t R, std::uint8_t G, std::uint8_t B )
			: R( R ), G( G ), B( B ), A( 255u )
		{}
		ColorRGBA( std::uint8_t R, std::uint8_t G, std::uint8_t B, std::uint8_t A )
			: R( R ), G( G ), B( B ), A( A )
		{}
		ColorRGBA( )
			: R( 0u ), G( 0u ), B( 0u ), A( 255u )
		{}
	};

	template<typename _Codec = cvt::codec_cvt<>>
	class color_palette
	{
	public:
		using codec = _Codec;
		using string_type = typename codec::string_type;
		using color_type = ColorRGBA;

	public:
		color_palette( string_type elem, color_type color )
			: colors_( { std::move( elem ), std::move( color ) } )
		{}
		color_palette( std::initializer_list<std::pair<string_type, color_type>> colors )
			: colors_( std::move( colors ) )
		{}
		color_palette( )
			: colors_( )
		{}

		void set_color( const string_type &elem, color_type color )
		{
			colors_[elem] = color;
		}

		const color_type &get_color( const string_type &elem ) const
		{
			return colors_[elem];
		}

	private:
		std::unordered_map<string_type, color_type> colors_;
	};

} } }