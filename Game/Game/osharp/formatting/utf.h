#pragma once
#include <string>
#include <map>

namespace osharp { namespace formats {
	using utf_8 = char;
	using utf_32 = char32_t;
	using utf_16 = char16_t;
	using utf_w = wchar_t;
}}

namespace osharp { namespace cvt {

	namespace details
	{
		template<typename Fmt, typename T>
		struct to_str
		{
			std::basic_string<Fmt> operator()( const T &t )
			{
				return (std::basic_stringstream<Fmt>{} << t).str( );
			}
		};
	}

	template<typename T>
	struct is_string
	{
		static constexpr auto value = false;
	};

	template<typename T>
	struct is_string<std::basic_string<T>>
	{
		static constexpr auto value = true;
	};

	template<typename _Fmt_Out = formats::utf_8>
	class codec_cvt
	{
	public:
		using output_format = _Fmt_Out;
		using string_type = std::basic_string<output_format>;

	public:
		codec_cvt( ) = delete;

		template<typename _In,
			typename = typename std::enable_if<!is_string<_In>::value, void>::type>
		static constexpr auto cvt( const _In &input )
		{
			return static_cast< output_format >( input );
		}

		template<typename _In>
		static constexpr auto itoc( const _In &n )
		{
			return n >= 0 && n <= 9 ?
				cvt( cvt( '0' ) + n ) :
				cvt( '\0' );
		}

		template<typename _In, size_t size>
		static auto cvt( const _In( &input )[size] )
		{
			string_type output( size - 1, '\0' );
			for ( auto i = 0u;
				  i < size;
				  ++i )
				output[i] = cvt( input[i] );
			return output;
		}

		static auto cvt( const string_type &str )
		{
			return str;
		}

		static auto&& cvt( string_type &&str )
		{
			return str;
		}

		template<typename _In>
		static auto cvt( const std::basic_string<_In> &str )
		{
			string_type output( str.size( ), cvt( '\0' ) );
			for ( auto i = 0u;
				  i < str.size( );
				  ++i )
				output[i] = cvt( str[i] );
			return output;
		}

		static std::map<output_format, output_format> map_range( std::pair<output_format, output_format> p1,
																 std::pair<output_format, output_format> p2 )
		{
			if ( p2.second - p2.first != p1.second - p1.first )
				return{ };
			std::map<output_format, output_format> result;
			for ( auto i = cvt( 0 );
				  i <= ( p2.second - p2.first );
				  ++i )
			{
				result[p1.first + i] = p2.first + i;
				result[p2.first + i] = p2.first + i;
 			}
			return result;
		}

		template<typename _In>
		static constexpr bool is_space( const _In &fmt )
		{
			return cvt( fmt ) == cvt( ' ' ) ||
				cvt( fmt ) == cvt( '\t' );
		}

		template<typename _In>
		static constexpr bool is_eol( const _In &fmt,
									  const _In &eol = codec_cvt<_In>::cvt( '\n' ) )
		{
			return cvt( fmt ) == cvt( eol );
		}

		template<typename _In>
		static constexpr bool is_any_but_whitespace( const _In &fmt,
													 const _In &eol = codec_cvt<_In>::cvt( '\n' ) )
		{
			return !is_space( fmt ) && !is_eol( fmt, eol );
		}

		template<typename _In>
		static constexpr bool is_alpha( const _In &fmt )
		{
			return ( cvt( fmt ) >= cvt( 'a' ) && cvt( fmt ) <= cvt( 'z' ) ) ||
				( cvt( fmt ) >= cvt( 'A' ) && cvt( fmt ) <= cvt( 'Z' ) );
		}

		template<typename _In>
		static constexpr bool is_digit( const _In &fmt )
		{
			return cvt( fmt ) >= '0' && cvt( fmt ) <= '9';
		}

		template<typename _In>
		static constexpr bool is_xdigit( const _In &fmt )
		{
			return ( cvt( fmt ) >= cvt( 'a' ) && cvt( fmt ) <= cvt( 'f' ) ) ||
				( cvt( fmt ) >= cvt( 'A' ) && cvt( fmt ) <= cvt( 'F' ) ) ||
				is_digit( fmt );
		}

		template<typename _In>
		static constexpr bool is_alnum( const _In &fmt )
		{
			return is_digit( fmt ) || is_alpha( fmt );
		}
		
		template<typename _In>
		static auto to_lower( const _In &fmt )
		{
			auto found = upper_to_lower_map.find( cvt( fmt ) );
			return found != upper_to_lower_map.end( ) ?
				found->second :
				cvt( fmt );
		}

		template<typename _In>
		static auto to_upper( const _In &fmt )
		{
			auto found = lower_to_upper_map.find( cvt( fmt ) );
			return found != lower_to_upper_map.end( ) ?
				found->second :
				cvt( fmt );
		}

		static string_type to_upper( const string_type &fmt )
		{
			string_type result( fmt.size( ), '\0' );
			for ( auto i = 0u;
				  i < fmt.size( );
				  ++i )
				result[i] = to_upper( fmt[i] );
			return result;
		}

		static string_type to_lower( const string_type &fmt )
		{
			string_type result( fmt.size( ), '\0' );
			for ( auto i = 0u;
				  i < fmt.size( );
				  ++i )
				result[i] = to_lower( fmt[i] );
			return result;
		}

		template<typename T>
		static string_type to_string( const T &t )
		{
			return details::to_str<output_format, T>()( t );
		}

	private:
		static const std::map<output_format, output_format> lower_to_upper_map;
		static const std::map<output_format, output_format> upper_to_lower_map;

	};

	template<typename _Fmt>
	const std::map<typename codec_cvt<_Fmt>::output_format, typename codec_cvt<_Fmt>::output_format> 
		codec_cvt<_Fmt>::lower_to_upper_map = codec_cvt<_Fmt>::map_range( { codec_cvt<_Fmt>::cvt( 'a' ), codec_cvt<_Fmt>::cvt( 'z' ) }, 
		                                      { codec_cvt<_Fmt>::cvt( 'A' ), codec_cvt<_Fmt>::cvt( 'Z' ) } );

	template<typename _Fmt>
	const std::map<typename codec_cvt<_Fmt>::output_format, typename codec_cvt<_Fmt>::output_format>
		codec_cvt<_Fmt>::upper_to_lower_map = codec_cvt<_Fmt>::map_range( { codec_cvt<_Fmt>::cvt( 'A' ), codec_cvt<_Fmt>::cvt( 'Z' ) }, 
		                                      { codec_cvt<_Fmt>::cvt( 'a' ), codec_cvt<_Fmt>::cvt( 'z' ) } );

} }

