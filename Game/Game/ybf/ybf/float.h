#pragma once
#include "element.h"

namespace ybf
{

	class ybf_float : public ybf_element
	{
	public:
		ybf_float( std::string name, float value );
		ybf_float( const ybf_float &other );

		const std::uint32_t get_type( ) const override;

		const float get_value( ) const;

		void set_value( float value );

		virtual void write( std::ostream &stream ) const override;

	private:
		float value_;
	};

}