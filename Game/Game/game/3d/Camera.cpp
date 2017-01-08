#include "Camera.h"


engine::Camera::Camera( D3DXVECTOR3 pos, float fov, osharp::gui::Vector2i windowSize, float nearPlane, float farPlane, const osharp::gui::d3d9 &device )
{
	position_ = pos;
	float aspect = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
	D3DXMatrixPerspectiveFovLH( &projection_, fov, aspect, nearPlane, farPlane );
	reinterpret_cast<IDirect3DDevice9*>(device.get_device( ))->SetTransform( D3DTS_PROJECTION, &projection_ );
}

engine::Camera::~Camera( )
{}

void engine::Camera::CalculateViewMatrix( const osharp::gui::d3d9 &device )
{
	changed_ = false;
	up_ = { 0.f, 1.f, 0.f };
	look_ = { 0.f, 0.f, 1.f };
	right_ = { 1.f, 0.f, 0.f };
	ApplyYaw( );
	ApplyPitch( );
	ApplyRoll( );
	FillViewMatrix( device );
}

void engine::Camera::RotatePitchBy( float amount )
{
	pitch_ += amount;
	pitch_ = RestrictAngle( pitch_ );
}

void engine::Camera::RotateYawBy( float amount )
{
	yaw_ += amount;
	yaw_ = RestrictAngle( yaw_ );
}

void engine::Camera::RotateRollBy( float amount )
{
	roll_ += amount;
	roll_ = RestrictAngle( roll_ );
}

void engine::Camera::MoveForwardBy( float amount )
{
	position_ += look_ * (-amount);
}

void engine::Camera::MoveLeftBy( float amount )
{
	position_ += right_ * amount;
}

void engine::Camera::MoveRightBy( float amount )
{
	position_ += right_ * (-amount);
}

void engine::Camera::MoveBackBy( float amount )
{
	position_ += look_ * amount;
}

void engine::Camera::MoveUpBy( float amount )
{
	position_ += up_ * (-amount);
}

void engine::Camera::MoveDownBy( float amount )
{
	position_ += up_ * amount;
}

float engine::Camera::RestrictAngle( float angle )
{
	while ( angle>2 * D3DX_PI )
		angle -= 2 * D3DX_PI;
	while ( angle<0 )
		angle += 2 * D3DX_PI;
	return angle;
}

void engine::Camera::ApplyPitch( )
{
	D3DXMATRIX mtx;
	D3DXMatrixRotationAxis( &mtx, &right_, pitch_ );
	D3DXVec3TransformCoord( &look_, &look_, &mtx );
	D3DXVec3TransformCoord( &up_, &up_, &mtx );
}

void engine::Camera::ApplyYaw( )
{
	D3DXMATRIX mtx;
	D3DXMatrixRotationAxis( &mtx, &up_, yaw_ );
	D3DXVec3TransformCoord( &look_, &look_, &mtx );
	D3DXVec3TransformCoord( &right_, &right_, &mtx );
}

void engine::Camera::ApplyRoll( )
{
	D3DXMATRIX mtx;
	D3DXMatrixRotationAxis( &mtx, &look_, roll_ );
	D3DXVec3TransformCoord( &right_, &right_, &mtx );
	D3DXVec3TransformCoord( &up_, &up_, &mtx );
}

void engine::Camera::FillViewMatrix( const osharp::gui::d3d9 & device )
{
	D3DXMatrixIdentity( &view_ );
	
	view_._11 = right_.x;
	view_._21 = right_.y;
	view_._31 = right_.z;
	view_._12 = up_.x;
	view_._22 = up_.y;
	view_._32 = up_.z;
	view_._13 = look_.x;
	view_._23 = look_.y;
	view_._33 = look_.z;

	view_._41 = D3DXVec3Dot( &position_, &right_ );
	view_._42 = D3DXVec3Dot( &position_, &up_ );
	view_._43 = D3DXVec3Dot( &position_, &look_ );

	reinterpret_cast<IDirect3DDevice9*>(device.get_device( ))->SetTransform( D3DTS_VIEW, &view_ );
}


