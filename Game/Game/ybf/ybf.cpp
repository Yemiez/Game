#include "ybf.h"
#include "ybf/editor.h"
#include <sstream>

namespace ybf
{
	std::string get_str( std::istream &stream )
	{
		std::uint32_t str_len = 0;
		std::string str;
		editor::read( stream, str_len );
		editor::read( stream, str, str_len );
		return str;
	}

	std::wstring get_wstr( std::istream &stream )
	{
		std::uint32_t str_len = 0;
		std::wstring str;
		editor::read( stream, str_len );
		editor::read( stream, str, str_len * sizeof(wchar_t) );
		return str;
	}

	template< typename T >
	struct array_deleter
	{
		void operator ()( T const * p )
		{
			delete[] p;
		}
	};

	ybf::ybf_ptr parse_type( std::istream &stream )
	{
		std::uint32_t type = 0;
		editor::read( stream, type );

		switch ( type )
		{
			case types::TYPE_ASCII_STRING:
			{
				auto name = get_str( stream );
				auto value = get_str( stream );
				return std::make_shared<ybf_ascii>( std::move( name ), std::move( value ) );
			}
			break;
			case types::TYPE_BUNDLE:
			{
				auto name = get_str( stream );
				std::uint32_t size = 0u;
				editor::read( stream, size );
				std::vector<ybf_ptr> elements;
				for ( auto i = 0u; i < size; ++i )
				{
					auto xy = parse_type( stream );
					if ( xy )
						elements.emplace_back( xy );
				}
				return std::make_shared<ybf_bundle>( std::move( name ), std::move( elements ) );
			}
			break;
			case types::TYPE_DOUBLE:
			{
				auto name = get_str( stream );
				double value = .0;
				editor::read( stream, value );
				return std::make_shared<ybf_double>( std::move( name ), value );
			}
			break;
			case types::TYPE_FLOAT:
			{
				auto name = get_str( stream );
				float value = .0f;
				editor::read( stream, value );
				return std::make_shared<ybf_float>( std::move( name ), value );
			}
			case types::TYPE_INT:
			{
				auto name = get_str( stream );
				int value = 0;
				editor::read( stream, value );
				return std::make_shared<ybf_int>( std::move( name ), value );
			}
			break;
			case types::TYPE_LONG:
			{
				auto name = get_str( stream );
				long value = 0l;
				editor::read( stream, value );
				return std::make_shared<ybf_long>( std::move( name ), value );
			}
			break;
			case types::TYPE_SHORT:
			{
				auto name = get_str( stream );
				short value = 0;
				editor::read( stream, value );
				return std::make_shared<ybf_short>( std::move( name ), value );
			}
			break;
			case types::TYPE_UINT:
			{
				auto name = get_str( stream );
				std::uint32_t value = 0u;
				editor::read( stream, value );
				return std::make_shared<ybf_uint>( std::move( name ), value );
			}
			break;
			case types::TYPE_ULONG:
			{
				auto name = get_str( stream );
				unsigned long value = 0ul;
				editor::read( stream, value );
				return std::make_shared<ybf_ulong>( std::move( name ), value );
			}
			break;
			case types::TYPE_BYTE_ARRAY:
			{
				auto name = get_str( stream );
				auto len = 0u;
				
				editor::read( stream, len );
				std::shared_ptr<char> ptr_b( new char[len], array_deleter<char>( ) );
				std::vector<char> bytes;
				bytes.insert( bytes.begin( ), ptr_b.get( ), ptr_b.get( ) + len );
				return std::make_shared<ybf_byte_array>( std::move( name ), std::move( bytes ) );
			}
			//case types::TYPE_UNICODE_STRING:
			//{
			//	auto name = get_str( stream );
			//	auto value = get_wstr( stream );
			//	return std::make_shared<ybf_unicode>( std::move( name ), std::move( value ) );
			//}
			break;
		}
		return nullptr;
	}
}

ybf::ybf_ptr ybf::import( ybf::ybf_header & header )
{
	std::stringstream dmem;
	dmem.write( reinterpret_cast<const char*>(header.dynamic_memory_pointer( )), header.get_sizeof_dynamic_region( ) );
	dmem.seekg( 0, std::ios_base::beg );
	return parse_type( dmem );
}
