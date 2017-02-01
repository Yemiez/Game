#include "Game.h"
#include <sstream>

namespace cvt = osharp::cvt;
namespace fmt = osharp::formats;
namespace gui = osharp::gui;
namespace cnc = osharp::concurrency;
namespace spg = osharp::spelling;
namespace tok = osharp::tokenizing;

Game::Game( osharp::gui::window<codec>& window, const osharp::gui::d3d9 & gfx )
	: font_( gfx, "Verdana", gfx.get_default_font( ).get_parameters( ).enable_antialias( ) ),
	timer_( ),
	window_( window ),
	rng_( std::random_device( )( ) ),
	gif_( gfx, "../../resources/loading/", 0.03f, 0.136974603f, 0.136974603f ),
	player_( gfx, std::ifstream( "../../resources/player.png", std::ios::binary ) ),
	camera_( window.get_size( ), 0.f, 0.f, 1.f, { 1.f, 1.f, 1.f } )
{
	setup( window_, gfx );
}

void Game::setup( osharp::gui::window<codec>& window, const osharp::gui::d3d9 & gfx )
{
	player_.scale_x( 0.5f );
	player_.scale_y( 0.5f );

	window_.get_event( "moved" ) += [this, gfx, lastSize = gui::Vector2i{}]( gui::event_data<codec> &ev ) mutable
	{
		auto &data = reinterpret_cast<gui::window_moved_data<codec>&>( ev );
		if ( lastSize != data.size )
		{
			gfx.reset( data.sender.get_handle( ), false );
			lastSize = data.size;
			gif_.set_position( data.size / 2 );
		}
	
	};

}

void Game::update( const osharp::gui::d3d9 & gfx )
{
	this->dt_ = timer_.mark( );

	auto velocity = 100.f;

	gui::Vector2f move{ 0.f, 0.f };
	if ( window_.is_key_down( gui::key_w ) )
		move.y -= velocity * dt_;
	else if ( window_.is_key_down( gui::key_s ) )
		move.y += velocity * dt_;
	if ( window_.is_key_down( gui::key_d ) )
		move.x += velocity * dt_;
	else if ( window_.is_key_down( gui::key_a ) )
		move.x -= velocity * dt_;

	player_.set_position( player_.get_position( ) + move );
	gfx.draw_frame( [this]( const gui::d3d9 &d3d9 )
	{
		this->draw( d3d9, dt_ );
	}, gui::d3d9_color::from( 38, 38, 38 ) );
}

void Game::draw( const osharp::gui::d3d9 & gfx, const float &dt )
{
	camera_.follow( window_.get_size( ) / 2 );
	camera_.calculate_view_matrix( );
	camera_.set_transform( gfx );
	player_.draw( gfx );
	gif_.draw( gfx );

}

#undef far
