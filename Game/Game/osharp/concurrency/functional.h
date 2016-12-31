#pragma once
#include <functional>
#include <vector>
#include "../formatting/utf.h"

namespace osharp { namespace concurrency {

	template<typename _Fty>
	struct callable_traits;

	template<typename Result, typename...Arguments>
	struct callable_traits<Result( Arguments... )>
	{
		using result_type = Result;
		typedef result_type( signature_type )( Arguments... );
		typedef result_type( *function_type )( Arguments... );
		static constexpr auto is_void = std::is_void<Result>::value;
		static constexpr auto is_const = false;
		static constexpr auto is_ptmf = false;
		static constexpr auto args_size = sizeof...( Arguments );

		template<std::size_t at>
		struct get_arg_t
		{
			using type = typename std::tuple_element<at, std::tuple<Arguments...>>::type;
		};
	};

	template<typename Result, typename...Arguments>
	struct callable_traits<Result( Arguments... ) const>
	{
		using result_type = Result;
		typedef result_type( signature_type )( Arguments... );
		typedef result_type( *function_type )( Arguments... );
		static const constexpr auto is_void = std::is_void<Result>::value;
		static const constexpr auto is_const = true;
		static const constexpr auto is_ptmf = false;
		static const constexpr auto args_size = sizeof...( Arguments );

		template<std::size_t at>
		struct get_arg_t
		{
			using type = typename std::tuple_element<at, std::tuple<Arguments...>>::type;
		};
	};

	template<typename Result, typename T, typename...Arguments>
	struct callable_traits<Result( T::* )( Arguments... )>
	{
		using result_type = Result;
		typedef result_type( signature_type )( Arguments... );
		typedef result_type( T::*function_type )( Arguments... );
		static const constexpr auto is_void = std::is_void<Result>::value;
		static const constexpr auto is_const = false;
		static const constexpr auto is_ptmf = true;
		static const constexpr auto args_size = sizeof...( Arguments );

		template<std::size_t at>
		struct get_arg_t
		{
			using type = typename std::tuple_element<at, std::tuple<Arguments...>>::type;
		};
	};

	template<typename Result, typename T, typename...Arguments>
	struct callable_traits<Result( T::* )( Arguments... ) const>
	{
		using result_type = Result;
		typedef result_type( signature_type )( Arguments... );
		typedef result_type( T::*function_type )( Arguments... ) const;
		static const constexpr auto is_void = std::is_void<Result>::value;
		static const constexpr auto is_const = true;
		static const constexpr auto is_ptmf = true;
		static const constexpr auto args_size = sizeof...( Arguments );

		template<std::size_t at>
		struct get_arg_t
		{
			using type = typename std::tuple_element<at, std::tuple<Arguments...>>::type;
		};
	};

	template<typename _Fty, typename _Codec = cvt::codec_cvt<>>
	class event_proxy
	{
	public:
		using signature_type = _Fty;
		using codec_cvt = _Codec;
		using self_type = event_proxy<signature_type, codec_cvt>;
		using callable_type = callable_traits<signature_type>;
		using string_type = typename codec_cvt::string_type;
		using hasher = std::hash<string_type>;

	public:
		template<typename _Fn>
		event_proxy( const string_type &str, _Fn &&fn )
			: hash_( hasher( )( str ) ),
			fn_( std::forward<_Fn>( fn ) )
		{}

		template<typename _Fn>
		event_proxy( _Fn &&fn )
			: hash_( static_cast<std::size_t>( -1 ) ),
			fn_( std::forward<_Fn>( fn ) )
		{}

		event_proxy( )
			: hash_( static_cast<std::size_t>( -1 ) ),
			fn_( nullptr )
		{ }

		auto 
			equals( const string_type &str ) const
		{
			return hash_ == hasher( )( str );
		}

		auto
			equals( const std::size_t &hash ) const
		{
			return hash_ == hash;
		}

		const auto
			&get_target( ) const
		{
			return fn_;
		}

		auto
			&get_target( )
		{
			return fn_;
		}

	public:
		operator bool( ) const
		{
			return fn_.operator bool( );
		}

		template<typename...Arguments>
		auto operator()( Arguments&&...arguments ) const
			-> typename callable_type::result_type
		{
			if ( *this )
				return fn_( std::forward<Arguments>( arguments )... );
			return typename callable_type::result_type( );
		}

	private:
		std::size_t hash_;
		std::function<signature_type> fn_;
	};

	template<typename _Fty, typename _Codec = cvt::codec_cvt<>>
	class event
	{
	public:
		using signature_type = _Fty;
		using codec_cvt = _Codec;
		using string_type = typename codec_cvt::string_type;
		using callable_type = callable_traits<signature_type>;
		using self_type = event<signature_type, codec_cvt>;
		using proxy = event_proxy<signature_type, codec_cvt>;

	public:
		event( )
			: proxies_( )
		{ }

		std::size_t get_number_of_proxies( ) const
		{
			return proxies_.size( );
		}

		const std::vector<proxy> &get_proxies( ) const
		{
			return proxies_;
		}

		std::vector<proxy> &get_proxies( )
		{
			return proxies_;
		}

		const proxy &get_proxy( const std::size_t &index ) const
		{
			return proxies_[index];
		}

		proxy &get_proxy( const std::size_t &index )
		{
			return proxies_[index];
		}

	public:
		template<typename _Fn>
		self_type &operator+=( _Fn &&fn )
		{
			proxies_.emplace_back( std::forward<_Fn>( fn ) );
			return *this;
		}

		self_type &operator+=( proxy fn )
		{
			proxies_.emplace_back( std::move( fn ) );
			return *this;
		}

		template<typename _Fn>
		self_type operator+( _Fn &&fn ) const
		{
			return self_type( *this ) += std::forward<_Fn>( fn );
		}

		self_type operator+( proxy fn ) const
		{
			return self_type( *this ) += std::move( fn );
		}

		self_type &operator-=( const string_type &str )
		{
			auto hash = typename proxy::hasher( )( str );
			for ( auto it = proxies_.begin( ), end = proxies_.end( );
				  it < end; )
			{
				if ( it->equals( hash ) )
				{
					it = proxies_.erase( it );
					end = proxies_.end( );
				}
				else
					++it;
			}
			return *this;
		}

		self_type operator-( const string_type &str ) const
		{
			return self_type( *this ) -= str;
		}

		template<typename...Arguments>
		const self_type &operator()( Arguments&&...args ) const
		{
			for ( auto &x : proxies_ )
				x( std::forward<Arguments>( args )... );
			return *this;
		}

	private:
		std::vector<proxy> proxies_;
	};

} }