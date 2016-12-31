#pragma once
#include "../formatting/utf.h"
#include <array>


namespace osharp { namespace constexprs {

	namespace details
	{
		template<std::size_t N, typename _Codec = cvt::codec_cvt<>>
		struct string
		{
			using codec = _Codec;
			using string_type = typename codec::string_type;
			using value_type = typename string_type::value_type;
			
			constexpr value_type const *c_str( ) const
			{
				return elems_;
			}

			constexpr std::size_t size( ) const
			{
				return N;
			}

			constexpr std::size_t length( ) const
			{
				return N;
			}
			value_type elems_[N ? N : 1];
		};

		template<std::size_t...>
		struct index_tuple
		{ };

		template<std::size_t S, std::size_t F, typename accumulator = index_tuple<>, bool B = S >= F>
		struct index_range
		{
			using type = accumulator;
		};

		template<std::size_t S, std::size_t F, std::size_t... Indices>
		struct index_range<S, F, index_tuple<Indices...>, false>
			: index_range<S + 1, F, index_tuple<Indices..., S> >
		{ };

		template<typename _Codec, typename _Fmt, typename _Fmt2, std::size_t M, std::size_t N, std::size_t...Indices1, std::size_t... Indices2>
		constexpr string<sizeof...(Indices1) + sizeof...(Indices2) + 1, _Codec> strcat_impl( _Fmt const ( &a )[M], index_tuple<Indices1...>,
																		 _Fmt2 const ( &b )[N], index_tuple<Indices2...> )
		{
			return{ { _Codec::cvt( a[Indices1] )..., _Codec::cvt( b[Indices2] )..., _Codec::cvt( '\0' ) } };
		}

		template<typename _Codec, typename _Fmt, std::size_t N, std::size_t...Indices>
		constexpr string<N + 1, _Codec> to_str( _Fmt const ( &a )[N], index_tuple<Indices...> )
		{
			return{ { _Codec::cvt( a[Indices] )..., _Codec::cvt( '\0' ) } };
		}

		using u32 = std::uint32_t;
		using u64 = std::uint64_t;

		template<u32 S, u32 A = 16807UL, u32 C = 0UL, u32 M = ( 1UL << 31 ) - 1>
		struct LinearGenerator
		{
			static const u32 state = ( ( u64 ) S * A + C ) % M;
			static const u32 value = state;
			typedef LinearGenerator<state> next;
			struct Split
			{ // Leapfrog
				typedef LinearGenerator< state, A*A, 0, M> Gen1;
				typedef LinearGenerator<next::state, A*A, 0, M> Gen2;
			};
		};

		template<u32 S, std::size_t index, typename _Codec = cvt::codec_cvt<>>
			struct Generate
		{
			static const typename _Codec::output_format value = Generate<LinearGenerator<S>::state, index - 1>::value;
		};

		template<u32 S, typename _Codec>
		struct Generate<S, 0, _Codec>
		{
			static const typename _Codec::output_format value = _Codec::cvt( LinearGenerator<S>::value );
		};

		template<std::size_t...Indices>
		struct value_list
		{
		};

		template<typename TL, typename TR>
		struct Concat;

		template<std::size_t...SL, std::size_t...SR>
		struct Concat<value_list<SL...>, value_list<SR...>>
		{
			using type = value_list<SL..., SR...>;
		};

		template<std::size_t N>
		struct Count
		{
			using type = typename Concat<typename Count<N - 1>::type, value_list<N - 1>>::type;
		};

		template<>
		struct Count<0>
		{
			using type = value_list<>;
		};

		template<u32 seed, std::size_t index, std::size_t N, typename _Codec, typename T>
		static constexpr typename _Codec::output_format
			get_scrambled_utf_char( T const ( &a )[N] )
		{
			return _Codec::cvt( a[index] ) + _Codec::cvt( Generate<seed, index>::value );
		}

		template<u32 seed, typename T, typename _Codec = cvt::codec_cvt<>>
		struct cipher_helper;

		template<u32 seed, std::size_t... SL, typename _Codec>
		struct cipher_helper<seed, value_list<SL...>, _Codec>
		{
			template<typename T>
			static constexpr std::array<typename _Codec::output_format, sizeof...(SL)>
				get_array( T const ( &a )[sizeof...( SL )] )
			{
				return{ { get_scrambled_utf_char<seed, SL, sizeof...( SL ), _Codec>( a )... } };
			}
		};

		template<u32 seed, std::size_t N, typename _Codec = cvt::codec_cvt<>, typename T>
		static constexpr auto get_cipher_text( T const ( &a )[N] )
		{
			return cipher_helper<seed, typename Count<N>::type, _Codec>::get_array( a );
		}

		template<u32 seed, typename T, typename _Codec>
		struct noise_helper;

		template<u32 seed, std::size_t... SL, typename _Codec>
		struct noise_helper<seed, value_list<SL...>, _Codec>
		{
			static constexpr std::array<typename _Codec::output_format, sizeof...( SL )>
				get_array(  )
			{
				return{ { _Codec::cvt( Generate<seed, SL, _Codec>::value ) ...  } };
			}
		};

		template<u32 seed, std::size_t N, typename _Codec>
		constexpr std::array<typename _Codec::output_format, N> get_key( )
		{
			return noise_helper<seed, typename Count<N>::type, _Codec>::get_array( );
		}

		template<std::size_t seed, std::size_t N, typename _Codec = cvt::codec_cvt<>>
		class obfuscated_string
		{
		private:
			using fmt = typename _Codec::output_format;
			std::array<fmt, N> cipher_text_;
			std::array<fmt, N> key_;
		public:
			template<typename T>
			explicit constexpr obfuscated_string( T const ( &a )[N] )
				: cipher_text_( get_cipher_text<seed, N, _Codec>( a ) )
				, key_( get_key<seed, N, _Codec>( ) )
			{ }

			operator std::basic_string<fmt>( ) const
			{
				fmt plain_text[N];
				for ( volatile std::size_t i = 0; i < N; ++i )
				{
					volatile fmt temp = _Codec::cvt( cipher_text_[i] - key_[i] );
					plain_text[i] = temp;
				}
				return std::basic_string<fmt>{ plain_text, plain_text + N };
			}
			
			std::basic_string<fmt> str( ) const
			{
				return *this;
			}
		};

		struct seed_gen
		{
			static constexpr auto value = ( ( __TIME__[7] - '0' ) * 1 + ( __TIME__[6] - '0' ) * 10 + \
											( __TIME__[4] - '0' ) * 60 + ( __TIME__[3] - '0' ) * 600 + \
											( __TIME__[1] - '0' ) * 3600 + ( __TIME__[0] - '0' ) * 36000 ) + \
											( __LINE__ * 100000 );
		};

	}

	template<typename _Codec = cvt::codec_cvt<>, typename _Fmt, std::size_t N>
	constexpr auto to_str( _Fmt const ( &a )[N] )
	{
		return details::to_str<_Codec>( a, typename details::index_range<0, N - 1>::type( ) );
	}

	template<typename _Codec = cvt::codec_cvt<>, typename _Fmt, typename _Fmt2, std::size_t M, std::size_t N>
	constexpr auto str_cat( _Fmt const ( &a )[M],
						   _Fmt2 const ( &b )[N] )
	{
		return details::strcat_impl<_Codec>( a,
											 typename details::index_range<0, M - 1>::type( ),
											 b,
											 typename details::index_range<0, N - 1>::type( ) );
	}

	template<typename _Codec = osharp::cvt::codec_cvt<>,
		typename _Codec1,
		typename _Codec2,
		std::size_t N,
		std::size_t M>
		constexpr auto str_cat( const details::string<N, _Codec1> &a,
								const details::string<M, _Codec2> &b )
	{
		return details::strcat_impl<_Codec>( a.elems_,
											 typename details::index_range<0, N - 1>::type( ),
											 b.elems_,
											 typename details::index_range<0, M - 1>::type( ) );
	}

	template<typename _Codec = osharp::cvt::codec_cvt<>,
		typename _Codec1,
		std::size_t N,
		typename _Fmt2,
		std::size_t M>
		constexpr auto str_cat( const details::string<N, _Codec1> &a,
								_Fmt2 const ( &b )[M] )
	{
		return str_cat<_Codec>( a,
						        to_str<_Codec>( b ) );
	}

	template<typename _Codec = osharp::cvt::codec_cvt<>,
		typename _Fmt1,
		std::size_t N,
		typename _Codec2,
		std::size_t M>
		constexpr auto str_cat( _Fmt1 const ( &a )[N],
								const details::string<M, _Codec2> &b )
	{
		return str_cat<_Codec>( to_str<_Codec>( a ),
								b );
	}

	template<typename _Codec = osharp::cvt::codec_cvt<>, 
		typename _Fmt1, 
		std::size_t N>
		constexpr auto str_obf( _Fmt1 const ( &a )[N] )
	{
		return details::obfuscated_string<details::seed_gen::value, N, _Codec>{a};
	}
	
	template<typename _Codec = osharp::cvt::codec_cvt<>,
		typename _Codec1,
		std::size_t N>
		constexpr auto str_obf( const details::string<N, _Codec1> &a )
	{
		return details::obfuscated_string<details::seed_gen::value, N, _Codec>{a.elems_};
	}


}}

template<typename _Codec, typename _Fmt = typename _Codec::output_format, std::size_t N>
std::basic_ostream<_Fmt> &operator<<( std::basic_ostream<_Fmt> &ostr, const osharp::constexprs::details::string<N, _Codec> &str )
{
	return ostr << str.c_str( );
}

template<typename _Codec, typename _Fmt = typename _Codec::output_format, std::size_t N, std::size_t seed>
std::basic_ostream<_Fmt> &operator<<( std::basic_ostream<_Fmt> &ostr, const osharp::constexprs::details::obfuscated_string<seed, N, _Codec> &str )
{
	return ostr << static_cast<std::basic_string<_Fmt>>( str );
}