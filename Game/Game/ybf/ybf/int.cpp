#include "int.h"

ybf::ybf_int::ybf_int( std::string name, int value )
	: ybf_element( std::move( name ) ),
	value_( value )
{}

ybf::ybf_int::ybf_int( const ybf_int & other )
	: ybf_element( other ),
	value_( other.value_ )
{}

const std::uint32_t ybf::ybf_int::get_type( ) const
{
	return types::TYPE_INT;
}

const int ybf::ybf_int::get_value( ) const
{
	return value_;
}

void ybf::ybf_int::set_value( int value )
{
	value_ = value;
}

void ybf::ybf_int::write( std::ostream &stream ) const
{
	__super::write( stream );
	editor::write( stream, value_ );
}
