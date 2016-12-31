#include "element.h"
#include "header.h"
#include "ascii.h"
#include "bundle.h"
#include "double.h"
#include "float.h"
#include "int.h"
#include "long.h"
#include "short.h"
#include "uint.h"
#include "ulong.h"
#include "byte_array.h"
#include <sstream>
#include <zlib.h>

ybf::ybf_element::ybf_element( std::string name )
	: name_( std::move( name ) )
{}

ybf::ybf_element::ybf_element( const ybf_element &other )
	: name_( other.name_ )
{}

ybf::ybf_element::~ybf_element( )
{}

const std::string & ybf::ybf_element::get_name( ) const
{
	return name_;
}

void ybf::ybf_element::set_name( std::string name )
{
	name_ = std::move( name );
}

void ybf::ybf_element::write( std::ostream &stream ) const
{
	editor::write( stream, get_type( ) );
	editor::write( stream, name_.length( ) );
	editor::write( stream, get_name( ) );
}

void ybf::ybf_element::introduce( std::ostream & output, std::uint32_t indent ) const
{
	output << editor::indent( indent ) <<
		name_;
	switch ( this->get_type( ) )
	{
		case types::TYPE_ASCII_STRING:
			output << ": \"" << reinterpret_cast<const ybf_ascii*>(this)->get_value( ) << "\"\n";
			break;
		case types::TYPE_BUNDLE:
		{
			output << ": bundle";
			auto bundle = reinterpret_cast<const ybf_bundle*>(this);
			auto value = bundle->get_values( );
			if ( !value.empty( ) )
			{
				output << "\n" << editor::indent( indent ) << "{\n";
				for ( auto &x : value )
 					x->introduce( output, indent + 1 );
				output << editor::indent( indent ) << "}\n";
			}
			else
				output << " { }\n";
			break;
		}
		case types::TYPE_DOUBLE:
			output << ": " << reinterpret_cast<const ybf_double*>(this)->get_value( ) << "\n";
			break;
		case types::TYPE_FLOAT:
			output << ": " << reinterpret_cast<const ybf_float*>(this)->get_value( ) << "\n";
			break;
		case types::TYPE_INT:
			output << ": " << reinterpret_cast<const ybf_int*>(this)->get_value( ) << "\n";
			break;
		case types::TYPE_LONG:
			output << ": " << reinterpret_cast<const ybf_long*>(this)->get_value( ) << "\n";
			break;
		case types::TYPE_SHORT:
			output << ": " << reinterpret_cast<const ybf_short*>(this)->get_value( ) << "\n";
			break;
		case types::TYPE_UINT:
			output << ": " << reinterpret_cast<const ybf_uint*>(this)->get_value( ) << "\n";
			break;
		case types::TYPE_ULONG:
			output << ": " << reinterpret_cast<const ybf_ulong*>(this)->get_value( ) << "\n";
			break;
		case types::TYPE_BYTE_ARRAY:
			output << ": [size] " << reinterpret_cast<const ybf_byte_array*>(this)->get_value( ).size( ) << "\n";
			break;
	}

}

void ybf::ybf_element::export_element(std::ostream &stream, const std::uint32_t & eflags) const
{
	using header = ybf_header;
	using file = header::ybf_file;
	using header_info = file::header_info;

	header_info info;
	info.magic = header::MAGIC;
	info.file_flags = header::FileFlags::FILE_ASCII |
		header::FileFlags::FILE_UNICODE |
		header::FileFlags::FILE_SORTED |
		(eflags & ExportAs::compressed_ybf ?
		  header::FileFlags::FILE_COMPRESSED :
		  header::FILE_ASCII) |
		  (eflags & ExportAs::encrypted_ybf ?
			header::FileFlags::FILE_ENCRYPTED :
			header::FileFlags::FILE_ASCII) |
			(eflags & ExportAs::dynamic_ybf ?
			  header::FileFlags::FILE_DYNAMIC :
			  header::FileFlags::FILE_ASCII);
	info.size_header = sizeof( header );
	info.size_reserved_region = header::RESERVED_SIZE;
	info.size_static_region = header::STATIC_REGION;
	info.version = { MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION };
	info.layout[0] = header::LAYOUT_RMEM,
		info.layout[1] = header::LAYOUT_DMEM,
		info.layout[2] = header::LAYOUT_SMEM;

	uint32_t *rmem_ptr = nullptr;
	char *dmem_ptr = nullptr,
		*smem_ptr = nullptr;

	if ( eflags & ExportAs::dynamic_ybf )
	{
		smem_ptr = new char[0];
		info.size_static_region = 0;
		rmem_ptr = new uint32_t[info.size_reserved_region];
		memset( rmem_ptr, 0, info.size_reserved_region * sizeof uint32_t );

		std::stringstream dmem;
		write( dmem );
		auto dmem_str = dmem.str( );

		if ( eflags & ExportAs::compressed_ybf )
		{
			rmem_ptr[header::LAYOUT_DMEM] = dmem_str.size( );

			auto c_size = static_cast<std::uint32_t>( rmem_ptr[header::LAYOUT_DMEM] * 1.1 + 12 );
			auto new_buf = new char[c_size];
			auto result = compress( reinterpret_cast<unsigned char*>(new_buf),
									reinterpret_cast<uLong*>(&c_size),
									reinterpret_cast<const unsigned char*>(dmem_str.c_str( )),
									static_cast<uLong>(rmem_ptr[header::LAYOUT_DMEM]) );
			if ( result != Z_OK )
				throw ybf_exception( "Compression error", "COMPRESSION", "ybf_element::export_element", 0 );
			dmem_ptr = new_buf;
			info.size_dynamic_region = c_size;

		}
		else
		{
			info.size_dynamic_region = dmem_str.size( );
			dmem_ptr = new char[info.size_dynamic_region];
			memcpy( dmem_ptr, dmem_str.c_str( ), info.size_dynamic_region );
		}

	}
	else
	{

	}

	editor::write( stream, info );
	stream.write( reinterpret_cast<const char*>(rmem_ptr), info.size_reserved_region * sizeof( uint32_t ) );
	stream.write( dmem_ptr, info.size_dynamic_region );
	stream.write( smem_ptr, info.size_static_region );

	delete[] rmem_ptr;
	delete[] dmem_ptr;
	delete[] smem_ptr;
}
