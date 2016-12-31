#pragma once
#include "element.h"

namespace ybf
{

	class ybf_ulong : public ybf_element
	{
	public:
		ybf_ulong( std::string name, int value );
		ybf_ulong( const ybf_ulong &other );

		const std::uint32_t get_type( ) const override;

		const unsigned long get_value( ) const;

		void set_value( unsigned long value );

		virtual void write( std::ostream &stream ) const override;

	private:
		unsigned long value_;
	};

}