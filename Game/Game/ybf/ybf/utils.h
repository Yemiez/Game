#pragma once
#include <string>

namespace ybf
{
	static constexpr std::uint32_t MAJOR_VERSION = 0;
	static constexpr std::uint32_t MINOR_VERSION = 1;
	static constexpr std::uint32_t PATCH_VERSION = 13;

	static constexpr auto make_bit(std::uint32_t mask)->std::uint32_t
	{
		return (1 << mask);
	}

	static constexpr auto make_bytes(std::uint32_t amount)->std::uint32_t 
	{
		return amount;
	}

	static constexpr auto make_kilobyte(std::uint32_t amount)->std::uint32_t
	{
		return amount << 10;
	}

	static constexpr auto make_megabytes(std::uint32_t amount)->std::uint32_t
	{
		return amount << 20;
	}


	class ybf_exception : public std::exception
	{
	public:
		ybf_exception( std::string message,
					   std::string step,
					   std::string function,
					   std::uint32_t sp )
			: message_( std::move( message ) ),
			step_( std::move( step ) ),
			function_( std::move( function ) ),
			fixed_(),
			sp_( sp )
		{ fix( ); }
		ybf_exception( )
		{}


		virtual char const* what( ) const override
		{
			return fixed_.c_str( );
		}

	private:
		void fix( )
		{
			fixed_ = "YBF exception occured at '" + step_ + "' in function '" + function_ + "'.\n";
			fixed_ += "\tat stream location: " + std::to_string( sp_ ) + ".\n";
			fixed_ += "\t'" + message_ + "'.";
		}

	private:
		std::string message_,
			step_,
			function_,
			fixed_;
		std::uint32_t sp_;
	};

	template<typename Stream>
	void introduce( Stream &str )
	{
		str << "YBF Version " << MAJOR_VERSION << "." << MINOR_VERSION << "." << PATCH_VERSION << '\n';
	}



	namespace types
	{
		static constexpr auto TYPE_INT = make_bit( 0 );
		static constexpr auto TYPE_FLOAT = make_bit( 1 );
		static constexpr auto TYPE_DOUBLE = make_bit( 2 );
		static constexpr auto TYPE_SHORT = make_bit( 3 );
		static constexpr auto TYPE_UINT = make_bit( 4 );
		static constexpr auto TYPE_LONG = make_bit( 5 );
		static constexpr auto TYPE_ULONG = make_bit( 6 );
		static constexpr auto TYPE_ASCII_STRING = make_bit( 7 );
		static constexpr auto TYPE_UNICODE_STRING = make_bit( 8 );
		static constexpr auto TYPE_BUNDLE = make_bit( 9 );
		static constexpr auto TYPE_BYTE_ARRAY = make_bit( 10 );
	}
}