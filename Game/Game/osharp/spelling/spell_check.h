#pragma once
#include "dictionary.h"


namespace osharp { namespace spelling {

	template<typename _Codec>
	class suggestion
	{
	public:
		using codec_cvt = _Codec;
		using string_type = typename _Codec::string_type;

		enum SuggestionTypes
		{
			SwapCharacter,
			InsertCharacter,
			DeleteCharacter,
			ReplaceCharacter,
			Split,
			Found,
			None,
		};

	public:
		suggestion( string_type suggestedWord, SuggestionTypes suggestedType, size_t strength = 0 )
			: suggested_( std::move( suggestedWord ) ),
			type_( suggestedType ),
			strength_( strength )
		{ }
		suggestion( )
			: suggested_( ),
			type_( None )
		{ }

		bool is_split( ) const
		{
			return type_ == Split;
		}

		bool is_swap_character( ) const
		{
			return type_ == SwapCharacter;
		}

		bool is_insert_character( ) const
		{
			return type_ == InsertCharacter;
		}

		bool is_delete_character( ) const
		{
			return type_ == DeleteCharacter;
		}

		bool is_replace_character( ) const
		{
			return type_ == ReplaceCharacter;
		}

		bool is_correct_word( ) const
		{
			return type_ == Found;
		}

		const string_type &get_suggested_word( ) const
		{
			return suggested_;
		}

		string_type &get_suggested_word( )
		{
			return suggested_;
		}

		const SuggestionTypes &get_suggestion_type( ) const
		{
			return type_;
		}

		SuggestionTypes &get_suggestion_type( )
		{
			return type_;
		}

		size_t get_strength( ) const
		{
			return strength_;
		}

		void increase_strength( )
		{
			++strength_;
		}

	private:
		string_type suggested_;
		SuggestionTypes type_;
		size_t strength_;
	};

	template<typename _Codec>
	class word_suggestions
	{
	public:
		using codec_cvt = _Codec;
		using string_type = typename codec_cvt::string_type;
		using suggestion_type = suggestion<codec_cvt>;
	public:
		word_suggestions()
			: typed_word_( ),
			suggestions_( )
		{ }
		word_suggestions( string_type typed )
			: typed_word_( std::move( typed ) ),
			suggestions_( )
		{ }
		word_suggestions( string_type typed, std::vector<suggestion_type> suggestions )
			: typed_word_( std::move( typed ) ),
			suggestions_( std::move( suggestions ) )
		{ }

		bool has_suggestions( ) const
		{
			return !suggestions_.empty( );
		}
		
		size_t get_number_of_suggestions( ) const
		{
			return suggestions_.size( );
		}

		const suggestion_type &get_likeliest_correction( ) const
		{
			return suggestions_.front( );
		}

		const std::vector<suggestion_type> &get_suggestions( ) const
		{
			return suggestions_;
		}

		std::vector<suggestion_type> &get_suggestions( )
		{
			return suggestions_;
		}

		const string_type &get_typed_word( ) const
		{
			return typed_word_;
		}

		string_type &get_typed_word( )
		{
			return typed_word_;
		}

	private:
		string_type typed_word_;
		std::vector<suggestion_type> suggestions_;
	};

	template<typename _Codec>
	class spell_check
	{
	public:
		using codec_cvt = _Codec;
		using self_type = spell_check<codec_cvt>;
		using string_type = typename codec_cvt::string_type;

	public:
		spell_check( dictionary<codec_cvt> dictionary )
			: dictionary_( std::move( dictionary ) )
		{ }
		spell_check( ) = delete;

		word_suggestions<codec_cvt> spell_suggestion( const string_type &word ) const
		{
			if ( word.empty( ) )
				return{ };

			std::vector<suggestion<codec_cvt>> suggestions;
			get_suggestions( word, suggestions );

			std::sort( suggestions.begin( ), suggestions.end( ), []( auto &a, auto &b )
			{
				return a.get_strength( ) > b.get_strength( );
			} );

			if ( dictionary_.lookup( word ) )
			{
				auto highest = 0u;
				std::for_each( suggestions.begin( ), suggestions.end( ), [&highest]( auto &x )mutable
				{
					highest = x.get_strength( ) > highest ?
						x.get_strength( ) :
						highest;
				} );
				increase_or_insert( word, suggestion<codec_cvt>::Found, suggestions, highest + 1 );
			}
			return word_suggestions<codec_cvt>( word, suggestions );
		}
		
	private:
		void get_suggestions( const string_type &word, 
							  std::vector<suggestion<codec_cvt>> &suggestions ) const
		{
			check_swap( word, suggestions );
			check_insert( word, suggestions );
			check_delete( word, suggestions );
			check_replace( word, suggestions );
			check_split( word, suggestions );
		}

		void increase_or_insert( string_type word, 
								 typename suggestion<codec_cvt>::SuggestionTypes sugg_t,
								 std::vector<suggestion<codec_cvt>> &suggestions,
								 size_t startingStrength = 1 ) const
		{
			bool found = false;
			for ( auto &x : suggestions )
			{
				if ( x.get_suggested_word( ) == word )
				{
					//if ( !x.is_split( ) )
						x.increase_strength( );
					if ( sugg_t == suggestion<codec_cvt>::Found )
					{
						while ( x.get_strength( ) < startingStrength )
							x.increase_strength( );
						x.get_suggestion_type( ) = suggestion<codec_cvt>::Found;
					}
					found = true;
					break;
				}
			}
			if ( !found )
				suggestions.emplace_back( std::move( word ), sugg_t, startingStrength );
		}

		void check_swap( const string_type &word, std::vector<suggestion<codec_cvt>> &suggestions ) const
		{
			auto temp = word;
			for ( auto i = 0u;
				  i < word.length( ) - 1;
				  ++i )
			{
				std::swap( *( std::begin( temp ) + i ), *( std::begin( temp ) + i + 1 ) );
				if ( dictionary_.lookup( temp ) )
					increase_or_insert( temp, suggestion<codec_cvt>::SwapCharacter, suggestions, 2 );
				std::swap( *( std::begin( temp ) + i ), *( std::begin( temp ) + i + 1 ) );
			}
		}

		void check_insert( const string_type &word, std::vector<suggestion<codec_cvt>> &suggestions ) const
		{
			auto temp = word;
			for ( auto i = 0u;
				  i < word.length( );
				  ++i )
			{
				temp.insert( std::begin( temp ) + i, codec_cvt::cvt( ' ' ) );

				for ( auto j = codec_cvt::cvt( 'a' );
					  j <= codec_cvt::cvt( 'z' );
					  ++j )
				{
					temp[i] = j;
					if ( dictionary_.lookup( temp ) )
						increase_or_insert( temp, suggestion<codec_cvt>::InsertCharacter, suggestions, 1 );
				}
				temp.erase( std::begin( temp ) + i );
			}
		}

		void check_delete( const string_type &word, std::vector<suggestion<codec_cvt>> &suggestions ) const
		{
			auto temp = word;
			for ( auto i = 0u;
				  i < word.length( );
				  ++i )
			{
				auto at = temp[i];
				temp.erase( std::begin( temp ) + i );
				if ( dictionary_.lookup( temp ) )
					increase_or_insert( temp, suggestion<codec_cvt>::DeleteCharacter, suggestions, 1 );
				temp.insert( std::begin( temp ) + i, at );
			}
		}

		void check_replace( const string_type &word, std::vector<suggestion<codec_cvt>> &suggestions ) const
		{
			auto temp = word;
			for ( auto i = 0u;
				  i < word.length( );
				  ++i )
			{
				auto tmpAt = temp[i];

				for ( auto j = codec_cvt::cvt( 'a' );
					  j <= codec_cvt::cvt( 'z' );
					  ++j )
				{
					temp[i] = j;
					if ( dictionary_.lookup( temp ) )
						increase_or_insert( temp, suggestion<codec_cvt>::ReplaceCharacter, suggestions, 2 );
				}
				temp[i] = tmpAt;
			}
		}

		void check_split( const string_type &word, std::vector<suggestion<codec_cvt>> &suggestions ) const
		{
			for ( auto i = 0u;
				  i < word.length( );
				  ++i )
			{
				auto temp1 = word.substr( 0, i );
				auto temp2 = word.substr( i );
				if ( dictionary_.lookup( temp1 ) )
					increase_or_insert( temp1, suggestion<codec_cvt>::Split, suggestions );
				if ( dictionary_.lookup( temp2 ) )
					increase_or_insert( temp2, suggestion<codec_cvt>::Split, suggestions );
			}
		}

	private:
		dictionary<codec_cvt> dictionary_;
	};

} }

template<typename _Codec, typename _Fmt = typename _Codec::output_format>
static std::basic_ostream<_Fmt> &operator<<( std::basic_ostream<_Fmt> &ostr, const osharp::spelling::suggestion<_Codec> &codec )
{
	ostr << '\'' << codec.get_suggested_word( ) << '\'';
	return ostr;
}