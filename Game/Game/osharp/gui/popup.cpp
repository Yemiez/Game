#include "popup.h"
#include <Windows.h>

osharp::gui::DialogResult osharp::gui::abstractions::MessageBoxW( handle parent, std::wstring message, std::wstring caption, long type )
{
	return DialogResult( ::MessageBoxW( parent ? *reinterpret_cast< HWND* >( parent.native_ptr( ) ) : nullptr,
										message.c_str( ),
										caption.c_str( ),
										type ) );
}
