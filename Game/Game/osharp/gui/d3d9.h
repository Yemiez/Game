#pragma once
#include "../formatting/utf.h"
#include "../constexpr/string.h"
#include "handle.h"
#include "dimensions.h"
#include <vector>

namespace osharp { namespace gui {

	class d3d9_error
		: public std::exception
	{
	public:
		template<typename T>
		d3d9_error( const std::basic_string<T> &str )
			: std::exception( cvt::codec_cvt<formats::utf_8>::cvt( str ).c_str( ) )
		{ }
		template<typename T>
		d3d9_error( long res, const std::basic_string<T> &str )
			: d3d9_error( str )
		{ 
			res_ = res;
		}
		template<typename T>
		d3d9_error( long res, const T *str )
			: std::exception( str ),
			res_( res )
		{ }
		template<typename T>
		d3d9_error( const T *str )
			: std::exception( str ),
			res_( 0 )
		{ }
		d3d9_error( )
		{ }

		long get_errc( ) const
		{
			return res_;
		}

	private:
		long res_;
	};

	class d3d9_color
	{
	public:
		typedef const std::uint32_t type;
		static type AliciaBlue = 0xFFF0F8FFu;
		static type AntiqueWhite = 0xFFFAEBD7u;
		static type Aqua = 0xFF00FFFFu;
		static type Aquamarine = 0xFF7FFFD4u;
		static type Azure = 0xFFF0FFFFu;
		static type Beige = 0xFFF5F5DCu;
		static type Bisque = 0xFFFFE4C4u;
		static type Black = 0xFF000000u;
		static type BlanchedAlmond = 0xFFFFEBCDu;
		static type Blue = 0xFF0000FFu;
		static type BlueViolet = 0xFF8A2BE2u;
		static type Brown = 0xFFA52A2Au;
		static type BurlyWodd = 0xFFDEB887u;
		static type CadetBlue = 0xFF5F9EA0u;
		static type Chartreuse = 0xFF7FFF00u;
		static type Chocolate = 0xFFD2691Eu;
		static type Coral = 0xFFFF7F50u;
		static type CornFlowerBlue = 0xFF6495EDu;
		static type Cornsilk = 0xFFFFF8DCu;
		static type Crimson = 0xFFDC143Cu;
		static type Cyan = 0xFF00FFFFu;
		static type DarkBlue = 0xFF00008Bu;
		static type DarkCyan = 0xFF008B8Bu;
		static type DarkGoldenRod = 0xFFB8860Bu;
		static type DarkGray = 0xFFA9A9A9u;
		static type DarkGreen = 0xFF006400u;
		static type DarkKhaki = 0xFFBDB76Bu;
		static type DarkMagneta = 0xFF8B008Bu;
		static type DarkOliveGreen = 0xFF556B2Fu;
		static type DarkOrange = 0xFFFF8C00u;
		static type DarkOrchid = 0xFF9932CCu;
		static type DarkRed = 0xFF8B0000u;
		static type DarkSalmon = 0xFFE9967Au;
		static type DarkSeaGreen = 0xFF8FBC8Fu;
		static type DarkSlateBlue = 0xFF483D8Bu;
		static type DarkSlateGray = 0xFF2F4F4Fu;
		static type DarkTurquoise = 0xFF00CED1u;
		static type DarkViolet = 0xFF9400D3u;
		static type DeepPink = 0xFFFF1493u;
		static type DeepSkyeBlue = 0xFF00BFFFu;
		static type DimGray = 0xFF696969u;
		static type DodgerBlue = 0xFF1E90FFu;
		static type FireBrick = 0xFFB22222u;
		static type FloralWhite = 0xFFFFFAF0u;
		static type ForestGreen = 0xFF228B22u;
		static type Fuchsia = 0xFFFF00FFu;
		static type Gainsboro = 0xFFDCDCDCu;
		static type GhostWhite = 0xFFF8F8FFu;
		static type Gold = 0xFFFFD700u;
		static type GoldenRod = 0xFFDAA520u;
		static type Gray = 0xFF808080u;
		static type Green = 0xFF008000u;
		static type GreenYellow = 0xFFADFF2Fu;
		static type HoneyDew = 0xFFF0FFF0u;
		static type HotPink = 0xFFFF69B4u;
		static type IndianRed = 0xFFCD5C5Cu;
		static type Indigo = 0xFF4B0082u;
		static type Ivory = 0xFFFFFFF0u;
		static type Khaki = 0xFFF0E68Cu;
		static type Lavender = 0xFFE6E6FAu;
		static type LavenderBlush = 0xFFFFF0F5u;
		static type LawnGreen = 0xFF7CFC00u;
		static type LemonChiffon = 0xFFFFFACDu;
		static type LightBlue = 0xFFADD8E6u;
		static type LightCoral = 0xFFF08080u;
		static type LightCyan = 0xFFE0FFFFu;
		static type LightGoldenrodYellow = 0xFFFAFAD2u;
		static type LightGray = 0xFFD3D3D3u;
		static type LightGreen = 0xFF90EE90u;
		static type LightPink = 0xFFFFB6C1u;
		static type LightSalmon = 0xFFFFA07Au;
		static type LightSeaGreen = 0xFF20B2AAu;
		static type LightSkyBlue = 0xFF87CEFAu;
		static type LightSlateGray = 0xFF778899u;
		static type LightSteelBlue = 0xFFB0C4DEu;
		static type LightYellow = 0xFFFFFFE0u;
		static type Lime = 0xFF00FF00u;
		static type LimeGreen = 0xFF32CD32u;
		static type Linen = 0xFFFAF0E6u;
		static type Magenta = 0xFFFF00FFu;
		static type Maroon = 0xFF800000u;
		static type MediumAquamarine = 0xFF66CDAAu;
		static type MediumBlue = 0xFF0000CDu;
		static type MediumOrchid = 0xFFBA55D3u;
		static type MediumPurple = 0xFF9370DBu;
		static type MediumSeaGreen = 0xFF3CB371u;
		static type MediumSlateBlue = 0xFF7B68EEu;
		static type MediumSpringGreen = 0xFF00FA9Au;
		static type MediumTurquoise = 0xFF48D1CCu;
		static type MediumVioletRed = 0xFFC71585u;
		static type MidnightBlue = 0xFF191970u;
		static type MintCream = 0xFFF5FFFAu;
		static type MistyRose = 0xFFFFE4E1u;
		static type Moccasin = 0xFFFFE4B5u;
		static type NavajoWhite = 0xFFFFDEADu;
		static type Navy = 0xFF000080u;
		static type OldLace = 0xFFFDF5E6u;
		static type Olive = 0xFF808000u;
		static type OliveDrab = 0xFF6B8E23u;
		static type Orange = 0xFFFFA500u;
		static type OrangeRed = 0xFFFF4500u;
		static type Orchid = 0xFFDA70D6u;
		static type PaleGoldenRod = 0xFFEEE8AAu;
		static type PaleGreen = 0xFF98FB98u;
		static type PaleTurquoise = 0xFFAFEEEEu;
		static type PaleVioletRed = 0xFFDB7093u;
		static type PapayaWhip = 0xFFFFEFD5u;
		static type PeachPuff = 0xFFFFDAB9u;
		static type Peru = 0xFFCD853Fu;
		static type Pink = 0xFFFFC0CBu;
		static type Plum = 0xFFDDA0DDu;
		static type PowderBlue = 0xFFB0E0E6u;
		static type Purple = 0xFF800080u;
		static type Red = 0xFFFF0000u;
		static type RosyBrown = 0xFFBC8F8Fu;
		static type RoyalBlue = 0xFF4169E1u;
		static type SaddleBrown = 0xFF8B4513u;
		static type Salmon = 0xFFFA8072u;
		static type SandyBrown = 0xFFF4A460u;
		static type SeaGreen = 0xFF2E8B57u;
		static type SeaShell = 0xFFFFF5EEu;
		static type Sienna = 0xFFA0522Du;
		static type Silver = 0xFFC0C0C0u;
		static type SkyBlue = 0xFF87CEEBu;
		static type SlateBlue = 0xFF6A5ACDu;
		static type SlateGray = 0xFF708090u;
		static type Snow = 0xFFFFFAFAu;
		static type SpringGreen = 0xFF00FF7Fu;
		static type SteelBlue = 0xFF4682B4u;
		static type Tan = 0xFFD2B48Cu;
		static type Teal = 0xFF008080u;
		static type Thistle = 0xFFD8BFD8u;
		static type Tomato = 0xFFFF6347u;
		static type Transparent = 0x00FFFFFFu;
		static type Turquoise = 0xFF40E0D0u;
		static type Violet = 0xFFEE82EEu;
		static type Wheat = 0xFFF5DEB3u;
		static type White = 0xFFFFFFFFu;
		static type WhiteSmoke = 0xFFF5F5F5u;
		static type Yellow = 0xFFFFFF00u;
		static type YellowGreen = 0xFF9ACD32u;

		template<typename R, typename G, typename B>
		static type from( R r, G g, B b )
		{
			return from( r, g, b, 255u );
		}

		template<typename R, typename G, typename B, typename A>
		static type from( R r, G g, B b, A a )
		{
			return static_cast<type>(((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff));
		}

	};

	class d3d9;
	class d3d9_renderable;
	class d3d9_font;
	struct d3d9_font_struct;

	class d3d9_renderable
	{
	public:
		virtual void draw( const d3d9 &d3d ) const = 0;
		virtual void release( )
		{}
	};

	struct d3d9_font_struct
	{
		d3d9_font_struct( )
			: Height( 0 ), Width( 0 ), Weight( 0 ), MipLevels( 0 ), Italic( false ),
			CharSet( 1l ), OutputPrecision( 0l ), Quality( 0l ),
			PitchAndFamily( 0l | (0l << 4l) )
		{ }
		d3d9_font_struct& enable_antialias( );
		d3d9_font_struct& disable_antialias( );

		int Height;
		unsigned int Width,
			Weight,
			MipLevels;
		bool Italic;
		unsigned long CharSet, OutputPrecision, Quality, PitchAndFamily;
	};

	class d3d9_font
	{
	public:
		d3d9_font( )
			: font_( nullptr )
		{ }
		d3d9_font( const d3d9 &renderer, const std::string &name, d3d9_font_struct params = d3d9_font_struct( ) )
			: font_( )
		{
			create( renderer, *this, name, params );
		}
		template<typename T>
		d3d9_font( const d3d9 &renderer, const std::basic_string<T> &name, d3d9_font_struct params = d3d9_font_struct() )
			: font_( nullptr )
		{
			create( renderer, *this, cvt::codec_cvt<formats::utf_8>::cvt( name ), params );
		}

		enum CalcFlags
		{
			WithoutTrailingSpaces = 1 << 1,
			WithTrailingSpaces = 1 << 2,
			OnlyTrailingSpaces = 1 << 3,
		};

	public:
		Vector2i get_size( const std::string &str, const CalcFlags &flags = WithoutTrailingSpaces ) const;

		Vector2i get_size( const std::wstring &str, const CalcFlags &flags = WithoutTrailingSpaces ) const;

		template<typename T>
		Vector2i get_size( const std::basic_string<T> &str ) const
		{
			return get_size( cvt::codec_cvt<formats::utf_w>( str ) );
		}

		bool valid( ) const
		{
			return font_ != nullptr;
		}

		void *get_native( ) const;

		d3d9_font_struct get_parameters( ) const;

	public:
		void release( );

	private:
		static void create( const d3d9 &renderer, d3d9_font &font, std::string name, const d3d9_font_struct &params );

	private:
		void *font_;
		d3d9_font_struct params_;
	};

	class d3d9
	{
	public:
		d3d9( ) = delete;
		d3d9( handle wnd, bool vsync = false )
			: interface_( nullptr ),
			device_( nullptr ),
			default_( )
		{
			create( wnd, vsync );
		}

		struct CustomF{};
		static const CustomF custom;

	public:
		void draw_frame( const std::function<void( const d3d9& )> &lambda, const std::uint32_t &color = d3d9_color::Black ) const;

		void draw_frame( const d3d9_renderable &renderable, const std::uint32_t &color = d3d9_color::Black ) const;

		template<typename Pred, typename...Tx>
		void draw_frame( Pred &&lambda, const std::uint32_t &color, const CustomF&, Tx&&...tx )
		{
			return begin_draw( [&lambda, &tx...]( const d3d9& gfx )
			{
				std::forward<Pred>( lambda )( gfx, std::forward<Tx>( tx )... );
			},
							   color );
		}

		void draw( const d3d9_renderable & ) const;

		bool valid( ) const;

		void release( );

		void reset( handle &wnd, bool vsync = false ) const;

		void draw_string( const d3d9_font &font, 
						  const std::string &str, 
						  const std::uint32_t &color, 
						  const Vector2i &pos, 
						  const Vector2i &clip ) const;

		void draw_string( const d3d9_font &font, 
						  const std::wstring &str, 
						  const std::uint32_t &color,
						  const Vector2i &pos, 
						  const Vector2i &clip ) const;

		template<typename T>
		void draw_string( const d3d9_font &font,
						  const std::basic_string<T> &str, 
						  const std::uint32_t &color,
						  const Vector2i &pos, 
						  const Vector2i &clip ) const
		{
			return draw_string( font, cvt::codec_cvt<formats::utf_w>::cvt( str ), color, pos, clip );
		}

		d3d9_font &get_default_font( );

		const d3d9_font &get_default_font( ) const;

		void set_default_font( d3d9_font &font );

		void *get_interface( ) const;

		void *get_device( ) const;

		void *get_line_buffer( ) const;

		void *get_sprite( ) const;

	private:
		void create( handle wnd, bool vsync );

	private:
		void *interface_,
			*device_,
			*line_,
			*sprite_;
		d3d9_font default_;
		bool reset_;
	};

	template<typename _Codec = cvt::codec_cvt<>>
	class d3d9_string
		: public d3d9_renderable
	{
	public:
		using codec_cvt = _Codec;
		using string_type = typename codec_cvt::string_type;

	public:
		d3d9_string( string_type str, 
					 d3d9_font font,
					 Vector2i pos,
					 std::uint32_t color )
			: str_( std::move( str ) ),
			font_( font ),
			pos_( std::move( pos ) ),
			color_( color )
		{
			initial_calculations( );
		}

		d3d9_string( d3d9_font font )
			: str_( ),
			font_( font )
		{		
			initial_calculations( );
		}

	public:
		Vector2i get_size_of_trailing_spaces( ) const
		{
			return cached_advanced_size_;
		}

		Vector2i get_size( ) const
		{
			return cached_size_;
		}

		std::uint32_t get_color( ) const
		{
			return color_;
		}

		Vector2i get_position( ) const
		{
			return pos_;
		}

		const string_type &get_text( ) const
		{
			return str_;
		}

		void set_color( std::uint32_t color )
		{
			color_ = color;
		}

		void set_position( Vector2i pos )
		{
			pos_ = pos;
		}

		void set_font( const d3d9_font &font )
		{
			font_ = font;
			initial_calculations( );
		}

		void set_text( std::string text )
		{
			str_ = std::move( text );
			initial_calculations( );
		}

		virtual void draw( const d3d9 &renderer ) const
		{
			renderer.draw_string( font_, str_, color_, pos_, cached_size_ );
		}

	private:
		void initial_calculations( )
		{
			if ( font_.valid( ) )
			{
				cached_size_ = font_.get_size( str_, d3d9_font::CalcFlags::WithoutTrailingSpaces );
				cached_advanced_size_ = font_.get_size( str_, d3d9_font::CalcFlags::OnlyTrailingSpaces );
			}
		}

	private:
		string_type str_;
		d3d9_font font_;
		std::uint32_t color_;
		Vector2i pos_, cached_size_, cached_advanced_size_;
	};

	struct d3d9_line
		: public d3d9_renderable
	{
		d3d9_line( );
		d3d9_line( Vector2i start, Vector2i end, float width, std::uint32_t color );
		Vector2i start, end;
		float width;
		std::uint32_t color;
		virtual void draw( const d3d9 &renderer ) const;
	};

	class d3d9_image_view
		: public d3d9_renderable
	{
	public:
		d3d9_image_view( const d3d9 &gfx, std::istream &stream, Vector2i pos = {0, 0} );
		d3d9_image_view( );

		void load( const d3d9 &gfx, std::istream &stream );

		virtual void draw( const d3d9 &gfx ) const;

		void *get_texture( ) const
		{
			return texture_;
		}

		const Vector2i &get_dimensions( ) const
		{
			return dim_;
		}

		const Vector2i &get_position( ) const
		{
			return pos_;
		}

		void set_position( Vector2i pos )
		{
			pos_ = std::move( pos );
		}

		void scale_x( float amount )
		{
			x_ += amount;
		}

		void scale_y( float amount )
		{
			y_ += amount;
		}

		void shrink_x( float amount )
		{
			x_ -= amount;
		}

		void shrink_y( float amount )
		{
			y_ -= amount;
		}

		const float& scale_x( ) const
		{
			return x_;
		}

		const float& scale_y( ) const
		{
			return y_;
		}

	private:
		void *texture_;
		Vector2i dim_, pos_;
		float x_ = 0.f, y_ = 0.f;
	};

}}