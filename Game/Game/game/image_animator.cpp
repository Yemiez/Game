#include "image_animator.h"
#include <filesystem>

image_animator::image_animator( const osharp::gui::d3d9 &gfx, 
								const std::string folder, 
								const float frame_time,
								float scale_x,
								float scale_y )
	: images_( ), timer_( ), frame_time_( frame_time )
{
	namespace fs = std::experimental::filesystem;
	fs::directory_iterator it{ folder };
	for ( fs::directory_iterator it{ folder };
		  it != fs::directory_iterator{};
		  ++it )
	{
		auto path = it->path( );
		if ( path.extension( ) == ".png" )
		{
			auto str_path = path.string( );

			auto bracket_begin = str_path.find( '[' ) + 1;
			auto bracket_end = str_path.find( ']', bracket_begin );

			if ( bracket_begin == std::string::npos || bracket_end == std::string::npos )
				return;

			Image image;
			image.num = std::stoul( str_path.substr( bracket_begin, bracket_end - bracket_begin ) );
			image.image.load( gfx, std::ifstream( str_path, std::ios::binary ) );
			images_.emplace_back( std::move( image ) );
		}
	}

	std::sort( images_.begin( ), images_.end( ), []( Image &b1, Image &b2 )
	{
		return b1.num < b2.num;
	} );
	this->scale_x( scale_x );
	this->scale_y( scale_y );
	current_ = &images_.front( );
}

void image_animator::draw( const osharp::gui::d3d9 & gfx )
{
	auto dt = timer_.mark( );
	passed_dt_ += dt;

	if ( passed_dt_ >= frame_time_ )
	{
		current_ = current_ ? ( current_->num + 1 < images_.size( ) ? &images_.at( current_->num + 1 ) : &images_.front( ) ) : &images_.front( );
		passed_dt_ = 0.f;
	}

	current_->image.draw( gfx );
}

void image_animator::scale_x( float amount )
{
	for ( auto &x : images_ )
		x.image.scale_x( amount );
}

void image_animator::scale_y( float amount )
{
	for ( auto &x : images_ )
		x.image.scale_y( amount );
}

void image_animator::shrink_x( float amount )
{
	for ( auto &x : images_ )
		x.image.shrink_x( amount );
}

void image_animator::shrink_y( float amount )
{
	for ( auto &x : images_ )
		x.image.shrink_y( amount );
}

void image_animator::set_position( osharp::gui::Vector2i pos )
{
	for ( auto &x : images_ )
		x.image.set_position( pos );
}
