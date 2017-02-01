#pragma once
#include <vector>
#include <random>
#include "2d/2dcamera.h"
#include "Teapot.h"
#include "image_animator.h"

class Game
{
public:
	using codec = osharp::cvt::codec_cvt<>;
	using d3d9_string = osharp::gui::d3d9_string<codec>;

public:
	Game( osharp::gui::window<codec> &window, const osharp::gui::d3d9 &gfx );

	void setup( osharp::gui::window<codec> &window, const osharp::gui::d3d9 &gfx );

	void update( const osharp::gui::d3d9 &gfx );

	void draw( const osharp::gui::d3d9 &gfx, const float &dt );

private:
	osharp::gui::d3d9_font font_;
	osharp::concurrency::frame_timer<> timer_;
	float dt_;
	osharp::gui::window<codec> &window_;
	bool paused_ = true;
	std::mt19937 rng_;
	std::normal_distribution<float> num_{ 0.01f, 1.0f };
	image_animator gif_;
	osharp::gui::d3d9_image_view player_;
	engine::Camera2d camera_;
};