#include "uint.h"

ybf::ybf_uint::ybf_uint( std::string name, std::uint32_t value )
	: ybf_element( std::move( name ) ),
	value_( value )
{}

ybf::ybf_uint::ybf_uint( const ybf_uint & other )
	: ybf_element( other ),
	value_( other.value_ )
{}

const std::uint32_t ybf::ybf_uint::get_type( ) const
{
	return types::TYPE_UINT;
}

const std::uint32_t ybf::ybf_uint::get_value( ) const
{
	return value_;
}

void ybf::ybf_uint::set_value( std::uint32_t value )
{
	value_ = value;
}

void ybf::ybf_uint::write( std::ostream &stream ) const
{
	__super::write( stream );
	editor::write( stream, value_ );
}
