#pragma once
#include "utils.h"
#include "editor.h"
#include <memory>

namespace ybf
{
	class ybf_element;
	using ybf_ptr = std::shared_ptr<ybf_element>;

	enum ExportAs : std::uint32_t
	{
		compressed_ybf = make_bit( 0 ),
		encrypted_ybf = make_bit( 1 ),
		dynamic_ybf = make_bit( 2 ),
		static_ybf = make_bit( 3 ),
	};

	class ybf_element
	{
	public:
		ybf_element( std::string name );
		ybf_element( const ybf_element & );
		virtual ~ybf_element( );

		const std::string &get_name( ) const;

		void set_name( std::string name );

		virtual const std::uint32_t get_type( ) const = 0;
		
		virtual void write( std::ostream &output ) const;

		virtual void introduce( std::ostream &output, std::uint32_t indent = 0 ) const;

		void export_element( std::ostream &stream,
							 const std::uint32_t &eflags ) const;

	private:
		std::string name_;
	};

}