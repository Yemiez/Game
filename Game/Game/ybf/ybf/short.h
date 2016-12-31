#pragma once
#include "element.h"

namespace ybf
{

	class ybf_short : public ybf_element
	{
	public:
		ybf_short( std::string name, short value );
		ybf_short( const ybf_short &other );

		const std::uint32_t get_type( ) const override;

		const short get_value( ) const;

		void set_value( short value );

		virtual void write( std::ostream &stream ) const override;

	private:
		short value_;
	};

}