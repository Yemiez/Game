#pragma once
#include "element.h"

namespace ybf
{

	class ybf_unicode : public ybf_element
	{
	public:
		ybf_unicode( std::string name, std::wstring value );
		ybf_unicode( const ybf_unicode &other );

		const std::uint32_t get_type( ) const override;

		const std::wstring get_value( ) const;

		void set_value( std::wstring value );

		virtual void write( std::ostream &stream ) const override;

	private:
		std::wstring value_;
	};

}