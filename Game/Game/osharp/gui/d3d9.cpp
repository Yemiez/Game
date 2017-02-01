#include "d3d9.h"
#include <d3d9.h>
#include <d3dx9.h>

void osharp::gui::d3d9::draw_frame( const std::function<void( const d3d9& )>& lambda, const std::uint32_t &colors ) const
{ 
	auto device = reinterpret_cast< IDirect3DDevice9* >( device_ );
	device->Clear( 0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, colors, 1.f, 0 );
	device->BeginScene( );
	lambda( *this );
	device->EndScene( );
	device->Present( nullptr, nullptr, 0, nullptr );
}

void osharp::gui::d3d9::draw_frame( const d3d9_renderable & renderable, const std::uint32_t &colors ) const
{ 
	return draw_frame( [&renderable]( const d3d9 &obj )
	{
		obj.draw( renderable );
	},
					   colors );
}

void osharp::gui::d3d9::draw( const d3d9_renderable &renderable ) const
{ 
	renderable.draw( *this );
}


bool osharp::gui::d3d9::valid( ) const
{
	return interface_ && device_;
}

void osharp::gui::d3d9::release( )
{ 
	if ( valid( ) )
	{
		reinterpret_cast< IDirect3D9Ex* >( interface_ )->Release( );
		reinterpret_cast< IDirect3DDevice9* >( device_ )->Release( );
		default_.release( );
		interface_ = nullptr;
		device_ = nullptr;
	}
}

void osharp::gui::d3d9::reset( handle & wnd, bool vsync ) const
{ 
	RECT rect;
	GetWindowRect( *reinterpret_cast< HWND* >( wnd.native_ptr( ) ), &rect );
	D3DPRESENT_PARAMETERS params
	{
		0u,
		0u,
		D3DFMT_A8R8G8B8,
		0u,
		D3DMULTISAMPLE_NONE,
		0u,
		D3DSWAPEFFECT_DISCARD,
		*reinterpret_cast<HWND*>(wnd.native_ptr( )),
		true,
		true,
		D3DFMT_D16,
		0u,
		vsync ?
		D3DPRESENT_RATE_DEFAULT :
		0u,
		vsync ?
		D3DPRESENT_INTERVAL_ONE :
		D3DPRESENT_INTERVAL_IMMEDIATE
	};
	auto res = reinterpret_cast< IDirect3DDevice9* >( device_ )->Reset( &params );
	if ( FAILED( res ) )
		throw d3d9_error( res, constexprs::str_obf( "Failed to reset d3d9 device" ).str( ) );
	if ( default_.valid( ) )
		reinterpret_cast< ID3DXFont* >( default_.get_native( ) )->OnResetDevice( );
}

void osharp::gui::d3d9::draw_string( const d3d9_font & font, 
									 const std::string & str, 
									 const std::uint32_t &color, 
									 const Vector2i &pos, 
									 const Vector2i &clip ) const
{
	auto font_ptr = reinterpret_cast< ID3DXFont* >( font.get_native( ) );
	RECT rect{
		static_cast< long >( pos.x ),
		static_cast< long >( pos.y ),
		static_cast< long >( pos.x + clip.x ),
		static_cast< long >( pos.y + clip.y )
	};
	font_ptr->DrawTextA( nullptr, str.c_str( ), -1, &rect, DT_NOCLIP, color );
}

void osharp::gui::d3d9::draw_string( const d3d9_font & font, 
									 const std::wstring & str,
									 const std::uint32_t &color,
									 const Vector2i &pos,
									 const Vector2i &clip ) const
{
	auto font_ptr = reinterpret_cast< ID3DXFont* >( font.get_native( ) );
	RECT rect{
		static_cast<long>( pos.x ),
		static_cast<long>( pos.y ),
		static_cast<long>( pos.x + clip.x ),
		static_cast<long>( pos.y + clip.y )
	};
	font_ptr->DrawTextW( nullptr, str.c_str( ), -1, &rect, DT_NOCLIP, color );
}

osharp::gui::d3d9_font & osharp::gui::d3d9::get_default_font( )
{
	return default_;
}

const osharp::gui::d3d9_font & osharp::gui::d3d9::get_default_font( ) const
{
	return default_;
}

void osharp::gui::d3d9::set_default_font( d3d9_font & font )
{ 
	if ( default_.valid( ) )
		default_.release( );
	default_ = font;
}

void * osharp::gui::d3d9::get_interface( ) const
{
	return interface_;
}

void * osharp::gui::d3d9::get_device( ) const
{
	return device_;
}

void * osharp::gui::d3d9::get_line_buffer( ) const
{
	return line_;
}

void * osharp::gui::d3d9::get_sprite( ) const
{
	return sprite_;
}

void osharp::gui::d3d9::create( handle wnd, bool vsync )
{ 
	if ( !wnd )
		throw d3d9_error( constexprs::str_obf( "Failed to create DirectX9 object (HWND passed was null)" ).str( ) );
	HRESULT code = 0;
	if ( FAILED( code = Direct3DCreate9Ex( D3D_SDK_VERSION, reinterpret_cast<IDirect3D9Ex**>( &interface_ ) ) ) )
		throw d3d9_error( code, constexprs::str_obf( "Failed to create DirectX9 Interface" ).str( ) );
	RECT rect;
	GetWindowRect( *reinterpret_cast< HWND* >( wnd.native_ptr( ) ), &rect );
	D3DPRESENT_PARAMETERS params
	{
		0u,
        0u,
		D3DFMT_A8R8G8B8,
		0u,
		D3DMULTISAMPLE_NONE,
		0u,
		D3DSWAPEFFECT_DISCARD,
		*reinterpret_cast<HWND*>( wnd.native_ptr( ) ),
		true,
		true,
		D3DFMT_D16,
		0u,
	    vsync ? 
		D3DPRESENT_RATE_DEFAULT :
		0u,
		vsync ? 
		D3DPRESENT_INTERVAL_ONE :
		D3DPRESENT_INTERVAL_IMMEDIATE
	};
	if ( FAILED( code = reinterpret_cast< IDirect3D9Ex* >( interface_ )->CreateDevice( D3DADAPTER_DEFAULT,
																					   D3DDEVTYPE_HAL,
																					   *reinterpret_cast< HWND* >( wnd.native_ptr( ) ),
																					   D3DCREATE_HARDWARE_VERTEXPROCESSING,
																					   &params,
																					   reinterpret_cast< IDirect3DDevice9** >( &device_ ) ) ) )
		throw d3d9_error( code, constexprs::str_obf( "Failed to create DirectX9 Device" ).str( ) );
	default_ = d3d9_font( *this, constexprs::str_obf( "Verdana" ).str( ) );
	code = D3DXCreateLine( reinterpret_cast< IDirect3DDevice9* >( device_ ), reinterpret_cast< ID3DXLine** >( &line_ ) );
	if ( FAILED( code ) )
		throw d3d9_error( code, constexprs::str_obf( "D3D9 Error occurred when creating line buffer" ).str( ) );

	code = D3DXCreateSprite( reinterpret_cast< IDirect3DDevice9* >( device_ ), reinterpret_cast<ID3DXSprite**>( &sprite_ ) );
	if ( FAILED( code ) )
		throw d3d9_error( code, constexprs::str_obf( "D3D9 Error occurred when creating sprite" ).str( ) );
}

osharp::gui::Vector2i osharp::gui::d3d9_font::get_size( const std::string & str, const CalcFlags &flags ) const
{
	if ( flags == CalcFlags::WithoutTrailingSpaces )
	{
		auto font = reinterpret_cast< ID3DXFont* >( this->font_ );
		if ( !font )
			return{ };
		RECT result;
		font->DrawTextA( nullptr, str.c_str( ), -1, &result, DT_CALCRECT, 0 );
		return{ result.right - result.left,
				result.bottom - result.top };
	}
	else if ( flags == CalcFlags::OnlyTrailingSpaces )
	{
		static constexpr auto A = constexprs::to_str( "W" );
		static constexpr auto space = constexprs::to_str( "W W" );
		
		auto dot_size = get_size( A.c_str( ) );
		auto manip_size = get_size( space.c_str( ) );
		Vector2i vspace{ manip_size.x - ( dot_size.x * 2 ), manip_size.y };
		if ( str.back( ) == ' ' )
		{
			auto count = std::count_if( std::rbegin( str ), std::rend( str ), [non_sp = true]( const auto &elem )mutable
			{
				if ( !non_sp )
					return false;
				else if ( elem == ' ' )
					non_sp = false;
				return non_sp;
			} );
			return { count * vspace.x, vspace.y };
		}
		return { 0, 0 };
	}
	else
	{
		return get_size( str, CalcFlags::WithoutTrailingSpaces ) + get_size( str, CalcFlags::OnlyTrailingSpaces );
	}
}

osharp::gui::Vector2i osharp::gui::d3d9_font::get_size( const std::wstring & str, const CalcFlags &flags ) const
{
	if ( flags == CalcFlags::WithoutTrailingSpaces )
	{
		auto font = reinterpret_cast< ID3DXFont* >( this->font_ );
		if ( !font )
			return{};
		RECT result;
		font->DrawTextW( nullptr, str.c_str( ), -1, &result, DT_CALCRECT, 0 );
		return{ result.right - result.left,
			result.bottom - result.top };
	}
	else if ( flags == CalcFlags::OnlyTrailingSpaces )
	{
		static constexpr auto A = constexprs::to_str( "W" );
		static constexpr auto space = constexprs::to_str( "W W" );

		auto dot_size = get_size( A.c_str( ) );
		auto manip_size = get_size( space.c_str( ) );
		Vector2i vspace{ manip_size.x - ( dot_size.x * 2 ), manip_size.y };
		if ( str.back( ) == ' ' )
		{
			auto count = std::count_if( std::rbegin( str ), std::rend( str ), [non_sp = true]( const auto &elem )mutable
			{
				if ( !non_sp )
					return false;
				else if ( elem == ' ' )
					non_sp = false;
				return non_sp;
			} );
			return { count * vspace.x, vspace.y };
		}
		return { 0, 0 };
	}
	else
	{
		return get_size( str, CalcFlags::WithoutTrailingSpaces ) + get_size( str, CalcFlags::OnlyTrailingSpaces );
	}
}

void * osharp::gui::d3d9_font::get_native( ) const
{
	return font_;
}

osharp::gui::d3d9_font_struct osharp::gui::d3d9_font::get_parameters( ) const
{
	return params_;
}

void osharp::gui::d3d9_font::release( )
{ 
	if ( valid( ) )
	{
		reinterpret_cast< ID3DXFont* >( font_ )->Release( );
		font_ = nullptr;
	}
}

void osharp::gui::d3d9_font::create( const d3d9 & renderer, d3d9_font & font, std::string name, const d3d9_font_struct &params )
{
	auto device = reinterpret_cast< IDirect3DDevice9* >( renderer.get_device( ) );
	auto res = D3DXCreateFontA( device, params.Height, params.Width, params.Weight, params.MipLevels, params.Italic,
								params.CharSet, params.OutputPrecision, params.Quality, params.PitchAndFamily, name.c_str( ), 
								reinterpret_cast< ID3DXFont** >( &font.font_ ) );
	if ( FAILED( res ) )
		throw d3d9_error( res, constexprs::str_obf( "Failed to create Font object" ).str( ) );
}

osharp::gui::d3d9_font_struct& osharp::gui::d3d9_font_struct::enable_antialias( )
{
	Quality = ANTIALIASED_QUALITY;
	return *this;
}

osharp::gui::d3d9_font_struct& osharp::gui::d3d9_font_struct::disable_antialias( )
{ 
	Quality = DEFAULT_QUALITY;
	return *this;
}

osharp::gui::d3d9_line::d3d9_line( )
	: start( ),
	end( ),
	width( ),
	color( )
{ }

osharp::gui::d3d9_line::d3d9_line( Vector2i start, Vector2i end, float width, std::uint32_t color )
	: start( start ),
	end( end ),
	width( width ),
	color( color )
{ }

void osharp::gui::d3d9_line::draw( const d3d9 & renderer ) const
{ 
	auto line = reinterpret_cast< ID3DXLine* >( renderer.get_line_buffer( ) );
	line->SetWidth( width );
	D3DXVECTOR2 vertices[2]
	{
		{ static_cast<float>( start.x ), static_cast<float>( start.y ) },
		{ static_cast<float>( end.x ), static_cast<float>( end.y ) }
	};
	line->Draw( vertices, 2, color );
}

osharp::gui::d3d9_image_view::d3d9_image_view( const d3d9 & gfx, std::istream & stream, Vector2f pos )
	: texture_( nullptr ), pos_( pos )
{
	load( gfx, stream );
}

osharp::gui::d3d9_image_view::d3d9_image_view( )
	: texture_( nullptr ), pos_( 0, 0 )
{}

void osharp::gui::d3d9_image_view::load( const d3d9 & gfx, std::istream & stream )
{
	auto device = reinterpret_cast<IDirect3DDevice9*>( gfx.get_device( ) );
	std::string data{ std::istreambuf_iterator<char>( stream ),
					  std::istreambuf_iterator<char>( ) };
	auto hr = D3DXCreateTextureFromFileInMemory( device, data.c_str( ), data.size( ), reinterpret_cast<IDirect3DTexture9**>( &texture_ ) );

	if ( FAILED( hr ) )
		throw d3d9_error( hr, constexprs::str_obf( "D3D9 Error occurred when creating texture from std::istream" ).str( ) );
	D3DSURFACE_DESC s;
	reinterpret_cast<IDirect3DTexture9*>( texture_ )->GetLevelDesc( 0, &s );
	dim_.x = s.Width;
	dim_.y = s.Height;
}

void osharp::gui::d3d9_image_view::draw( const d3d9 & gfx ) const
{
	if ( texture_ )
	{
		auto sprite = reinterpret_cast<ID3DXSprite*>( gfx.get_sprite( ) );
		sprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE );

		if ( x_ != 0.f || y_ != 0.f )
		{
			D3DXMATRIX old;
			
			// Save current
			sprite->GetTransform( &old );

			D3DXMATRIX scaled;
			D3DXVECTOR2 scaling{ x_, y_ };

			D3DXMatrixTransformation2D( &scaled, nullptr, 0.f, &scaling, nullptr, 0.f, nullptr );

			// Scale
			sprite->SetTransform( &scaled );

			// Calculate new position based on the scaling applied
			D3DXVECTOR3 pos{ pos_.x * ( 1.f / x_ ), pos_.y * ( 1.f / y_ ), 0.f };
			sprite->Draw( reinterpret_cast<IDirect3DTexture9*>( texture_ ), nullptr, nullptr, &pos, 0xFFFFFFFF );

			// Reset back
			sprite->SetTransform( &old );
		}
		else
		{
			D3DXVECTOR3 pos{ static_cast<float>( pos_.x ), static_cast<float>( pos_.y ), 0.f };
			sprite->Draw( reinterpret_cast<IDirect3DTexture9*>( texture_ ), nullptr, nullptr, &pos, 0xFFFFFFFF );
		}

		sprite->End( );
	}
}
