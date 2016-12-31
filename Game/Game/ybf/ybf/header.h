#pragma once
#include <cstdint>
#include <string>
#include "utils.h"
#include "editor.h"

namespace ybf
{

	class ybf_header
	{
	public:
		struct ybf_file;

	public:
		using int_t = std::int32_t;
		using uint_t = std::uint32_t;

	public:
		static constexpr uint_t MAGIC = 0xABCDEF;
		static constexpr uint_t STATIC_REGION = make_kilobyte( 10 );
		static constexpr uint_t RESERVED_SIZE = 10;
		static constexpr char LAYOUT_DMEM = 0;
		static constexpr char LAYOUT_SMEM = 1;
		static constexpr char LAYOUT_RMEM = 2;


	public:
		enum FileFlags : uint_t
		{
			FILE_UNICODE = make_bit( 0 ), // this file contains unicode strings	|	If neither are checked, ascii is assumed.
			FILE_ASCII = make_bit( 1 ), // this file contains ascii strings		|	If both are checked, ybf assumes the string header is 32 bits longer.
			FILE_COMPRESSED = make_bit( 2 ), //									|	If checked, ybf will decompress the elements.
			FILE_ENCRYPTED = make_bit( 3 ), //									|	If checked, ybf will decrypt the elements.
			FILE_SORTED = make_bit( 4 ), //										|	If checked, ybf will not sort the elements.
			FILE_DYNAMIC = make_bit( 5 ), //									|	If checked, ybf will read/write the dynamic region.
		};

		enum ParseFlags : uint_t
		{
			IgnoreOldVersion = make_bit( 0 ), //	|	This will ignore if the ybf is an old version, it will still parse the file. (Depending on how old it, they may be broken)
			RestoreStream = make_bit( 1 ), //		|	Restores the standard istream IF an error occurs.
			ErrorIfNewVersion = make_bit( 2 ), //	|	Will cause an error if the version we're trying to load is newer than the current version.
			Simple = IgnoreOldVersion | RestoreStream | ErrorIfNewVersion,
			IgnoreStream = IgnoreOldVersion | ErrorIfNewVersion,
			Unsafe = ErrorIfNewVersion,
		};

	public:
		ybf_header( std::istream &stream, const uint_t &flags = ParseFlags::Simple );
		ybf_header( const ybf_header& ) = delete;
		ybf_header( );
		~ybf_header( );

		void parse_header( std::istream &stream, const uint_t &flags = ParseFlags::Simple );

		bool is_valid( ) const;

		bool is_older_version( ) const;

		bool is_newer_version( ) const;

		bool is_matching_version( ) const;

		uint_t get_current_magic( ) const;

		uint_t get_file_flags( ) const;

		uint_t get_minor_version( ) const;

		uint_t get_major_version( ) const;

		uint_t get_patch_version( ) const;

		uint_t get_sizeof_header( ) const;

		uint_t get_sizeof_static_region( ) const;

		uint_t get_sizeof_dynamic_region( ) const;

		uint_t get_sizeof_reserved_region( ) const;

		uint_t get_reserved_max_idx( ) const;

		uint_t get_reserved_at( const uint_t &index ) const;

		void *static_memory_pointer( ) const;

		void *dynamic_memory_pointer( ) const;

		void introduce( std::ostream &output ) const;

	private:
		void validate_stream( std::istream &stream ) const;

		void validate_stream( std::istream &stream, std::string step, uint_t size ) const;

		void load_memory_layout( std::istream &stream );

		void read_dmem( std::istream &stream );

		void read_smem( std::istream &stream );

		void read_rmem( std::istream &stream );

	private:
		static size_t get_compressed_length( uint_t compressed_size );

		void decrypt( char **data, uint_t *size );
 
	public:
		struct ybf_file
		{
			struct header_info
			{
				struct version_struct
				{
					uint_t major,
						minor,
						patch;
				};
				uint_t magic,
					file_flags;
				version_struct version;
				uint_t size_header,
					size_static_region,
					size_dynamic_region,
					size_reserved_region;
				char layout[3];
			};
			header_info header;
			uint_t *rmem;
			void *smem,
				*dmem;

			ybf_file( )
				: rmem( nullptr ),
				smem( nullptr ),
				dmem( nullptr )
			{}
		};

	private:
		ybf_file header_;

	public:
		static constexpr uint_t HEADER_SIZE = sizeof( ybf_file );
	};


}

//	VALIDATE STREAM
//	LOAD HEADER
//	VALIDATE MEMORY
//	LAYOUT
//		DECRYPT
//			OR
//		DECOMPRESS
//			OR
//		