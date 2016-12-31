#include "header.h"
#include <random>
#include <zlib.h>
#include <memory>

ybf::ybf_header::ybf_header( std::istream &stream, const uint_t & flags )
{
	parse_header( stream, flags );
}

ybf::ybf_header::ybf_header( )
{}

ybf::ybf_header::~ybf_header( )
{
	if ( is_valid( ) )
	{
		if ( header_.dmem )
			delete[] header_.dmem;
		if ( header_.rmem )
			delete[] header_.rmem;
		if ( header_.smem )
			delete[] header_.smem;
	}
}

void ybf::ybf_header::parse_header( std::istream &stream, const uint_t & flags )
{
	auto save = editor::save( stream );
	try
	{
		validate_stream( stream );
		editor::read( stream, header_.header );

		if ( !is_valid( ) )
			throw ybf_exception( "Invalid magic constant", "HEADER", "ybf_header::parse_header", stream.tellg( ) );

		if ( flags & ParseFlags::ErrorIfNewVersion &&
			 is_newer_version( ) )
			throw ybf_exception( "Version mismatch (flag denies newer versions)", "HEADER", "ybf_header::parse_header", stream.tellg( ) );

		if ( !(flags & ParseFlags::IgnoreOldVersion) &&
			 is_older_version( ) )
			throw ybf_exception( "Version mismatch (flag denies older versions)", "HEADER", "ybf_header::parse_header", stream.tellg( ) );

		validate_stream( stream, "Validate memory", header_.header.size_dynamic_region + header_.header.size_reserved_region + header_.header.size_static_region );
		load_memory_layout( stream );
	}
	catch ( const ybf_exception &e )
	{
		if ( flags & ParseFlags::RestoreStream )
			editor::restore( stream, save );
		throw e;
	}
}

bool ybf::ybf_header::is_valid( ) const
{
	return header_.header.magic == MAGIC;
}

bool ybf::ybf_header::is_older_version( ) const
{
	if ( header_.header.version.major < MAJOR_VERSION )
		return true;
	else if ( header_.header.version.major > MAJOR_VERSION )
		return false;
	else
	{
		if ( header_.header.version.minor < MINOR_VERSION )
			return true;
		else if ( header_.header.version.patch < PATCH_VERSION )
			return true;
	}
	return false;
}

bool ybf::ybf_header::is_newer_version( ) const
{
	if ( header_.header.version.major < MAJOR_VERSION )
		return false;
	else if ( header_.header.version.major > MAJOR_VERSION )
		return true;
	else
	{
		if ( header_.header.version.minor > MINOR_VERSION )
			return true;
		else if ( header_.header.version.patch > PATCH_VERSION )
			return true;
	}
	return false;
}

bool ybf::ybf_header::is_matching_version( ) const
{
	return is_valid( ) &&
		header_.header.version.major == MAJOR_VERSION &&
		header_.header.version.minor == MINOR_VERSION && 
		header_.header.version.patch == PATCH_VERSION;
}

ybf::ybf_header::uint_t ybf::ybf_header::get_current_magic( ) const
{
	return header_.header.magic;
}

ybf::ybf_header::uint_t ybf::ybf_header::get_file_flags( ) const
{
	return header_.header.file_flags;
}

ybf::ybf_header::uint_t ybf::ybf_header::get_minor_version( ) const
{
	return header_.header.version.minor;
}

ybf::ybf_header::uint_t ybf::ybf_header::get_major_version( ) const
{
	return header_.header.version.major;
}

ybf::ybf_header::uint_t ybf::ybf_header::get_patch_version( ) const
{
	return header_.header.version.patch;
}

ybf::ybf_header::uint_t ybf::ybf_header::get_sizeof_header( ) const
{
	return header_.header.size_header;
}

ybf::ybf_header::uint_t ybf::ybf_header::get_sizeof_static_region( ) const
{
	return header_.header.size_static_region;
}

ybf::ybf_header::uint_t ybf::ybf_header::get_sizeof_dynamic_region( ) const
{
	return header_.header.size_dynamic_region;
}

ybf::ybf_header::uint_t ybf::ybf_header::get_sizeof_reserved_region( ) const
{
	return header_.header.size_reserved_region * sizeof( uint_t );
}

ybf::ybf_header::uint_t ybf::ybf_header::get_reserved_max_idx( ) const
{
	return header_.header.size_reserved_region;
}

ybf::ybf_header::uint_t ybf::ybf_header::get_reserved_at( const uint_t & index ) const
{
	return (index >= 0 &&
			 index < header_.header.size_reserved_region) ?
		header_.rmem[index] :
		throw ybf_exception( "Index out of ranges", "[~]", "ybf_header::get_reserved_at", 0 );
}

void * ybf::ybf_header::static_memory_pointer( ) const
{
	return header_.smem;
}

void * ybf::ybf_header::dynamic_memory_pointer( ) const
{
	return header_.dmem;
}

void ybf::ybf_header::introduce( std::ostream & output ) const
{
	output << "Header information: " << std::endl;
	output << "\tis valid: " << std::boolalpha << this->is_valid( ) << std::endl;
	output << "\tis matching version: " << this->is_matching_version( ) << std::endl;
	output << "\tis older version: " << this->is_older_version( ) << std::endl;
	output << "\tis newer version: " << this->is_newer_version( ) << std::endl;
	output << "\tMagic: " << this->get_current_magic( ) << std::endl;
	output << "\tFile flags: " << this->get_file_flags( ) << std::endl;
	output << "\tVersion: " << this->get_major_version( ) << "." << this->get_minor_version( ) << "." << this->get_patch_version( ) << std::endl;
	output << "\tDynamic region size: " << this->get_sizeof_dynamic_region( ) << std::endl;
	output << "\tStatic region size: " << this->get_sizeof_static_region( ) << std::endl;
	output << "\tReserved region size: " << this->get_sizeof_reserved_region( ) << std::endl;
	output << "\tPage(s) size: " << this->get_sizeof_dynamic_region( ) +
		this->get_sizeof_static_region( ) +
		this->get_sizeof_reserved_region( ) << std::endl;
}

void ybf::ybf_header::validate_stream( std::istream &stream ) const
{
	auto off = stream.tellg( );
	stream.seekg( 0, std::ios_base::end );
	if ( (stream.tellg( ) - off) < sizeof( ybf_file::header_info ) )
	{
		stream.seekg( off );
		throw ybf_exception( "Stream is invalid (too few bytes)", "Stream validation", "ybf_header::validate_stream", off );
	}
	stream.seekg( off );
}

void ybf::ybf_header::validate_stream( std::istream &stream, std::string step, uint_t size ) const
{
	auto off = stream.tellg( );
	stream.seekg( 0, std::ios_base::end );
	if ( (stream.tellg( ) - off) < size )
	{
		stream.seekg( off );
		throw ybf_exception( "Stream is invalid (too few bytes)", "Stream validation", "ybf_header::validate_stream", off );
	}
	stream.seekg( off );
}

void ybf::ybf_header::load_memory_layout( std::istream &stream )
{
	for ( auto i = 0u;
		  i < sizeof( header_.header.layout );
		  ++i )
	{
		switch ( header_.header.layout[i] )
		{
			case LAYOUT_DMEM:
				read_dmem( stream );
				break;
			case LAYOUT_RMEM:
				read_rmem( stream );
				break;
			case LAYOUT_SMEM:
				read_smem( stream );
				break;
			default:
				throw ybf_exception( "Error parsing memory layout", "LAYOUT", "ybf_header::load_memory_layout", stream.tellg( ) );
		}
	}
}

void ybf::ybf_header::read_dmem( std::istream &stream )
{
	if ( header_.header.file_flags & FileFlags::FILE_DYNAMIC )
	{
		auto buffer = new char[header_.header.size_dynamic_region];
		stream.read( buffer, header_.header.size_dynamic_region );

		if ( header_.header.file_flags & FileFlags::FILE_ENCRYPTED )
			decrypt( &buffer, &header_.header.size_dynamic_region );
		if ( header_.header.file_flags & FileFlags::FILE_COMPRESSED )
		{
			auto original_size = &header_.rmem[LAYOUT_DMEM];
			if ( *original_size == 0 )
			{
				delete[] buffer;
				return;
			}
			auto new_buffer = new char[*original_size];
			auto result = uncompress( reinterpret_cast<Byte*>(new_buffer),
									  reinterpret_cast<uLongf*>(original_size),
									  reinterpret_cast<Byte*>(buffer),
									  header_.header.size_dynamic_region );
			if ( result != Z_OK )
				throw ybf_exception( "Error decompressing memory region", "DECOMPRESSION", "ybf_header::decompress", 0 );
			
			delete[] buffer;
			header_.dmem = new_buffer;
			header_.header.size_dynamic_region = *original_size;
			return;
		}
		header_.dmem = reinterpret_cast<void*>(buffer);
	}
}

void ybf::ybf_header::read_smem( std::istream &stream )
{
	auto buffer = new char[header_.header.size_static_region];
	stream.read( buffer, header_.header.size_static_region );

	if ( header_.header.file_flags & FileFlags::FILE_ENCRYPTED )
		decrypt( &buffer, &header_.header.size_static_region );
	if ( header_.header.file_flags & FileFlags::FILE_COMPRESSED )
	{
		auto original_size = &header_.rmem[LAYOUT_SMEM];
		if ( *original_size == 0 )
		{
			delete[] buffer;
			return;
		}

		auto new_buffer = new char[*original_size];
		auto result = uncompress( reinterpret_cast<Byte*>(new_buffer),
								  reinterpret_cast<uLongf*>(original_size),
								  reinterpret_cast<Byte*>(buffer),
								  header_.header.size_static_region );
		if ( result != Z_OK )
			throw ybf_exception( "Error decompressing memory region", "DECOMPRESSION", "ybf_header::decompress", 0 );

		delete[] buffer;
		header_.smem = new_buffer;
		header_.header.size_static_region = *original_size;
		return;
	}	
	header_.smem = reinterpret_cast<void*>(buffer);
}

void ybf::ybf_header::read_rmem( std::istream &stream )
{
	auto buffer = new uint_t[header_.header.size_reserved_region];
	stream.read( reinterpret_cast<char*>( buffer ), header_.header.size_reserved_region * sizeof( uint_t ) );
	header_.rmem = buffer;
}

size_t ybf::ybf_header::get_compressed_length( uint_t compressed_size )
{
	return (compressed_size * 1.1) + 12;
}



void ybf::ybf_header::decrypt( char ** data, uint_t *size )
{}
