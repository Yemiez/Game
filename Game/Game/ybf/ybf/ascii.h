#pragma once
#include "element.h"

namespace ybf
{

	class ybf_ascii : public ybf_element
	{
	public:
		ybf_ascii( std::string name, std::string value );
		ybf_ascii( const ybf_ascii &other );

		const std::uint32_t get_type( ) const override;

		const std::string get_value( ) const;

		void set_value( std::string value );

		virtual void write( std::ostream &stream ) const override;

	private:
		std::string value_;
	};

}