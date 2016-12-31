#pragma once
#include "element.h"

namespace ybf
{

	class ybf_uint : public ybf_element
	{
	public:
		ybf_uint( std::string name, std::uint32_t value );
		ybf_uint( const ybf_uint &other );

		const std::uint32_t get_type( ) const override;

		const std::uint32_t get_value( ) const;

		void set_value( std::uint32_t value );

		virtual void write( std::ostream &stream ) const override;

	private:
		std::uint32_t value_;
	};

}