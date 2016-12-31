#pragma 
#include "../concurrency/functional.h"
#include "handle.h"
#include "dimensions.h"
#include "../constexpr/string.h"
#include "popup.h"
#include <unordered_map>


namespace osharp { namespace gui {

	namespace abstractions
		// an abstraction layer is made between windows and
		// osharp beacuse we don't want to pollute the global namespace.
		// But also because, abstraction makes porting the framework to other
		// operating systems simpler.
	{

		enum WS_STYLES : unsigned long
		{
			None = 0L,
			ws_overlapped = 0L,
			ws_popup = 0x80000000L,
			ws_child = 0x40000000L,
			ws_minimize = 0x20000000L,
			ws_visible = 0x10000000L,
			ws_disabled = 0x08000000L,
			ws_clipsiblings = 0x04000000L,
			ws_clipchildren = 0x02000000L,
			ws_maximize = 0x01000000L,
			ws_caption = 0x00C00000L,
			ws_border = 0x00800000L,
			ws_dlgframe = 0x00400000L,
			ws_vscroll = 0x00200000L,
			ws_hscroll = 0x00100000L,
			ws_sysmenu = 0x00080000L,
			ws_thickframe = 0x00040000L,
			ws_group = 0x00020000L,
			ws_tabstop = 0x00010000L,
			ws_minimizebox = 0x00020000L,
			ws_maximizebox = 0x00010000L,
			ws_tiled = ws_overlapped,
			ws_iconic = ws_minimize,
			ws_sizebox = ws_thickframe,
			ws_overlappedwindow = ws_overlapped | ws_caption | ws_sysmenu | ws_thickframe | ws_minimizebox | ws_maximizebox,
			ws_tiledwindow = ws_overlappedwindow,
			ws_popupwindow = ws_popup | ws_border | ws_sysmenu,
			ws_childwindow = ws_child,
			ws_ex_dlgmodalframe = 0x00000001L,
			ws_ex_noparentnotify = 0x00000004L,
			ws_ex_topmost = 0x00000008L,
			ws_ex_acceptfiles = 0x00000010L,
			ws_ex_transparent = 0x00000020L,
			ws_ex_right = 0x00001000L,
			ws_ex_left = 0x00000000L,
			ws_ex_rtlreading = 0x00002000L,
			ws_ex_ltrreading = 0x00000000L,
			ws_ex_leftscrollbar = 0x00004000L,
			ws_ex_rightscrollbar = 0x00000000L,
			ws_ex_controlparent = 0x00010000L,
			ws_ex_staticedge = 0x00020000L,
			ws_ex_appwindow = 0x00040000L,			
		};

		enum WM_EVENTS
		{
			wm_paint = 0x000F,
			wm_quit = 0x0012,
			wm_keydown = 0x0100,
			wm_keyup = 0x0101,
			wm_mousemove = 0x0200,
			wm_move = 0x0003,
			wm_size = 0x0005,
			wm_lbuttondown = 0x201,
			wm_lbuttonup = 0x0202,
			wm_lbuttondblclk = 0x0203,
			wm_rbuttondown = 0x204,
			wm_rbuttonup = 0x0205,
			wm_rbuttondblclk = 0x0206,
			wm_mousewheel = 0x020A,
			wm_char = 0x0102,
			wm_close = 0x0010,
			wm_destroy = 0x0002,
		};

		extern handle CreateHWND( handle parent_handle,
								  std::wstring str,
								  Vector2i pos,
								  Vector2i size,
								  std::function<long(std::uint32_t, std::uint32_t, long)> callback,
								  unsigned long dwStyle = ws_overlappedwindow, 
								  unsigned long dwStyleEx = None );

		extern Vector2i MakePoints( long lParam );

		extern int GetWheelDelta( std::uint32_t wParam );

		extern void Finalize( handle );

		extern long DefWindowProc( handle &handle, std::uint32_t msg, std::uint32_t wParam, long lParam );

		extern void ShowWindow( handle &handle );

		extern void HideWindow( handle &handle );

		extern bool PollNext( handle &handle );

	}

	enum key : int
	{
		key_leftButton = 1,
		key_rightButton,
		key_reserved_01,
		key_middleButton,
		key_xbutton,
		key_xbutton2,
		key_reserved_02,
		key_backspace,
		key_tab,
		key_reserved_03 = 0x0A,
		key_clear = 0x0C,
		key_enter = 0x0D,
		key_shift = 0x10,
		key_control = 0x11,
		key_alt = 0x12,
		key_pause = 0x13,
		key_caps = 0x14,
		key_escape = 0x1B,
		key_space = 0x20,
		key_pgup = 0x21,
		key_pgdn = 0x22,
		key_end = 0x23,
		key_home = 0x24,
		key_left = 0x25,
		key_up = 0x26,
		key_right = 0x27,
		key_down = 0x28,
		key_insert = 0x2D,
		key_delete = 0x2E,
		key_help = 0x2F,
		key_0 = 0x30,
		key_1 = 0x31,
		key_2 = 0x32,
		key_3 = 0x33,
		key_4 = 0x34,
		key_5 = 0x35,
		key_6 = 0x36,
		key_7 = 0x37,
		key_8 = 0x38,
		key_9 = 0x39,
		key_a = 0x41,
		key_b = 0x42,
		key_c = 0x43,
		key_d = 0x44,
		key_e = 0x45,
		key_f = 0x46,
		key_g = 0x47,
		key_h = 0x48,
		key_i = 0x49,
		key_j = 0x4A,
		key_k = 0x4B,
		key_l = 0x4C,
		key_m = 0x4D,
		key_n = 0x4E,
		key_o = 0x4F,
		key_p = 0x50,
		key_q = 0x51,
		key_r = 0x52,
		key_s = 0x53,
		key_t = 0x54,
		key_u = 0x55,
		key_v = 0x56,
		key_w = 0x57,
		key_x = 0x58,
		key_y = 0x59,
		key_z = 0x5A,
		key_leftWindows = 0x5B,
		key_rightWindows = 0x5C,
		key_num0 = 0x60,
		key_num1 = 0x61,
		key_num2 = 0x62,
		key_num3 = 0x63,
		key_num4 = 0x64,
		key_num5 = 0x65,
		key_num6 = 0x66,
		key_num7 = 0x67,
		key_num8 = 0x68,
		key_num9 = 0x69,
		key_F1 = 0x70,
		key_F2 = 0x71,
		key_F3 = 0x72,
		key_F4 = 0x73,
		key_F5 = 0x74,
		key_F6 = 0x75,
		key_F7 = 0x76,
		key_F8 = 0x77,
		key_F9 = 0x78,
		key_F10 = 0x79,
		key_F11 = 0x7A,
		key_F12 = 0x7B,
		key_F13 = 0x7C,
		key_F14 = 0x7D,
		key_F15 = 0x7E,
		key_numlock = 0x90,
		key_leftShift = 0xA1,
		key_leftControl = 0xA2,
		key_rightControl = 0xA3,
		key_comma = 0xBC,
		key_minus = 0xBD,
		key_dot = 0xBE
	};

	extern int MonitorWidth( );

	extern int MonitorHeight( );

	extern Vector2i MonitorSize( );
	

	template<typename _Codec = cvt::codec_cvt<>>
	class window;

	template<typename _Codec = cvt::codec_cvt<>>
	struct event_data
	{
		event_data( window<_Codec> &sender, std::uint32_t msg, std::uint32_t wparam, long lparam )
			: sender( sender ), Msg( msg ), wParam( wparam ), lParam( lparam )
		{ } 
		window<_Codec> &sender;
		std::uint32_t Msg, wParam;
		long lParam;
	};

	template<typename _Codec = cvt::codec_cvt<>>
	struct window_moved_data
		: public event_data<_Codec>
	{
		window_moved_data( window<_Codec> &sender, std::uint32_t msg, std::uint32_t wparam, long lparam,
						   Vector2i position, Vector2i size )
			: event_data<_Codec>( sender, msg, wparam, lparam ), position( position ), size( size )
		{ }
		Vector2i position, size;
	};


	template<typename _Codec = cvt::codec_cvt<>>
	struct window_closing_data
		: public event_data<_Codec>
	{
		window_closing_data( window<_Codec> &sender, std::uint32_t msg, std::uint32_t wparam, long lparam,
						   bool shouldClose )
			: event_data<_Codec>( sender, msg, wparam, lparam ), ShouldClose( shouldClose )
		{ }
		bool ShouldClose;
	};

	template<typename _Codec = cvt::codec_cvt<>>
	struct window_closed_data
		: public event_data<_Codec>
	{
		window_closed_data( window<_Codec> &sender, std::uint32_t msg, std::uint32_t wparam, long lparam )
			: event_data<_Codec>( sender, msg, wparam, lparam )
		{ }
	};

	template<typename _Codec = cvt::codec_cvt<>>
	struct window_key_data
		: public event_data<_Codec>
	{
		window_key_data( window<_Codec> &sender, std::uint32_t msg, std::uint32_t wparam, long lparam,
						 bool isRelease, bool isPress, int code, bool shift,
						 bool ctrl )
			: event_data<_Codec>( sender, msg, wparam, lparam ), isRelease( isRelease ),
			isPress( isPress ), code( code ), shift( shift ), ctrl( ctrl )
		{ }
		bool isRelease, isPress;
		int code;
		bool shift, ctrl;
	};

	template<typename _Codec = cvt::codec_cvt<>>
	struct window_char_data
		: public event_data<_Codec>
	{
		window_char_data( window<_Codec> &sender, std::uint32_t msg, std::uint32_t wparam, long lparam,
						  typename _Codec::output_format key )
			: event_data<_Codec>( sender, msg, wparam, lparam ), key( key )
		{ }
		typename _Codec::output_format key;
	};

	template<typename _Codec = cvt::codec_cvt<>>
	struct window_mouse_data
		: public event_data<_Codec>
	{
		window_mouse_data( window<_Codec> &sender, std::uint32_t msg, std::uint32_t wparam, long lparam,
						   bool isRelease, bool isPress, bool isScroll, int code,
						   int delta, bool shift, bool ctrl, bool isDbl,
						   Vector2i position )
			: event_data<_Codec>( sender, msg, wparam, lparam ), isRelease( isRelease ),
			isPress( isPress ), isScroll( isScroll ), code( code ),
			delta( delta ), shift( shift ), ctrl( ctrl ), isDbl( isDbl ),
			position( position )
		{ }
		bool isRelease, isPress, isScroll;
		int code,
			delta; // positive delta = scroll up, negative delta = down
		bool shift, ctrl,
			isDbl;
		Vector2i position;
	};

	template<typename _Codec = cvt::codec_cvt<>>
	struct window_paint_data
		: public event_data<_Codec>
	{
		window_paint_data( window<_Codec> &sender, std::uint32_t msg, std::uint32_t wparam, long lparam )
			: event_data<_Codec>( sender, msg, wparam, lparam )
		{ }
	};

	template<typename _Codec>
	class window
	{
	public:
		using codec_cvt = _Codec;
		using self_type = window<codec_cvt>;
		using string_type = typename codec_cvt::string_type;
		using main_codec = cvt::codec_cvt<formats::utf_w>;
		using event_data = event_data<codec_cvt>;
		using window_moved_data = window_moved_data<codec_cvt>;
		using window_closing_data = window_closing_data<codec_cvt>;
		using window_closed_data = window_closed_data<codec_cvt>;
		using window_key_data = window_key_data<codec_cvt>;
		using window_char_data = window_char_data<codec_cvt>;
		using window_mouse_data = window_mouse_data<codec_cvt>;
		using window_paint_data = window_paint_data<codec_cvt>;

	private:
		struct cexpr_init_val
		{
			struct helper
			{
				template<typename T, std::size_t N, std::size_t...Indices>
				static constexpr std::array<T, N> make( constexprs::details::index_tuple<Indices...> )
				{
					return{ { []( std::size_t )
				              { return false; }( Indices )... } };		
				}
			};

			template<typename T, std::size_t N>
			static constexpr std::array<T, N> make( std::array<T, N> const & )
			{
				return helper::make<T, N>( typename constexprs::details::index_range<0, N - 1>::type( ) );
			}
		};

	public:
		window( string_type title, 
				Vector2i position, 
				Vector2i size,
				unsigned long dwStyle = abstractions::ws_overlappedwindow,
				unsigned long dwStyleEx = abstractions::None )
			: title_( std::move( title ) ),
			pos_( std::move( position ) ),
			size_( std::move( size ) ),
			style_( dwStyle ),
			style_ex_( dwStyleEx ),
			keys_{ cexpr_init_val::make( keys_ ) }
		{
			try_create( );
		}

		window( window &parent,
				string_type title,
				Vector2i position,
				Vector2i size,
				unsigned long dwStyle = abstractions::ws_overlappedwindow,
				unsigned long dwStyleEx = abstractions::None )
			: title_( std::move( title ) ),
			pos_( std::move( position ) ),
			size_( std::move( size ) ),
			style_( dwStyle ),
			style_ex_( dwStyleEx ),
			parent_( &parent ),
			keys_{ cexpr_init_val::make( keys_ ) }
		{ }

		window( window *parent, 
				string_type title, 
				Vector2i position, 
				Vector2i size,
				unsigned long dwStyle = abstractions::ws_overlappedwindow,
				unsigned long dwStyleEx = abstractions::None )
			: window( *parent, std::move( title ), std::move( position ), std::move( size ), dwStyle, dwStyleEx )
		{ }

		window( )
			: title_( ),
			pos_( ),
			size_( ),
			style_( abstractions::ws_overlappedwindow ),
			style_ex_( abstractions::None ),
			parent_( nullptr ),
			keys_{ cexpr_init_val::make( keys_ ) }
		{ }

		const handle &get_handle( ) const
		{
			return handle_;
		}

		handle &get_handle( )
		{
			return handle_;
		}

		void show_window( )
		{
			return handle_ ?
				abstractions::ShowWindow( handle_ ) :
				void( );
		}

		void hide_window( )
		{
			return handle_ ?
				abstractions::HideWindow( handle_ ) :
				void( );
		}

		void enable_paint_event( )
		{
			pev_ = true;
		}

		void disable_paint_event( )
		{
			pev_ = true;
		}

		bool poll_next( )
		{
			return handle_ ? 
				abstractions::PollNext( handle_ ) :
				false;
		}

		DialogResult MessageBox( const string_type &message,
								 const string_type &caption,
								 long type = YesNo )
		{
			return MsgBox<codec_cvt>::ShowDialog( handle_,
												  message,
												  caption,
												  type );
		}

	public:
		bool try_create( )
		{
			handle_ = abstractions::CreateHWND( parent_ ? parent_->handle_ : nullptr,
				                                main_codec::cvt( title_ ),
												pos_,
												size_, 
												[this]( std::uint32_t m, std::uint32_t w, long l )
			{
				return handle_message( m, w, l );
			},
												style_,
				                                style_ex_ );
			/*
				msg_ev_,
				paint_ev_,
				closing_ev_,
				closed_ev_,
				key_ev_,
				char_ev_,
				mouse_ev_,
				moved_ev_;
			*/
			
			abstractions::Finalize( handle_ );
			events_["message"] = &msg_ev_;
			events_["paint"] = &paint_ev_;
			events_["closing"] = &closing_ev_;
			events_["closed"] = &closed_ev_;
			events_["moved"] = &moved_ev_;
			events_["key"] = &key_ev_;
			events_["char"] = &char_ev_;
			events_["mouse"] = &mouse_ev_;
			return !handle_.is_bad_handle( );
		}

		concurrency::event<void( event_data& ), codec_cvt> &get_event( const string_type &event )
		{
			auto it = events_.find( event );
			if ( it == events_.end( ) )
				throw;
			return *it->second;
		}

		long handle_message( std::uint32_t msg, std::uint32_t wParam, long lParam )
		{
			event_data data{ *this, msg, wParam, lParam };
			auto points = abstractions::MakePoints( lParam );
			switch ( msg )
			{
				case osharp::gui::abstractions::wm_paint:
					if ( pev_ )
						paint_ev_( data );
					return 0;
					break;
				case osharp::gui::abstractions::wm_keydown:
					key_ev_( window_key_data{ *this, msg, wParam,
							 lParam, false, true,
							 static_cast< int >( wParam ), keys_[key_shift], keys_[key_control] } );
					return 0;
					break;
				case osharp::gui::abstractions::wm_keyup:
					key_ev_( window_key_data{ *this, msg, wParam,
							 lParam, true, false,
							 static_cast< int >( wParam ), keys_[key_shift], keys_[key_control] } );
					return 0;
					break;
				case osharp::gui::abstractions::wm_mousemove:
					mouse_ev_( window_mouse_data{ *this, msg, wParam,
							   lParam, false, false,
                               false, static_cast< int >( wParam ), 0, 
							   keys_[key_shift], keys_[key_control], false,
					           points } );
					return 0;
					break;
				case osharp::gui::abstractions::wm_move:
					pos_ = points;
					moved_ev_( window_moved_data{ *this, msg, wParam,
							   lParam, pos_, size_ } );
					return 0;
					break;
				case osharp::gui::abstractions::wm_size:
					size_ = points;
					moved_ev_( window_moved_data{ *this, msg, wParam,
							   lParam, pos_, size_ } );
					return 0;
					break;
				case osharp::gui::abstractions::wm_lbuttondown:
					mouse_ev_( window_mouse_data{ *this, msg, wParam,
							   lParam, false, true,
							   false, key_leftButton, 0,
							   keys_[key_shift], keys_[key_control], false,
					           points } );
					return 0;
					break;
				case osharp::gui::abstractions::wm_lbuttonup:
					mouse_ev_( window_mouse_data{ *this, msg, wParam,
							   lParam, true, false,
							   false, key_leftButton, 0,
							   keys_[key_shift], keys_[key_control], false,
					           points } );
					return 0;
					break;
				case osharp::gui::abstractions::wm_lbuttondblclk:
					mouse_ev_( window_mouse_data{ *this, msg, wParam,
							   lParam, false, true,
							   false, key_leftButton, 0,
							   keys_[key_shift], keys_[key_control], true,
					           points } );
					return 0;
					break;
				case osharp::gui::abstractions::wm_rbuttondown:
					mouse_ev_( window_mouse_data{ *this, msg, wParam,
							   lParam, false, true,
							   false, key_rightButton, 0,
							   keys_[key_shift], keys_[key_control], false,
					           points } );
					return 0;
					break;
				case osharp::gui::abstractions::wm_rbuttonup:
					mouse_ev_( window_mouse_data{ *this, msg, wParam,
							   lParam, true, false,
							   false, key_rightButton, 0,
							   keys_[key_shift], keys_[key_control], false,
					           points } );
					return 0;
					break;
				case osharp::gui::abstractions::wm_rbuttondblclk:
					mouse_ev_( window_mouse_data{ *this, msg, wParam,
							   lParam, false, true,
							   false, key_rightButton, 0,
							   keys_[key_shift], keys_[key_control], true,
					           points } );
					return 0;
					break;
				case osharp::gui::abstractions::wm_mousewheel:
					mouse_ev_( window_mouse_data{ *this, msg, wParam,
							   lParam, false, false,
							   true, 0, abstractions::GetWheelDelta( wParam ),
							   keys_[key_shift], keys_[key_control], false,
					           points } );
					return 0;
					break;
				case osharp::gui::abstractions::wm_char:
					char_ev_( window_char_data{ *this, msg, wParam,
							  lParam, codec_cvt::cvt( wParam ) } );
					return 0;
					break;
				case osharp::gui::abstractions::wm_close:
				{
					window_closing_data data{ *this, msg, wParam, lParam, true };
					closing_ev_( data );
					if ( !data.ShouldClose )
						return 0;
				}
					break;
				case osharp::gui::abstractions::wm_destroy:
					closed_ev_( data );
					break;
				default:
					break;
			}
			return abstractions::DefWindowProc( handle_, msg, wParam, lParam );
		}

	private:
		string_type title_;
		Vector2i pos_, size_;
		unsigned long style_, style_ex_;
		handle handle_;
		window *parent_;
		std::unordered_map<string_type, concurrency::event<void( event_data& ), codec_cvt>*> events_;
		bool pev_ = true;
		std::array<bool, 255> keys_;

		// events
	private:
		concurrency::event<void( event_data& ), codec_cvt> msg_ev_,
			paint_ev_,
			closing_ev_,
			closed_ev_,
			key_ev_,
			char_ev_,
			mouse_ev_,
			moved_ev_;

	};

} }