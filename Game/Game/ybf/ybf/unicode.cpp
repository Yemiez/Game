#include "unicode.h"

ybf::ybf_unicode::ybf_unicode( std::string name, std::wstring value )
	: ybf_element( std::move( name ) ),
	value_( value )
{}

ybf::ybf_unicode::ybf_unicode( const ybf_unicode & other )
	: ybf_element( other ),
	value_( other.value_ )
{}

const std::uint32_t ybf::ybf_unicode::get_type( ) const
{
	return types::TYPE_UNICODE_STRING;
}

const std::wstring ybf::ybf_unicode::get_value( ) const
{
	return value_;
}

void ybf::ybf_unicode::set_value( std::wstring value )
{
	value_ = std::move( value );
}

void ybf::ybf_unicode::write( std::ostream &stream ) const
{
	__super::write( stream );
	//editor::write( stream, value_.length( ) );
	editor::write( stream, value_ );
}
