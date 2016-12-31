#pragma once
#include <vector>

namespace ybf
{

	class editor
	{
	public:
		struct SaveData
		{
			std::streampos streampos;
			int exceptions;
		};

		struct IndentData
		{
			std::uint32_t x;
		};

		template<typename T>
		static void write( std::ostream &output, const T &t )
		{
			output.write( reinterpret_cast<const char*>(&t), sizeof( T ) );
		}

		template<typename T>
		static void write( std::ostream &output, const std::basic_string<T> &s )
		{
			output.write( reinterpret_cast<const char*>(s.data( )), s.size( ) * sizeof( T ));
		}

		template<typename T>
		static void read( std::istream &input, T &out )
		{
			input.read( reinterpret_cast<char*>(&out), sizeof( T ) );
		}

		template<typename T>
		static void read( std::istream &input, std::basic_string<T> &str, const std::uint32_t &size )
		{
			str = std::basic_string<T>( size, static_cast<T>('\n') );
			input.read( reinterpret_cast<char*>( &str[0] ), size * sizeof T );
		}

		static SaveData save( std::istream &input )
		{
			return{
				input.tellg( ),
				input.exceptions( )
			};
		}

		static void restore( std::istream &input, SaveData &data )
		{
			input.seekg( data.streampos );
			input.exceptions( data.exceptions );
			input.clear( );
		}

		static IndentData indent( std::uint32_t indent )
		{
			return{ indent };
		}

	};

	template<typename T>
	static std::basic_ostream<T> &operator<<( std::basic_ostream<T> &str, const editor::IndentData &data )
	{
		for ( auto i = 0u; i < data.x; ++i )
			str << static_cast<T>( '\t' );
		return str;
	}

}