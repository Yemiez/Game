#include "2dcamera.h"

engine::Camera2d::Camera2d( osharp::gui::Vector2i size, float angle, float near, float far, DirectX::XMFLOAT3 scale )
	: size_( std::move( size ) ),
	pos_( { static_cast<float>( size_.x / 2 ), static_cast<float>( size_.y / 2 ) } ),
	angle_( angle ),
	far_( far ),
	near_( near ),
	scale_( scale )
{
	D3DXMatrixOrthoLH( &projection_, size.x, -size.y, near, far );
	D3DXMatrixIdentity( &identity_ );
}


void engine::Camera2d::calculate_view_matrix( )
{
	view_ = D3DXMATRIX( scale_.x * std::cos( angle_ ), scale_.x * std::sin( angle_ ), 0, 0,
						-scale_.y * std::sin( angle_ ), scale_.y * std::cos( angle_ ), 0, 0,
						0, 0, scale_.z, 0,
						-pos_.x * scale_.x * std::cos( angle_ ) + pos_.y * scale_.y * std::sin( angle_ ), 
						-pos_.x * scale_.y * std::sin( angle_ ) - pos_.y * scale_.y * std::cos( angle_ ), 0.f, 1.f );
}

void engine::Camera2d::set_transform( const osharp::gui::d3d9 & gfx )
{
	auto device = reinterpret_cast<IDirect3DDevice9*>( gfx.get_device( ) );
	device->SetTransform( D3DTS_PROJECTION, &projection_ );
	device->SetTransform( D3DTS_WORLD, &identity_ );
	device->SetTransform( D3DTS_VIEW, &view_ );
}
