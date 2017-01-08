#include <ybf.h>
#include <osharp.h>
#include <thread>
#include <array>
#include <queue>
#include "game/Game.h"



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
		(gui::abstractions::WS_STYLES::ws_overlappedwindow & ~gui::abstractions::ws_sizebox) };
	window.show_window( );

	gui::d3d9 renderer{ window.get_handle( ), false  /* vsync */ };

	Game game{ window, renderer };

	while ( window.poll_next( ) )
		game.update( renderer );
}

