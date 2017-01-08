#include "Game.h"
#include <sstream>

namespace cvt = osharp::cvt;
namespace fmt = osharp::formats;
namespace gui = osharp::gui;
namespace cnc = osharp::concurrency;
namespace spg = osharp::spelling;
namespace tok = osharp::tokenizing;

Game::Game( osharp::gui::window<codec>& window, const osharp::gui::d3d9 & gfx )
	: font_( gfx, "Verdana", gui::d3d9_font_struct{}.enable_antialias( ) ),
	timer_( ),
	window_( window ),
	camera_( { 0.f, 0.f, 12.f }, 1.5708f, window_.get_size( ), 1.0f, 1000.f, gfx ),
	monw_( gui::MonitorWidth( ) ),
	monh_( gui::MonitorHeight( ) ),
	rng_( std::random_device( )( ) ),
	image_( gfx, "../../resources/loading/", 0.03f, 1.f, 1.f )
{
	setup( window_, gfx );

	window_.get_event( "moved" ) += [gfx, this]( gui::event_data<codec> &ev )mutable
	{
		auto &data = reinterpret_cast<gui::window_moved_data<codec>&>( ev );
		gfx.reset( data.sender.get_handle( ) );
		POINT p{ gui::MonitorWidth( ) / 2, gui::MonitorHeight( ) / 2 };
		ScreenToClient( *reinterpret_cast<HWND*>( window_.get_handle( ).native_ptr( ) ),
						&p );
		middle_ = { p.x, p.y };
	};

	window_.get_event( "key" ) += [gfx, this]( gui::event_data<codec> &ev )mutable
	{
		auto &data = reinterpret_cast<gui::window_key_data<codec>&>( ev );
		if ( data.isRelease )
		{
			switch( data.code )
			{
				case gui::key_escape:
				this->paused_ = !this->paused_;
				break;
				case gui::key_left:
				--current_;
				if ( current_ == -1 )
					current_ = 2;
				break;
				case gui::key_right:
				++current_;
				if ( current_ == 3 )
					current_ = 0;
				break;

			}
		}

	};

	window_.get_event( "mouse" ) += [this]( gui::event_data<codec> &ev )
	{
		auto &data = reinterpret_cast<gui::window_mouse_data<codec>&>( ev );
		if ( data.isRelease && data.code == 1 )
			this->RaycastScreenPosition( { static_cast<float>( data.position.x ),
										 static_cast<float>( data.position.y ) } );
	};

	POINT p{ gui::MonitorWidth( ) / 2, gui::MonitorHeight( ) / 2 };
	ScreenToClient( *reinterpret_cast<HWND*>( window_.get_handle( ).native_ptr( ) ),
					&p );
	middle_ = { p.x, p.y };
}

void Game::setup( osharp::gui::window<codec>& window, const osharp::gui::d3d9 & gfx )
{
	auto device = reinterpret_cast<IDirect3DDevice9Ex*>( gfx.get_device( ) );

	// Create teapot
	ID3DXMesh *mesh = nullptr;
	auto hr = D3DXCreateTeapot( device, &mesh, nullptr );
	if ( FAILED( hr ) )
		throw;

	teapots_.emplace_back( mesh, 
						   D3DCOLORVALUE{ num_( rng_ ), num_( rng_ ), num_( rng_ ) }, 
						   D3DXVECTOR3{ 0.f, 0.f, 0.f } );

	// ===========
	//	Lighting
	// ===========

	// The teapot has position and normal per vertex so we will need to turn on the lights
	device->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB( 20, 20, 20 ) );
	device->SetRenderState( D3DRS_LIGHTING, 1 );

	// Fill in our light struct
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof( light ) );
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;
	light.Range = 1000.0f;

	// Create a direction (Must be normalized)
	D3DXVECTOR3 tmpDir{ 0.0f, -0.3f, 0.7f };
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &tmpDir );

	// Set it
	device->SetLight( 0, &light );
	device->LightEnable( 0, true );

	// ============
	//	Materials
	// ============


}

void Game::update( const osharp::gui::d3d9 & gfx )
{
	this->dt_ = timer_.mark( );

	if ( !paused_ )
	{
		if ( window_.is_key_down( gui::key_w ) )
			camera_.MoveForwardBy( vmove_ * dt_ );

		if ( window_.is_key_down( gui::key_s ) )
			camera_.MoveBackBy( vmove_ * dt_ );

		if ( window_.is_key_down( gui::key_a ) )
			camera_.MoveLeftBy( vmove_ * dt_ );

		if ( window_.is_key_down( gui::key_d ) )
			camera_.MoveRightBy( vmove_ * dt_ );

		if ( window_.is_key_down( gui::key_space ) )
			camera_.MoveUpBy( vmove_ * dt_ );

		if ( window_.is_key_down( gui::key_control ) )
			camera_.MoveDownBy( vmove_ * dt_ );

		if ( window_.is_key_down( gui::key_up ) )
		{
			if ( current_ == 0 )
				image_.scale_x( img_scale_ * dt_ );
			else if ( current_ == 1 )
				image_.scale_y( img_scale_ * dt_ );
			else
			{
				image_.scale_x( img_scale_ * dt_ );
				image_.scale_y( img_scale_ * dt_ );
			}
		}

		if ( window_.is_key_down( gui::key_down ) )
		{
			if ( current_ == 0 )
				image_.shrink_x( img_scale_ * dt_ );
			else if ( current_ == 1 )
				image_.shrink_y( img_scale_ * dt_ );
			else
			{
				image_.shrink_x( img_scale_ * dt_ );
				image_.shrink_y( img_scale_ * dt_ );
			}
		}

		// Rotation Speed
		if ( window_.is_key_down( gui::key_shift ) &&
			 window_.is_key_down( gui::key_add ) )
			vsens_ += 0.1f * dt_;
		else if ( window_.is_key_down( gui::key_add ) )
			vmove_ += 1.0f * dt_;

		if ( window_.is_key_down( gui::key_shift ) &&
			 window_.is_key_down( gui::key_sub ) )
			vsens_ -= 0.1f * dt_;
		else if ( window_.is_key_down( gui::key_sub ) )
			vmove_ -= 1.0f * dt_;

		auto mouse = window_.get_mouse( );

		auto distance = mouse - middle_;

		if ( distance.x != 0 )
			camera_.RotateYawBy( distance.x * vsens_ * dt_ );

		if ( distance.y != 0 )
			camera_.RotatePitchBy( distance.y * vsens_ * dt_ );

		SetCursorPos( monw_ / 2, monh_ / 2 );
	}
	else
		mouse_ = window_.get_size( ) / 2;

	gfx.draw_frame( [this]( const gui::d3d9 &d3d9 )
	{
		this->draw( d3d9, dt_ );
	}, gui::d3d9_color::from( 38, 38, 38 ) );
}

void Game::draw( const osharp::gui::d3d9 & gfx, const float &dt )
{
	rot_ += dt * 50.f;
	rot_o_ += dt * 10.f;
	time_o_ += dt;
	camera_.CalculateViewMatrix( gfx );
	auto device = reinterpret_cast<IDirect3DDevice9Ex*>( gfx.get_device( ) );

	auto count = 0u;

	// X = right / left
	// Y = Up / Down
	// Z = forward / back

	// implement: http://www.unknowncheats.me/forum/d3d-tutorials-and-source/102186-my-d3d9-mesh-directx-x-format-loading-class.html


	for ( auto &x : teapots_ )
	{
		x.rotate_yaw( rot_o_ );
		x.draw( gfx );
	}


	// Draw Text
	std::stringstream ss;
	ss <<  "Mouse sensitivity: " << vsens_ << "\n";
	ss << "Velocity: " << vmove_ << "\n";
	ss << "Paused: " << std::boolalpha << paused_ << "\n";
	ss << "Frametime: " << dt << "\n";
	ss << "Current scale modifying: " << ( current_ == 0 ? "X" : current_ == 1 ? "Y" : "Both" ) << "\n";
	gui::Vector2i position{ 15, 15 };
	gui::d3d9_string<codec> string{ ss.str( ) , font_, position, gui::d3d9_color::CadetBlue };
	string.draw( gfx );

	image_.set_position( string.get_position( ) + gui::Vector2i{ 0, string.get_size( ).y + 15 } );
	image_.draw( gfx );

}

#undef far

D3DXVECTOR3 Game::RaycastScreenPosition( const D3DXVECTOR2 &screen )
{
	auto view = camera_.get_view( );
	auto proj = camera_.get_projection( );

	auto origin = D3DXVECTOR3{ screen.x, screen.y, 0.f };
	auto far = D3DXVECTOR3{ screen.x, screen.y, 1.f };

	auto matrix = view * proj;
	D3DXMATRIX inverse;
	D3DXMatrixInverse( &inverse, nullptr, &matrix );

	D3DXVECTOR3 ray_origin, ray_far;
	D3DXVec3TransformCoord( &ray_origin, &origin, &inverse );
	D3DXVec3TransformCoord( &ray_far, &far, &inverse );

	D3DXVECTOR3 ray_result;
	D3DXVec3Normalize( &ray_result, &( ray_far - ray_origin ) );

	std::cout << "Ray result: { " << ray_result.x << ", " << ray_result.y << ", " << ray_result.z << " }" << std::endl;
	return ray_result;
}
