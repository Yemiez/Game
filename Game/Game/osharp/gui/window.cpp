#include "window.h"
#define WIN32_LEAN_AND_MEAN
#define STRICT
#define UNICODE
#include <Windows.h>
#include <random>
#undef DefWindowProc


namespace a { namespace b { namespace c { namespace d {

	struct WndStruct
	{
		std::function<LRESULT( UINT32, WPARAM, LPARAM )> function;
		bool hasInitialized;
	};

	LRESULT CALLBACK DefaultWndProc( HWND hWnd, UINT32 Msg, WPARAM wParam, LPARAM lParam )
	{
		if ( Msg == WM_NCCREATE )
		{
			auto lpCreate = reinterpret_cast< CREATESTRUCT* >( lParam );
			auto str = new WndStruct{ *reinterpret_cast< std::function<LRESULT( UINT32, WPARAM, LPARAM )>* >( lpCreate->lpCreateParams ),
									  false };
			SetPropA( hWnd, ".", str );
			return DefWindowProcW( hWnd, Msg, wParam, lParam );
		}
		else if ( Msg == WM_NCDESTROY || Msg == WM_DESTROY )
		{
			auto wnd = reinterpret_cast<WndStruct*>( GetPropA( hWnd, "." ) );
			if ( wnd )
			{
				auto result = wnd->function( Msg, wParam, lParam );
				delete wnd;
				RemovePropA( hWnd, "." );
				return result;
			}
			return LRESULT( );
		}
		auto wnd = reinterpret_cast<WndStruct*>( GetPropA( hWnd, "." ) );
		return wnd && wnd->hasInitialized ?
			wnd->function( Msg, wParam, lParam ) :
			DefWindowProcW( hWnd, Msg, wParam, lParam );
	}

} } } }

osharp::gui::handle osharp::gui::abstractions::CreateHWND( handle parent,
														   std::wstring str, 
														   Vector2i pos, 
														   Vector2i size, 
														   std::function<long( std::uint32_t, std::uint32_t, long )> callback,
														   unsigned long dwStyle, 
														   unsigned long dwStyleEx )
{
	WNDCLASSEXW wnd;
	wnd.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = a::b::c::d::DefaultWndProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.cbSize = sizeof( WNDCLASSEXW );
	wnd.hIcon = LoadIconW( NULL, IDI_APPLICATION );
	wnd.hIconSm = wnd.hIcon;
	wnd.hCursor = LoadCursorW( NULL, IDC_ARROW );
	wnd.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
	wnd.lpszMenuName = str.c_str( );
	auto class_ = []( )->std::wstring
	{
		static std::wstring alphabet = L"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";
		static std::mt19937 rng{ std::random_device( )( ) };
		static std::uniform_int_distribution<size_t> distribution{ 0u, alphabet.length( ) };
		static std::wstring str( 17, '\0' );
		str[0] = '[';
		for ( auto i = 1u;
			  i < 15u;
			  ++i )
			str[i] = alphabet[distribution( rng )];
		str[15] = ']';
		return str;
	}( );
	wnd.lpszClassName = class_.c_str( );
	wnd.hInstance = nullptr;

	if ( FAILED( RegisterClassExW( &wnd ) ) )
		return nullptr;
	auto hwnd = CreateWindowExW( dwStyleEx,
								 wnd.lpszClassName,
								 wnd.lpszMenuName,
								 dwStyle,
								 pos.x,
								 pos.y,
								 size.x,
								 size.y,
								 parent ? *reinterpret_cast<HWND*>( parent.native_ptr( ) ) : nullptr,
								 nullptr,
								 nullptr,
								 new std::function<LRESULT( UINT32, WPARAM, LPARAM )>( callback ) );
	if ( hwnd )
		return handle::FromHWND( &hwnd );
	return nullptr;
}

osharp::gui::Vector2i osharp::gui::abstractions::MakePoints( long lParam )
{
	auto points = MAKEPOINTS( lParam );
	return{ points.x, points.y };
}

int osharp::gui::abstractions::GetWheelDelta( std::uint32_t wParam )
{
	return GET_WHEEL_DELTA_WPARAM( wParam );
}

void osharp::gui::abstractions::Finalize( handle h )
{
	if ( h )
	{
		auto wnd = reinterpret_cast< a::b::c::d::WndStruct* >( GetPropA( *reinterpret_cast< HWND* >( h.native_ptr( ) ), "." ) );
		wnd->hasInitialized = true;
	}
}

long osharp::gui::abstractions::DefWindowProc( handle & handle, std::uint32_t msg, std::uint32_t wParam, long lParam )
{
	if ( handle )
		return DefWindowProcW( *reinterpret_cast< HWND* >( handle.native_ptr( ) ), msg, wParam, lParam );
	return long( );
}

void osharp::gui::abstractions::ShowWindow( handle & handle )
{ 
	::ShowWindow( *reinterpret_cast< HWND* >( handle.native_ptr( ) ), SW_SHOW );
}

void osharp::gui::abstractions::HideWindow( handle & handle )
{
	::ShowWindow( *reinterpret_cast< HWND* >( handle.native_ptr( ) ), SW_HIDE );
}

bool osharp::gui::abstractions::PollNext( handle & handle )
{
	MSG msg{ 0, 0, 0, 0, 0, 0 };
	if ( PeekMessageW( &msg, *reinterpret_cast< HWND* >( handle.native_ptr( ) ), 0u, 0u, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessageW( &msg );
	}
	return msg.message != WM_QUIT && msg.message != 0; 
}

int osharp::gui::MonitorWidth( )
{
	return GetSystemMetrics( SM_CXSCREEN );
}

int osharp::gui::MonitorHeight( )
{
	return GetSystemMetrics( SM_CYSCREEN );
}

osharp::gui::Vector2i osharp::gui::MonitorSize( )
{
	return{ MonitorWidth( ), MonitorHeight( ) };
}
