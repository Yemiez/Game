#include "byte_array.h"

ybf::ybf_byte_array::ybf_byte_array( std::string name, std::vector<char> bytes )
	: ybf_element( std::move( name ) ), bytes_( std::move( bytes ) )
{}

const std::uint32_t ybf::ybf_byte_array::get_type( ) const
{
	return types::TYPE_BYTE_ARRAY;
}

const std::vector<char> ybf::ybf_byte_array::get_value( ) const
{
	return bytes_;
}

void ybf::ybf_byte_array::append( const std::string & bytes )
{
	return append( bytes.c_str( ), bytes.size( ) );
}

void ybf::ybf_byte_array::append( const char * str, const size_t size )
{
	bytes_.insert( bytes_.begin( ), str, str + size );
}

void ybf::ybf_byte_array::append( const std::vector<char>& bytes )
{
	return append( bytes.data( ), bytes.size( ) );
}

void ybf::ybf_byte_array::write( std::ostream & stream ) const
{
	__super::write( stream );
	editor::write( stream, bytes_.size( ) );
	stream.write( bytes_.data( ), bytes_.size( ) );
}
