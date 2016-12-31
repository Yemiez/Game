#include "ulong.h"

ybf::ybf_ulong::ybf_ulong( std::string name, int value )
	: ybf_element( std::move( name ) ),
	value_( value )
{}

ybf::ybf_ulong::ybf_ulong( const ybf_ulong & other )
	: ybf_element( other ),
	value_( other.value_ )
{}

const std::uint32_t ybf::ybf_ulong::get_type( ) const
{
	return types::TYPE_ULONG;
}

const unsigned long ybf::ybf_ulong::get_value( ) const
{
	return value_;
}

void ybf::ybf_ulong::set_value( unsigned long value )
{
	value_ = value;
}

void ybf::ybf_ulong::write( std::ostream &stream ) const
{
	__super::write( stream );
	editor::write( stream, value_ );
}
