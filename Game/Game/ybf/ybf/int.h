#pragma once
#include "element.h"

namespace ybf
{

	class ybf_int : public ybf_element
	{
	public:
		ybf_int( std::string name, int value );
		ybf_int( const ybf_int &other );

		const std::uint32_t get_type( ) const override;

		const int get_value( ) const;

		void set_value( int value );

		virtual void write( std::ostream &stream ) const override;

	private:
		int value_;
	};

}