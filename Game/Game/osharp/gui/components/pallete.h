#pragma once
#include "../../formatting/utf.h"
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

		struct color_type
		{
			color_type( std::initializer_list<std::pair<string_type, ColorRGBA>> colors )
				: color_range_( std::move( colors ) )
			{}
			color_type( std::unordered_map<string_type, ColorRGBA> color )
				: color_range_( std::move( color ) )
			{}
			color_type( )
				: color_range_( )
			{}

			ColorRGBA &operator[]( const string_type &index )
			{
				return color_range_[index];
			}

			std::unordered_map<string_type, ColorRGBA> color_range_;
		};

	public:
		color_palette( std::unordered_map<string_type, color_type> color )
			: colors_( std::move( colors_ ) )
		{}
		color_palette( )
		{}

		void set_color( const string_type &elem, const string_type &index, ColorRGBA color )
		{
			colors_[elem][index] = std::move( color );
		}

		void set_color( const string_type &elem, color_type range )
		{
			colors_[elem] = std::move( range );
		}

		const auto &get_color( const string_type &elem, const string_type &index ) const
		{
			return colors_[elem][index];
		}

		const auto &get_color( const string_type &elem ) const
		{
			return colors_[elem];
		}

		auto get_or_default( const string_type &elem, const string_type &index, ColorRGBA color ) const
		{
			if ( colors_.count( elem ) )
			{
				auto &range = colors_.at( elem );
				if ( range.color_range_.count( index ) )
					return range.color_range_.at( index );
			}
			return std::move( color );
		}

		auto get_or_default( const string_type &elem, color_type color ) const
		{
			if ( colors_.count( elem ) )
				return colors_[elem];
			return std::move( color );
		}

	private:
		std::unordered_map<string_type, color_type> colors_;
	};

} } }