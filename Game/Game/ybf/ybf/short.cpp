#include "short.h"

ybf::ybf_short::ybf_short( std::string name, short value )
	: ybf_element( std::move( name ) ),
	value_( value )
{}

ybf::ybf_short::ybf_short( const ybf_short & other )
	: ybf_element( other ),
	value_( other.value_ )
{}

const std::uint32_t ybf::ybf_short::get_type( ) const
{
	return types::TYPE_SHORT;
}

const short ybf::ybf_short::get_value( ) const
{
	return value_;
}

void ybf::ybf_short::set_value( short value )
{
	value_ = value;
}

void ybf::ybf_short::write( std::ostream &stream ) const
{
	__super::write( stream );
	editor::write( stream, value_ );
}
