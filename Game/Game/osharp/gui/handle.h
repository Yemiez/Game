#pragma once
#include <memory>
#include <functional>

namespace osharp { namespace gui {

	using handle_t = void*;

	enum handle_flags
	{
		CloseFlag,
		WndFlag,
		DeleteFlag,
		DeleteArr,
		NoFlag,
	};

	class handle
	{
	private:
		class handle_impl;

	public:
		static handle FromHWND( void *tempHWND ); // HWND, calls CloseWindow
		static handle FromHNDL( void *HNDL ); // just a regular HANDLE (void*) type. Calls CloseHandle
		static handle FromPTR( void *ptr ); // heap memory block with NO destructor
		static handle Duplicate( handle &other );

		template<typename T>
		static handle FromPTR( T *ptr ) // heap pointer
		{
			return handle( ptr, DeleteFlag, []( void *ptr )
			{
				delete reinterpret_cast<T*>( ptr );
			} );
		}

		template<typename T>
		static handle FromPTR( T *ptr, std::size_t size ) // heap array
		{
			return handle( ptr, NoFlag, []( void *arr )
			{
				delete[] reinterpret_cast<T*>( arr );
			} );
		}

	public:
		handle( );
		handle( handle_t newptr, handle_flags flag = CloseFlag, std::function<void( handle_t )> = nullptr );
		handle( handle &other );
		handle( handle &&other );
		
		const handle_t native_ptr( ) const;
		
		handle_t native_ptr( );

		void set_native_ptr( handle_t ptr, handle_flags = CloseFlag, std::function<void( handle_t )> = nullptr );

		template<typename T>
		void set_native_ptr( T *ptr ) // heap pointer
		{
			return set_native_ptr( ptr, DeleteFlag, []( void *ptr )
			{
				delete reinterpret_cast<T*>( ptr );
			} );
		}

		template<typename T>
		void set_native_ptr( T *ptr, std::size_t size ) // heap array
		{
			return set_native_ptr( ptr, NoFlag, []( void *arr )
			{
				delete[] reinterpret_cast<T*>( arr );
			} );
		}

		bool empty( ) const;

		bool is_bad_handle( ) const;

		operator bool( ) const;

		handle Duplicate( );

	public:
		handle &operator=( const handle &other );

	private:
		std::shared_ptr<handle_impl> impl_;
	};

} }