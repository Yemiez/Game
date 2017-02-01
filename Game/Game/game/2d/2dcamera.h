#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <osharp.h>

#ifndef WINDOWS_NO_POLLUTION
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
#undef near
#undef far
#undef min
#undef max
#define WINDOWS_NO_POLLUTION
#endif

#include <DirectXMath.h>

namespace engine
{

	class Camera2d
	{
	public:
		Camera2d( osharp::gui::Vector2i size, float angle, float near, float far, DirectX::XMFLOAT3 scale );
		Camera2d( ) = delete;

		template<typename T>
		void follow( const osharp::gui::Vector2<T> &what )
		{
			pos_ = { static_cast<float>( what.x ), static_cast<float>( what.y ) };
		}

		void calculate_view_matrix( );

		void set_transform( const osharp::gui::d3d9 &gfx );

	private:
		osharp::gui::Vector2i size_;
		osharp::gui::Vector2f pos_;
		float angle_, far_, near_;
		D3DXMATRIX identity_,
			projection_,
			view_;
		DirectX::XMFLOAT3 scale_;
	};


}