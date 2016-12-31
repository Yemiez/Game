#pragma once
#include "alphabet.h"

namespace osharp { namespace tokenizing {

	enum token_types_t
	{
		token_t_identifier,
		token_t_string_literal,
		token_t_operand,
		token_t_binary_operator,
		token_t_logical_operator,
		token_t_bracket_type,
		token_t_arrow_access,
		token_t_dot_access,
		token_t_scope_resolution,
		token_t_comma,
		token_t_semicolon,
		token_t_keyword,
		token_t_other,
		token_t_none,
	};

	template<typename _Codec = cvt::codec_cvt<>>
	class single_token
	{
	public:
		using codec_cvt = _Codec;
		using utf_type = typename codec_cvt::output_format;
		using string_type = typename codec_cvt::string_type;

	public:
		single_token( string_type block, int row, int col, token_types_t type, int presedence = 0 )
			: row_( row ), 
			column_( col ),
			block_( std::move( block ) ),
			type_( type ),
			presedence_( presedence )
		{ }
		single_token( )
			: type_( token_t_none )
		{ }

		int get_row( ) const
		{
			return row_;
		}

		int get_column( ) const
		{
			return column_;
		}

		int get_presedence( ) const
		{
			return presedence_;
		}

		const string_type &get_block( ) const
		{
			return block_;
		}

		token_types_t get_type( ) const
		{
			return type_;
		}

		bool is_identifier( ) const
		{
			return type_ == token_types_t::token_t_identifier;
		}

		bool is_string_literal( ) const
		{
			return type_ == token_types_t::token_t_string_literal;
		}

		bool is_operand( ) const
		{
			return type_ == token_types_t::token_t_operand;
		}
		
		bool is_binary_operator( ) const
		{
			return type_ == token_types_t::token_t_binary_operator;
		}

		bool is_logical_operator( ) const
		{
			return type_ == token_types_t::token_t_logical_operator;
		}

		bool is_bracket_type( ) const
		{
			return type_ == token_types_t::token_t_bracket_type;
		}

		bool is_arrow_access( ) const
		{
			return type_ == token_types_t::token_t_arrow_access;
		}

		bool is_dot_access( ) const
		{
			return type_ == token_types_t::token_t_dot_access;
		}

		bool is_scope_resolution( ) const
		{
			return type_ == token_types_t::token_t_scope_resolution;
		}

		bool is_semicolon( ) const
		{
			return type_ == token_types_t::token_t_semicolon;
		}

		bool is_comma( ) const
		{
			return type_ == token_types_t::token_t_comma;
		}

		bool is_keyword( ) const
		{
			return type_ == token_types_t::token_t_keyword;
		}

		bool empty( ) const
		{
			return type_ == token_types_t::token_t_none;
		}

	public:
		template<typename _CodeIterator, typename _Alphabet>
		static single_token<codec_cvt> 
			parse_single( _CodeIterator &it, const _Alphabet &alphabet )
		{
			if ( !it )
				return single_token<codec_cvt>( );
			else if ( *it == codec_cvt::cvt( '"' ) )
			{
				string_type str;
				it.pre_increment( );
				while ( it && *it != codec_cvt::cvt( '"' ) )
					str.push_back( *it.post_increment( ) );
				++it;
				return single_token<codec_cvt>( str, it.get_row( ), it.get_column( ), token_types_t::token_t_string_literal );
			}
			else if ( *it == codec_cvt::cvt( '\'' ) )
			{
				string_type str;
				it.pre_increment( );
				while ( it && *it != codec_cvt::cvt( '\'' ) )
					str.push_back( *it.post_increment( ) );
				++it;
				return single_token<codec_cvt>( str, it.get_row( ), it.get_column( ), token_types_t::token_t_identifier );
			}
			else if ( codec_cvt::is_alpha( *it ) )
			{
				string_type str{ *it };
				it.pre_increment( );
				while ( it && codec_cvt::is_alnum( *it ) )
					str.push_back( *it.post_increment( ) );
				if ( it && !codec_cvt::is_any_but_whitespace( *it ) )
					it.move_to_next( );
				if ( alphabet.is_keyword( str ) )
					return single_token<codec_cvt>( str, it.get_row( ), it.get_column( ), token_types_t::token_t_keyword );
				return single_token<codec_cvt>( str, it.get_row( ), it.get_column( ), token_types_t::token_t_identifier );
			}
			else if ( codec_cvt::is_digit( *it ) )
			{
				string_type str{ *it };
				it.pre_increment( );
				while ( it && codec_cvt::is_digit( *it ) )
					str.push_back( *it.post_increment( ) );
				if ( it && !codec_cvt::is_any_but_whitespace( *it ) )
					it.move_to_next( );
				return single_token<codec_cvt>( str, it.get_row( ), it.get_column( ), token_types_t::token_t_operand );
			}
			else if ( *it == '0' && ( it + 1u ) && *( it + 1u ) == 'x' )
			{
				string_type str = codec_cvt::cvt( "0x" );
				++( ++it );
				while ( it && codec_cvt::is_xdigit( *it ) )
					str.push_back( *it.post_increment( ) );
				return single_token<codec_cvt>( str, it.get_row( ), it.get_column( ), token_types_t::token_t_operand );
			}
			else
			{
				switch ( *it )
				{
					case codec_cvt::cvt( '=' ):
					{
						if ( ( it + 1 ) && *( it + 1 ) == codec_cvt::cvt( '=' ) )
							return single_token<codec_cvt>( codec_cvt::cvt( "==" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_logical_operator );
						return single_token<codec_cvt>( codec_cvt::cvt( "=" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator );
					}
					case codec_cvt::cvt( ':' ):
					{
						if ( ( it + 1 ) && *( it + 1 ) == codec_cvt::cvt(':') )
							return single_token<codec_cvt>( codec_cvt::cvt("::"), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_scope_resolution );
						return single_token<codec_cvt>( codec_cvt::cvt(":"), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_other );
					}
					case codec_cvt::cvt( '+' ):
					{
						if ( ( it + 1 ) )
							if ( *( it + 1 ) == codec_cvt::cvt( '+' ) )
								return single_token<codec_cvt>( codec_cvt::cvt("++"), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 1 );
							else if ( *( it + 1 ) == codec_cvt::cvt('=') )
								return single_token<codec_cvt>( codec_cvt::cvt("+="), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 15 );
						return single_token<codec_cvt>( codec_cvt::cvt("+"), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 6 );
					}
					case codec_cvt::cvt( '-' ):
					{
						if ( ( it + 1 ) )
							if ( *( it + 1 ) == codec_cvt::cvt( '-' ) )
								return single_token<codec_cvt>( codec_cvt::cvt( "--" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 1 );
							else if ( *( it + 1 ) == codec_cvt::cvt( '=' ) )
								return single_token<codec_cvt>( codec_cvt::cvt( "-=" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 15 );
							else if ( *( it + 1 ) == '>' )
								return single_token<codec_cvt>( codec_cvt::cvt( "->" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_arrow_access );
							else if ( codec_cvt::is_digit( *( it + 1 ) ) )
							{
								auto dig = single_token<codec_cvt>::parse_single( ++it, alphabet );
								return single_token<codec_cvt>( codec_cvt::cvt( "-" ) + dig.get_block( ), dig.get_row( ), dig.get_column( ), token_types_t::token_t_operand );
							}
						return single_token<codec_cvt>( codec_cvt::cvt( "-" ), it.get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 6 );
					}
					case codec_cvt::cvt( '*' ):
					{
						if ( ( it + 1 ) && *( it + 1 ) == codec_cvt::cvt( '=' ) )
								return single_token<codec_cvt>( codec_cvt::cvt( "*=" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 15 );
						return single_token<codec_cvt>( codec_cvt::cvt( "*" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 5 );
					}
					case codec_cvt::cvt( '/' ):
					{
						if ( ( it + 1 ) && *( it + 1 ) == codec_cvt::cvt( '/' ) )
							return single_token<codec_cvt>( codec_cvt::cvt( "/=" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 15 );
						return single_token<codec_cvt>( codec_cvt::cvt( "/" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 5 );
					}
					case codec_cvt::cvt( '&' ):
					{
						if ( ( it + 1 ) )
							if ( *( it + 1 ) == codec_cvt::cvt( '&' ) )
								return single_token<codec_cvt>( codec_cvt::cvt( "&&" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_logical_operator, 13 );
							else if ( *( it + 1 ) == codec_cvt::cvt( '=' ) )
								return single_token<codec_cvt>( codec_cvt::cvt( "&=" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 15 );
						return single_token<codec_cvt>( codec_cvt::cvt( "&" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 10 );
					}
					case codec_cvt::cvt( '|' ):
					{
						if ( ( it + 1 ) )
							if ( *( it + 1 ) == codec_cvt::cvt( '|' ) )
								return single_token<codec_cvt>( codec_cvt::cvt( "||" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_logical_operator, 13 );
							else if ( *( it + 1 ) == codec_cvt::cvt( '=' ) )
								return single_token<codec_cvt>( codec_cvt::cvt( "|=" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 15 );
						return single_token<codec_cvt>( codec_cvt::cvt( "|" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 10 );
					}
					case codec_cvt::cvt( '^' ):
					{
						if ( ( it + 1 ) && *( it + 1 ) == codec_cvt::cvt( '=' ) )
							return single_token<codec_cvt>( codec_cvt::cvt( "^=" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 15 );
						return single_token<codec_cvt>( codec_cvt::cvt( "^" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 11 );
					}
					case codec_cvt::cvt( '>' ):
					{
						if ( ( it + 1 ) )
							if ( *( it + 1 ) == codec_cvt::cvt( '>' ) )
								if ( ( it + 2 ) && *( it + 2 ) == codec_cvt::cvt( '=' ) )
									return single_token<codec_cvt>( codec_cvt::cvt( ">>=" ), ( ++( ++( ++it ) ) ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 15 );
								else
									return single_token<codec_cvt>( codec_cvt::cvt( ">>" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 7 );
							else if ( *( it + 1 ) == codec_cvt::cvt( '=' ) )
								return single_token<codec_cvt>( codec_cvt::cvt( ">=" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_logical_operator, 8 );
						return single_token<codec_cvt>( codec_cvt::cvt( ">" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_logical_operator, 8 );
					}
					case codec_cvt::cvt( '<' ):
					{
						if ( ( it + 1 ) )
							if ( *( it + 1 ) == codec_cvt::cvt( '<' ) )
								if ( ( it + 2 ) && *( it + 2 ) == codec_cvt::cvt( '=' ) )
									return single_token<codec_cvt>( codec_cvt::cvt( "<<=" ), ( ++( ++( ++it ) ) ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 15 );
								else
									return single_token<codec_cvt>( codec_cvt::cvt( "<<" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 7 );
							else if ( *( it + 1 ) == codec_cvt::cvt( '=' ) )
								return single_token<codec_cvt>( codec_cvt::cvt( "<=" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_logical_operator, 8 );
						return single_token<codec_cvt>( codec_cvt::cvt( "<" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_logical_operator, 8 );
					}
					case codec_cvt::cvt( '%' ):
					{
						if ( ( it + 1 ) && *( it + 1 ) == codec_cvt::cvt( '=' ) )
							return single_token<codec_cvt>( codec_cvt::cvt( "%=" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 15 );
						return single_token<codec_cvt>( codec_cvt::cvt( "%" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 5 );
					}
					case codec_cvt::cvt( '!' ):
					{
						if ( ( it + 1 ) && *( it + 1 ) == codec_cvt::cvt( '=' ) )
							return single_token<codec_cvt>( codec_cvt::cvt( "!=" ), ( ++( ++it ) ).get_row( ), it.get_column( ), token_types_t::token_t_logical_operator, 9 );
						return single_token<codec_cvt>( codec_cvt::cvt( "!" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_logical_operator, 3 );
					}
					case codec_cvt::cvt( '~' ):
					{
						return single_token<codec_cvt>( codec_cvt::cvt( "~" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_binary_operator, 16 );
					}
					case codec_cvt::cvt( ',' ):
					{
						return single_token<codec_cvt>( codec_cvt::cvt( "," ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_comma );
					}
					case codec_cvt::cvt( '.' ):
					{
						return single_token<codec_cvt>( codec_cvt::cvt( "." ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_dot_access );
					}
					case codec_cvt::cvt( '(' ):
					{
						return single_token<codec_cvt>( codec_cvt::cvt( "(" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_bracket_type );
					}
					case codec_cvt::cvt( ')' ):
					{
						return single_token<codec_cvt>( codec_cvt::cvt( ")" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_bracket_type );
					}
					case codec_cvt::cvt( '[' ):
					{
						return single_token<codec_cvt>( codec_cvt::cvt( "[" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_bracket_type );
					}
					case codec_cvt::cvt( ']' ):
					{
						return single_token<codec_cvt>( codec_cvt::cvt( "]" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_bracket_type );
					}
					case codec_cvt::cvt( '{' ):
					{
						return single_token<codec_cvt>( codec_cvt::cvt( "{" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_bracket_type );
					}
					case codec_cvt::cvt( '}' ):
					{
						return single_token<codec_cvt>( codec_cvt::cvt( "}" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_bracket_type );
					}
					case codec_cvt::cvt( ';' ):
					{
						return single_token<codec_cvt>( codec_cvt::cvt( ";" ), ( ++it ).get_row( ), it.get_column( ), token_types_t::token_t_semicolon );
					}
				}
			}
			return single_token<codec_cvt>( { *(it++) }, it.get_row( ), it.get_column( ), token_types_t::token_t_other );
		}

		static single_token<codec_cvt>
			parse_single( const string_type &str )
		{
			return parse_single( code_iterator<codec_cvt>( str.begin( ), str.end( ) ), tokenizing::alphabet<codec_cvt>( ) );
		}


	private:
		int row_, column_;
		string_type block_;
		token_types_t type_;
		int presedence_;
	};

} }