#pragma once
#include "element.h"

namespace ybf
{

	class ybf_byte_array : public ybf_element
	{
	public:
		using of = std::vector<char>;

	public:
		ybf_byte_array( std::string name, std::vector<char> bytes );

		const std::uint32_t get_type( ) const override;

		const std::vector<char> get_value( ) const;

		void append( const std::string &bytes );

		void append( const char *str, const size_t size );

		void append( const std::vector<char> &bytes );

		virtual void write( std::ostream &stream ) const override;

	private:
		std::vector<char> bytes_;
	};

}