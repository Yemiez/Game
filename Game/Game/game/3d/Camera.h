#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <osharp.h>

#ifndef FAGGOT_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#define NOMINMAX
#define FAGGOT_WINDOWS
#endif

namespace engine
{

	// z = ROLL
	// y = YAW
	// x = PITCH

	class Camera
	{
	public:
		Camera( D3DXVECTOR3 position,
			    float fov,
				osharp::gui::Vector2i windowSize, 
				float nearPlane, 
				float farPlane,
				const osharp::gui::d3d9 &device );
		~Camera( );

		void CalculateViewMatrix( const osharp::gui::d3d9 &device );
		
		void RotatePitchBy( float amount );

		void RotateYawBy( float amount );

		void RotateRollBy( float amount );

		void MoveForwardBy( float amount );

		void MoveLeftBy( float amount );

		void MoveRightBy( float amount );

		void MoveBackBy( float amount );

		void MoveUpBy( float amount );

		void MoveDownBy( float amount );

		const auto &get_view( ) const
		{
			return view_;
		}

		const auto &get_projection( ) const
		{
			return projection_;
		}





	private:

		float RestrictAngle( float angle );

		void ApplyPitch( );

		void ApplyYaw( );

		void ApplyRoll( );

		void FillViewMatrix( const osharp::gui::d3d9 &device );

	private:
		D3DXVECTOR3 up_, 
			look_, 
			right_,   // AKA eye_
			position_;
		float pitch_, // x
			yaw_,     // y
			roll_;    // z
		D3DXMATRIX view_,
			world_,
			projection_;
		bool changed_ = false;
	};


}