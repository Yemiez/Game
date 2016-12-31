#include "float.h"

ybf::ybf_float::ybf_float( std::string name, float value )
	: ybf_element( std::move( name ) ),
	value_( value )
{}

ybf::ybf_float::ybf_float( const ybf_float & other )
	: ybf_element( other ),
	value_( other.value_ )
{}

const std::uint32_t ybf::ybf_float::get_type( ) const
{
	return types::TYPE_FLOAT;
}

const float ybf::ybf_float::get_value( ) const
{
	return value_;
}

void ybf::ybf_float::set_value( float value )
{
	value_ = value;
}

void ybf::ybf_float::write( std::ostream &stream ) const
{
	__super::write( stream );
	editor::write( stream, value_ );
}
