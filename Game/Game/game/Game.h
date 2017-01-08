#pragma once
#include <vector>
#include <random>
#include "3d/Camera.h"
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

	D3DXVECTOR3 RaycastScreenPosition( const D3DXVECTOR2 &screen );

private:
	osharp::gui::d3d9_font font_;
	osharp::concurrency::frame_timer<> timer_;
	float dt_;

	// Camera etc
	osharp::gui::window<codec> &window_;
	engine::Camera camera_;
	float vsens_ = 5.f,
		vmove_ = 40.f,
		rot_ = 0.0f,
		rot_o_ = 0.0f,
		time_o_ = 0.0f,
		img_scale_ = 1.f;
	osharp::gui::Vector2i mouse_{ 0, 0 };
	bool paused_ = true;
	int current_ = 1;
	osharp::gui::Vector2i middle_{ 0, 0 };
	const int monw_, monh_;
	std::mt19937 rng_;
	std::normal_distribution<float> num_{ 0.01f, 1.0f };
	std::vector<Teapot> teapots_;
	image_animator image_;
};