#include "double.h"

ybf::ybf_double::ybf_double( std::string name, double value )
	: ybf_element( std::move( name ) ),
	value_( std::move( value ) )
{}

ybf::ybf_double::ybf_double( const ybf_double & other )
	: ybf_element( other ),
	value_( other.value_ )
{}

const std::uint32_t ybf::ybf_double::get_type( ) const
{
	return types::TYPE_DOUBLE;
}

const double ybf::ybf_double::get_value( ) const
{
	return value_;
}

void ybf::ybf_double::set_value( double value )
{
	value_ = value;
}

void ybf::ybf_double::write( std::ostream &stream ) const
{
	__super::write( stream );
	editor::write( stream, value_ );
}
