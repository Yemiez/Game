#include "bundle.h"

ybf::ybf_bundle::ybf_bundle( std::string name, std::vector<ybf_ptr> value )
	: ybf_element( std::move( name ) ),
	value_( std::move( value ) )
{}

ybf::ybf_bundle::ybf_bundle( const ybf_bundle & other )
	: ybf_element( other ),
	value_( other.value_ )
{}

const std::uint32_t ybf::ybf_bundle::get_type( ) const
{
	return types::TYPE_BUNDLE;
}

const std::vector<ybf::ybf_ptr>& ybf::ybf_bundle::get_values( ) const
{
	return value_;
}

std::vector<ybf::ybf_ptr>& ybf::ybf_bundle::get_values( )
{
	return value_;
}

void ybf::ybf_bundle::write( std::ostream &stream ) const
{
	__super::write( stream );
	editor::write( stream, value_.size( ) );
	for ( auto &x : value_ )
		x->write( stream );
}
