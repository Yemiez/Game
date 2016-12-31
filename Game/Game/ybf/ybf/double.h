#pragma once
#include "element.h"

namespace ybf
{

	class ybf_double : public ybf_element
	{
	public:
		ybf_double( std::string name, double value );
		ybf_double( const ybf_double &other );

		const std::uint32_t get_type( ) const override;

		const double get_value( ) const;

		void set_value( double value );

		virtual void write( std::ostream &stream ) const override;

	private:
		double value_;
	};

}