#pragma once
#include "../standard_includes.h"

namespace osharp { namespace tokenizing {

	template<typename _Codec = cvt::codec_cvt<>>
	class code_iterator
	{
	public:
		using codec_cvt = _Codec;
		using string_type = typename codec_cvt::string_type;
		using fmt_type = typename codec_cvt::output_format;
		using self_type = code_iterator<codec_cvt>;
		using const_iterator = typename string_type::const_iterator;

	public:
		code_iterator( const_iterator begin, 
					   const_iterator end,
					   fmt_type eol = codec_cvt::cvt( '\n' ) )
			: it_( begin ), 
			end_( end ),
			row_( 0 ),
			column_( 0 )
		{
			if ( !codec_cvt::is_any_but_whitespace( *it_ ) )
				increment_self( );
		}

		int get_row( ) const
		{
			return row_;
		}

		int get_column( ) const
		{
			return column_;
		}

	public:
		bool operator<( const self_type &other ) const
		{
			return it_ < other.it_;
		}

		bool operator>( const self_type &other ) const
		{
			return it_ > other.it_;
		}

		bool operator==( const self_type &other ) const
		{
			return it_ == other.it_;
		}

		bool operator!=( const self_type &other ) const
		{
			return it_ != other.it_;
		}

		string_type as_block( ) const
		{
			string_type out;
			auto tmp = it_;
			while ( tmp < end_ && codec_cvt::is_any_but_whitespace( *tmp ) )
				out.push_back( *(tmp++) );
			return out;
		}

		void move_to_next( )
		{
			while ( it_ < end_ )
			{
				inc( );
				if ( !( it_ < end_ ) || codec_cvt::is_any_but_whitespace( *it_ ) )
					break;
			}
		}

		self_type operator+( size_t amount ) const
		{
			self_type tmp = *this;
			for ( auto i = 0u;
				  i < amount && tmp;
				  ++i )
				++tmp;
			return tmp;
		}

		self_type operator+( int amount ) const
		{
			self_type tmp = *this;
			for ( auto i = 0;
				  i < amount && tmp;
				  ++i )
				++tmp;
			return tmp;
		}

		self_type operator++( int )
		{
			self_type tmp = *this;
			increment_self( );
			return tmp;
		}

		self_type &operator++( )
		{
			return increment_self( );
		}

		self_type &pre_increment( )
		{
			if ( it_ < end_ )
				inc( );
			return *this;
		}

		self_type post_increment( )
		{
			auto tmp = *this;
			if ( it_ < end_ )
				inc( );
			return tmp;
		}

		auto& operator*( )
		{
			return *it_;
		}

		const auto& operator*( ) const
		{
			return *it_;
		}

		operator bool( )
		{
			return it_ < end_;
		}

	private:
		self_type& increment_self( )
		{
			while ( it_ < end_ )
			{
				inc( );
				if ( !( it_ < end_ ) || codec_cvt::is_any_but_whitespace( *it_ ) )
					break;
			}
			return *this;
		}

		void inc( )
		{
			if ( codec_cvt::is_eol( *it_ ) )
				++row_, column_ = 1;
			else
				++column_;
			++it_;
		}

	private:
		const_iterator it_, end_;
		int row_, column_;
		fmt_type eol_;
	};

} }