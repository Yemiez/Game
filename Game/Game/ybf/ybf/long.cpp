#include "long.h"

ybf::ybf_long::ybf_long( std::string name, int value )
	: ybf_element( std::move( name ) ),
	value_( value )
{}

ybf::ybf_long::ybf_long( const ybf_long & other )
	: ybf_element( other ),
	value_( other.value_ )
{}

const std::uint32_t ybf::ybf_long::get_type( ) const
{
	return types::TYPE_LONG;
}

const long ybf::ybf_long::get_value( ) const
{
	return value_;
}

void ybf::ybf_long::set_value( long value )
{
	value_ = value;
}

void ybf::ybf_long::write( std::ostream &stream ) const
{
	__super::write( stream );
	editor::write( stream, value_ );
}
