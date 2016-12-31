#pragma once
#include "element.h"

namespace ybf
{

	class ybf_bundle : public ybf_element
	{
	public:
		using of = std::vector<ybf_ptr>;

	public:
		ybf_bundle( std::string name, std::vector<ybf_ptr> value );
		ybf_bundle( const ybf_bundle &other );

		const std::uint32_t get_type( ) const override;

		const std::vector<ybf_ptr> &get_values( ) const;

		std::vector<ybf_ptr> &get_values( );

		virtual void write( std::ostream &stream ) const override;

	private:
		std::vector<ybf_ptr> value_;
	};

}