#pragma once
#include "element.h"

namespace ybf
{

	class ybf_long : public ybf_element
	{
	public:
		ybf_long( std::string name, int value );
		ybf_long( const ybf_long &other );

		const std::uint32_t get_type( ) const override;

		const long get_value( ) const;

		void set_value( long value );

		virtual void write( std::ostream &stream ) const override;

	private:
		long value_;
	};

}