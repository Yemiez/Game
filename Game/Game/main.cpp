#include <ybf.h>
#include <osharp.h>
#include <thread>

void main( )
{
	namespace cvt = osharp::cvt;
	namespace fmt = osharp::formats;
	namespace gui = osharp::gui;
	namespace cnc = osharp::concurrency;
	namespace spg = osharp::spelling;
	namespace tok = osharp::tokenizing;
	using codec = cvt::codec_cvt<fmt::utf_8>;

	gui::window<codec> window{ 
		"Test game", 
		{ gui::MonitorWidth( ) / 2 - 1280 / 2, gui::MonitorHeight( ) / 2 - 720 / 2 }, 
		{ 1280, 720 },
	    gui::abstractions::WS_STYLES::ws_overlappedwindow /*& ~gui::abstractions::ws_sizebox */ };
	window.show_window( );

	gui::d3d9 renderer{ window.get_handle( ) };
	window.get_event( "paint" ) += [&renderer]( gui::event_data<codec> &ev )mutable
	{
		auto &data = reinterpret_cast<gui::window_paint_data<codec>&>(ev);
		renderer.begin_draw( []( const gui::d3d9 &renderer )
		{
			gui::d3d9_string<codec>{ "Hello World!", renderer.get_default_font( ), { 15, 15 }, gui::d3d9_colors::CadetBlue }.draw( renderer );
		},
							 gui::d3d9_colors::from( 42, 42, 42 ) /* Clear colour */ );
	};

	while ( window.poll_next( ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) ); // Let's not fry the cpu ;)

}