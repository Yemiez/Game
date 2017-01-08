#include "Teapot.h"

Teapot::Teapot( ID3DXMesh *mesh, const D3DCOLORVALUE & color, D3DXVECTOR3 position )
	: mesh_( mesh ),
	mat_( ),
	position_( position )
{
	mat_.Ambient.r = mat_.Diffuse.r = color.r;
	mat_.Ambient.g = mat_.Diffuse.g = color.g;
	mat_.Ambient.b = mat_.Diffuse.b = color.b;
}

Teapot::~Teapot( )
{}

void Teapot::draw( const osharp::gui::d3d9 & gfx ) const
{
	auto device = reinterpret_cast<IDirect3DDevice9Ex*>( gfx.get_device( ) );
	D3DXMATRIX trans_mtx, rot_mtx_pitch, rot_mtx_yaw, rot_mtx_roll;
	D3DXMatrixIdentity( &rot_mtx_pitch );
	D3DXMatrixIdentity( &rot_mtx_yaw );
	D3DXMatrixIdentity( &rot_mtx_roll );
	D3DXMatrixTranslation( &trans_mtx, position_.x, position_.y, position_.z );

	D3DXMatrixRotationX( &rot_mtx_pitch, pitch_ );
	D3DXMatrixRotationY( &rot_mtx_yaw, yaw_ );
	D3DXMatrixRotationZ( &rot_mtx_roll, roll_ );


	auto res_mtx = rot_mtx_pitch * rot_mtx_yaw * rot_mtx_roll * trans_mtx;

	device->SetTransform( D3DTS_WORLD, &res_mtx );
	device->SetMaterial( &mat_ );
	mesh_->DrawSubset( 0 );
}

void Teapot::set_color( const D3DCOLORVALUE & color )
{
	mat_.Ambient.r = mat_.Diffuse.r = color.r;
	mat_.Ambient.g = mat_.Diffuse.g = color.g;
	mat_.Ambient.b = mat_.Diffuse.b = color.b;
}

void Teapot::set_position( const D3DXVECTOR3 & position )
{
	position_ = position;
}

void Teapot::rotate_yaw( float amount )
{
	yaw_ = amount;
}

void Teapot::rotate_pitch( float amount )
{
	pitch_ = amount;
}

void Teapot::rotate_roll( float amount )
{
	roll_ = amount;
}
