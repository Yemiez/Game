#include "handle.h"

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class osharp::gui::handle::handle_impl
{
public:
	handle_impl( handle_t ptr, handle_flags flag, std::function<void( void* )> dtor = nullptr )
		: ptr_( ptr ), flag_( flag ), destructor_( std::move( dtor ) )
	{
	}

	~handle_impl( )
	{
		destroy( );
	}

	void destroy( )
	{
		if ( ptr_ )
		{
			if ( destructor_ )
				destructor_( ptr_ );
			else
			{
				switch ( flag_ )
				{
					case osharp::gui::CloseFlag:
						CloseHandle( ptr_ );
						break;
					case osharp::gui::WndFlag:
						CloseWindow( *reinterpret_cast< HWND* >( ptr_ ) );
						delete reinterpret_cast< HWND* >( ptr_ );
						break;
					case osharp::gui::DeleteFlag:
						delete ptr_;
						break;
					case osharp::gui::DeleteArr:
						delete[] ptr_;
						break;
					case osharp::gui::NoFlag:
						break;
					default:
						break;
				}
			}
		}
		ptr_ = nullptr;
		flag_ = NoFlag;
		destructor_ = nullptr;
	}

public:
	handle_t ptr_;
	handle_flags flag_;
	std::function<void( handle_t )> destructor_;
};

osharp::gui::handle osharp::gui::handle::FromHWND( void * tempHWND )
{
	return handle( new HWND( *reinterpret_cast< HWND* >( tempHWND ) ), WndFlag );
}

osharp::gui::handle osharp::gui::handle::FromHNDL( void * HNDL )
{
	return handle( HNDL, CloseFlag );
}

osharp::gui::handle osharp::gui::handle::FromPTR( void * ptr )
{
	return handle( ptr, DeleteFlag );
}

osharp::gui::handle osharp::gui::handle::Duplicate( handle & other )
{
	return handle( other );
}

osharp::gui::handle::handle( )
	: impl_( new handle_impl( nullptr, NoFlag ) )
{ }

osharp::gui::handle::handle( handle_t newptr, handle_flags flag, std::function<void( handle_t )> destructor )
	: impl_( new handle_impl( newptr, flag, std::move( destructor ) ) )
{ }

osharp::gui::handle::handle( handle & other )
	: impl_( other.impl_ )
{ }

osharp::gui::handle::handle( handle && other )
	: impl_( std::move( other.impl_ ) )
{ }

const osharp::gui::handle_t osharp::gui::handle::native_ptr( ) const
{
	return impl_->ptr_;
}

osharp::gui::handle_t osharp::gui::handle::native_ptr( )
{
	return impl_->ptr_;
}

void osharp::gui::handle::set_native_ptr( handle_t ptr, handle_flags flag, std::function<void( handle_t )> dtor )
{ 
	impl_->destroy( );
	impl_->ptr_ = ptr;
	impl_->flag_ = flag;
	impl_->destructor_ = std::move( dtor );
}

bool osharp::gui::handle::empty( ) const
{
	return is_bad_handle( );
}

bool osharp::gui::handle::is_bad_handle( ) const
{
	return impl_->ptr_ == INVALID_HANDLE_VALUE || impl_->ptr_ == nullptr;
}

osharp::gui::handle::operator bool( ) const
{ 
	return !is_bad_handle( );
}

osharp::gui::handle osharp::gui::handle::Duplicate( )
{
	return handle( *this );
}

osharp::gui::handle & osharp::gui::handle::operator=( const handle & other )
{
	impl_ = other.impl_;
	return *this;
}
