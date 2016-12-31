#include "ascii.h"

ybf::ybf_ascii::ybf_ascii( std::string name, std::string value )
	: ybf_element( std::move( name ) ),
	value_( value )
{}

ybf::ybf_ascii::ybf_ascii( const ybf_ascii & other )
	: ybf_element( other ),
	value_( other.value_ )
{}

const std::uint32_t ybf::ybf_ascii::get_type( ) const
{
	return types::TYPE_ASCII_STRING;
}

const std::string ybf::ybf_ascii::get_value( ) const
{
	return value_;
}

void ybf::ybf_ascii::set_value( std::string value )
{
	value_ = std::move( value );
}

void ybf::ybf_ascii::write( std::ostream &stream ) const
{
	__super::write( stream );
	editor::write( stream, value_.length( ) );
	editor::write( stream, value_ );
}
