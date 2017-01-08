#pragma once
#include <osharp.h>
#include <d3d9.h>
#include <d3dx9.h>

class Teapot
{
public:
	Teapot( ID3DXMesh *mesh,
			const D3DCOLORVALUE &color,
			D3DXVECTOR3 position );
	~Teapot( );

	ID3DXMesh *get_mesh( )
	{
		return mesh_;
	}

	const D3DMATERIAL9 &get_material( ) const
	{
		return mat_;
	}

	const D3DXVECTOR3 &get_position( ) const
	{
		return position_;
	}

	void draw( const osharp::gui::d3d9 &gfx ) const;

	void set_color( const D3DCOLORVALUE &color );

	void set_position( const D3DXVECTOR3 &position );

	void rotate_yaw( float amount );

	void rotate_pitch( float amount );
	
	void rotate_roll( float amount );

private:
	ID3DXMesh *mesh_;
	D3DMATERIAL9 mat_;
	D3DXVECTOR3 position_;
	float yaw_ = 0.f,
		pitch_ = 0.f,
		roll_ = 0.f;
};