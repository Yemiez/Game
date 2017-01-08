#pragma once
#include <limits>


namespace osharp { namespace gui {

	template<typename T>
	struct Vector2
	{
		Vector2( T x, T y )
			: x( x ), y( y )
		{ }
		Vector2( )
			: x( static_cast<T>( 0 ) ),
			y( static_cast<T>( 0 ) )
		{ }

		template<typename OT>
		explicit operator Vector2<OT>( )
		{
			return{ static_cast< OT >( x ), static_cast< OT >( y ) };
		}

		auto length_square( ) const
		{
			return x * x + y * y;
		}

		auto length( ) const
		{
			return ::sqrt( length_square( ) );
		}

		auto &normalize( )
		{
			const auto len = length( );
			x /= len;
			y /= len;
			return *this;
		}

		auto normalised( ) const
		{
			return Vector2<T>( *this ).normalise( );
		}

	public:
		auto operator-( ) const
		{
			return Vector2( -x, -y );
		}

		template<typename Ty>
		auto &operator=( const Vector2<Ty> &other )
		{
			x = static_cast<T>( other.x );
			y = static_cast<T>( other.y );
			return *this;
		}

		template<typename Ty>
		auto &operator+=( const Vector2<Ty> &other )
		{
			x += static_cast<T>( other.x );
			y += static_cast<T>( other.y );
			return *this;
		}

		template<typename Ty>
		auto &operator-=( const Vector2<Ty> &other )
		{
			x -= static_cast<T>( other.x );
			y -= static_cast<T>( other.y );
			return *this;
		}
		
		template<typename Ty>
		auto operator*( const Vector2<Ty> &other ) const
		{
			return x * static_cast<T>( other.x ) + y * static_cast<T>( other.y );
		}

		template<typename Ty>
		auto operator+( const Vector2<Ty> &other ) const
		{
			return Vector2( *this ) += other;
		}

		template<typename Ty>
		auto operator-( const Vector2<Ty> &other ) const
		{
			return Vector2( *this ) -= other;
		}

		template<typename Ty>
		auto &operator*=( const Ty &other )
		{
			x *= static_cast<T>( other ); 
			y *= static_cast<T>( other );
			return *this;
		}

		template<typename Ty>
		auto operator*( const Ty &other ) const
		{
			return Vector2( *this ) *= other;
		}

		template<typename Ty>
		auto &operator/=( const Ty &other )
		{
			x /= static_cast<T>( other );
			y /= static_cast<T>( other );
			return *this;
		}

		template<typename Ty>
		auto operator/( const Ty &other ) const
		{
			return Vector2( *this ) /= other;
		}

		template<typename Ty>
		auto operator==( const Vector2<Ty> &other ) const
		{
			return x == static_cast<T>( other.x ) && y == static_cast<T>( other.y );
		}

		template<typename Ty>
		auto operator!=( const Vector2<Ty> &other ) const
		{
			return !( *this == other );
		}

		template<typename Ty>
		auto operator>( const Vector2<Ty> &other ) const
		{
			return x > other.x && y > other.y;
		}

		template<typename Ty>
		auto operator>=( const Vector2<Ty> &other ) const
		{
			return x >= other.x && y >= other.y;
		}

		template<typename Ty>
		auto operator<( const Vector2<Ty> &other ) const
		{
			return x < other.x && y < other.y;
		}

		template<typename Ty>
		auto operator<=( const Vector2<Ty> &other ) const
		{
			return x <= other.x && y <= other.y;
		}

		T x, y;
	};

	using Vector2f = Vector2<float>;
	using Vector2i = Vector2<int>;
	using Vector2u = Vector2<unsigned int>;
	using Vector2d = Vector2<double>;



} }