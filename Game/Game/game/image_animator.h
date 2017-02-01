#pragma once
#include <osharp.h>

class image_animator
{
public:
	image_animator( const osharp::gui::d3d9 &gfx,
					const std::string folder,
					const float frame_time,
					float scale_x = 1.f,
					float scale_y = 1.f );

	void draw( const osharp::gui::d3d9 &gfx );

	void scale_x( float amount );

	void scale_y( float amount );

	void shrink_x( float amount );

	void shrink_y( float amount );
	
	void set_position( osharp::gui::Vector2f pos );

private:
	struct Image
	{
		size_t num = 0u;
		osharp::gui::d3d9_image_view image{};
	};

	std::vector<Image> images_;
	osharp::concurrency::frame_timer<> timer_;
	float frame_time_ = 0.f,
		passed_dt_ = 0.f;
	Image *current_ = nullptr;
};