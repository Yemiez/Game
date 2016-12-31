#include <ybf.h>
#include <osharp.h>
#include <thread>
#include <array>

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
	auto font = gui::d3d9_font{ renderer, "Verdana", gui::d3d9_font_struct{  }.enable_antialias( ) };

	gui::d3d9_string<codec> str{ "Hello World!               ", font, { 15, 15 }, gui::d3d9_colors::CadetBlue },
		withOutTrailing{ "Without trailing spaces: " + std::to_string( str.get_size( ).x ), font, { 15, 15 + (str.get_size( ).y * 3 + 10 * 3) }, gui::d3d9_colors::CadetBlue },
		withTrailing{ "With trailing spaces: " + std::to_string( str.get_size( ).x + str.get_size_of_trailing_spaces( ).x ),
					   font,
					   { 15, withOutTrailing.get_position( ).y + 5 + withOutTrailing.get_size( ).y },
					   gui::d3d9_colors::CadetBlue },
		trailingBoxColour{ "Without trailing box-colour: Red\nWith trailing box-colour: White", font, { 15, withTrailing.get_position( ).y + withTrailing.get_size( ).y + 5 }, gui::d3d9_colors::CadetBlue };
	
	window.get_event( "paint" ) += [&renderer, &str, &withOutTrailing, &withTrailing, &trailingBoxColour]( gui::event_data<codec> &ev )mutable
	{
		auto &data = reinterpret_cast<gui::window_paint_data<codec>&>(ev);
		renderer.begin_draw( [&str, &withOutTrailing, &withTrailing, &trailingBoxColour]( const gui::d3d9 &renderer )
		{
			// simply beacuse the api hasn't implemented a way to draw a box yet...
			auto draw_rectangle = []( const gui::d3d9 &r, const auto &x, const auto &y, const auto &width, const auto &height, const auto &color )
			{
				std::array<osharp::gui::d3d9_line, 4> lines
				{ {
					osharp::gui::d3d9_line{ { x, y }, { x + width, y }, 1, color }, // top left to right
					osharp::gui::d3d9_line{ { x, y }, { x, y + height }, 1, color }, // top left to bottom left
					osharp::gui::d3d9_line{ { x, y + height }, { x + width, y + height }, 1, color }, // bottom left to right
					osharp::gui::d3d9_line{ { x + width, y + height }, { x + width, y }, 1, color }
				} };
				
				for ( const auto &x : lines )
					x.draw( r );
			};
			
			str.draw( renderer );
			withOutTrailing.draw( renderer );
			withTrailing.draw( renderer );
			trailingBoxColour.draw( renderer );

			auto pos = str.get_position( );
			auto sz = str.get_size( );
			auto trail = str.get_size_of_trailing_spaces( );
			draw_rectangle( renderer, pos.x, pos.y + 10 + sz.y, sz.x, sz.y, gui::d3d9_colors::Red );
			draw_rectangle( renderer, pos.x, pos.y + 20 + sz.y * 2, sz.x + trail.x, sz.y, gui::d3d9_colors::White );
		},
							 gui::d3d9_colors::from( 42, 42, 42 ) /* Clear colour */ );
	};

	window.get_event( "moved" ) += [lastSz = gui::Vector2i{ 0, 0 }, &renderer]( gui::event_data<codec> &ev )mutable
	{
		auto &data = reinterpret_cast<gui::window_moved_data<codec>&>(ev);
		if ( lastSz != data.size )
			renderer.reset( data.sender.get_handle( ) );
		lastSz = data.size;
	};

	while ( window.poll_next( ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) ); // Let's not fry the cpu ;)

}