#pragma once
#include <mutex>
#include <type_traits>
#include <vector>
#include <string>
#include "functional.h"

namespace osharp { namespace concurrency {

	namespace details
	{
		template<typename _Mtx>
		using lock_guard = std::unique_lock<_Mtx>;

		template<typename _Base>
		class synchro_guard
		{
		public:


		public:
			using base_type = _Base;
			using t_type = typename base_type::t_type;
			using mutex_type = typename base_type::mutex_type;
			using self_type = synchro_guard<base_type>;
			using lock_type = lock_guard<mutex_type>;

		public:
			synchro_guard( mutex_type &mtx, t_type &t )
				: lock_( mtx ), ptr_( &t )
			{ }

			synchro_guard( self_type &&other )
				: lock_( std::move( other.lock_ ) ), ptr_( other.ptr_ )
			{ }

			synchro_guard( const self_type &other )
				: lock_( other.lock_ ), ptr_( other.ptr_ )
			{ }

			// accessing operators
		public:
			auto
				operator->( )->t_type*
			{
				return ptr_;
			}

			auto
				operator->( ) const->const t_type*
			{
				return ptr_;
			}

				auto
				operator*( )->typename t_type&
			{
				return *ptr_;
			}

			auto
				operator*( ) const->const t_type&
			{
				return *ptr_;
			}

				operator
				const t_type&( ) const
			{
				return *ptr_;
			}

			operator
				t_type&( )
			{
				return *ptr_;
			}

			operator
				const t_type*( ) const
			{
				return ptr_;
			}

			operator
				t_type*( )
			{
				return ptr_;
			}

			// arithmetic/modifying operators
		public:
			auto
				operator++( )->self_type&
			{
				++( *ptr_ );
				return *this;
			}

			auto
				operator--( )->self_type&
			{
				--( *ptr_ );
				return *this;
			}

			// assignment operators
		public:
			template<typename _Rhs>
			auto
				operator=( _Rhs &&rhs )->self_type&
			{
				*ptr_ = std::forward<_Rhs>( rhs );
				return *this;
			}

			auto
				operator=( const self_type &rhs )->self_type&
			{
				*ptr_ = *rhs.ptr_;
				return *this;
			}

			template<typename _Rhs>
			auto
				operator+( _Rhs &&rhs )->t_type
			{
				return ( *ptr_ ) + rhs;
			}

			auto
				operator+( const self_type &rhs )->self_type&
			{
				return ( *ptr_ ) + ( *rhs.ptr_ );
			}

			template<typename _Rhs>
			auto
				operator-( _Rhs &&rhs )->t_type
			{
				return ( *ptr_ ) - rhs;
			}

			auto
				operator-( const self_type &rhs )->self_type&
			{
				return ( *ptr_ ) - ( *rhs.ptr_ );
			}

			template<typename _Rhs>
			auto
				operator*( _Rhs &&rhs )->t_type
			{
				return ( *ptr_ ) * rhs;
			}

			auto
				operator*( const self_type &rhs )->self_type&
			{
				return ( *ptr_ ) * ( *rhs.ptr_ );
			}

			template<typename _Rhs>
			auto
				operator/( _Rhs &&rhs )->t_type
			{
				return ( *ptr_ ) / rhs;
			}

			auto
				operator/( const self_type &rhs )->self_type&
			{
				return ( *ptr_ ) / ( *rhs.ptr_ );
			}

			template<typename _Rhs>
			auto
				operator%( _Rhs &&rhs )->t_type
			{
				return ( *ptr_ ) % rhs;
			}

			auto
				operator%( const self_type &rhs )->self_type&
			{
				return ( *ptr_ ) % ( *rhs.ptr_ );
			}

			template<typename _Rhs>
			auto
				operator&( _Rhs &&rhs )->t_type
			{
				return ( *ptr_ ) & rhs;
			}

			auto
				operator&( const self_type &rhs )->self_type&
			{
				return ( *ptr_ ) & ( *rhs.ptr_ );
			}

			template<typename _Rhs>
			auto
				operator|( _Rhs &&rhs )->t_type
			{
				return ( *ptr_ ) | rhs;
			}

			auto
				operator|( const self_type &rhs )->self_type&
			{
				return ( *ptr_ ) | ( *rhs.ptr_ );
			}

			template<typename _Rhs>
			auto
				operator^( _Rhs &&rhs )->t_type
			{
				return ( *ptr_ ) ^ rhs;
			}

			auto
				operator^( const self_type &rhs )->self_type&
			{
				return ( *ptr_ ) ^ ( *rhs.ptr_ );
			}

			template<typename _Rhs>
			auto
				operator+=( _Rhs &&rhs )->self_type&
			{
				*ptr_ += std::forward<_Rhs>( rhs );
				return *this;
			}

			auto
				operator+=( const self_type &rhs )->self_type&
			{
				*ptr_ += *rhs.ptr_;
				return *this;
			}

			template<typename _Rhs>
			auto
				operator-=( _Rhs &&rhs )->self_type&
			{
				*ptr_ -= std::forward<_Rhs>( rhs );
				return *this;
			}

			auto
				operator-=( const self_type &rhs )->self_type&
			{
				*ptr_ -= *rhs.ptr_;
				return *this;
			}

			template<typename _Rhs>
			auto
				operator*=( _Rhs &&rhs )->self_type&
			{
				*ptr_ *= std::forward<_Rhs>( rhs );
				return *this;
			}

			auto
				operator*=( const self_type &rhs )->self_type&
			{
				*ptr_ *= *rhs.ptr_;
				return *this;
			}

			template<typename _Rhs>
			auto
				operator/=( _Rhs &&rhs )->self_type&
			{
				*ptr_ /= std::forward<_Rhs>( rhs );
				return *this;
			}

			auto
				operator/=( const self_type &rhs )->self_type&
			{
				*ptr_ /= *rhs.ptr_;
				return *this;
			}

			template<typename _Rhs>
			auto
				operator%=( _Rhs &&rhs )->self_type&
			{
				*ptr_ %= std::forward<_Rhs>( rhs );
				return *this;
			}

			auto
				operator%=( const self_type &rhs )->self_type&
			{
				*ptr_ %= *rhs.ptr_;
				return *this;
			}

			template<typename _Rhs>
			auto
				operator&=( _Rhs &&rhs )->self_type&
			{
				*ptr_ /= std::forward<_Rhs>( rhs );
				return *this;
			}

			auto
				operator&=( const self_type &rhs )->self_type&
			{
				*ptr_ &= *rhs.ptr_;
				return *this;
			}

			template<typename _Rhs>
			auto
				operator|=( _Rhs &&rhs )->self_type&
			{
				*ptr_ |= std::forward<_Rhs>( rhs );
				return *this;
			}

			auto
				operator|=( const self_type &rhs )->self_type&
			{
				*ptr_ |= *rhs.ptr_;
				return *this;
			}

			template<typename _Rhs>
			auto
				operator^=( _Rhs &&rhs )->self_type&
			{
				*ptr_ ^= std::forward<_Rhs>( rhs );
				return *this;
			}

			auto
				operator^=( const self_type &rhs )->self_type&
			{
				*ptr_ ^= *rhs.ptr_;
				return *this;
			}

			// logical operators
		public:
			template<typename _Rhs>
			auto
				operator==( const _Rhs &rhs )->bool
			{
				return ( *ptr_ ) == rhs;
			}

			auto
				operator==( const self_type &other )->bool
			{
				return ( *ptr_ ) == ( *other.ptr_ );
			}

			template<typename _Rhs>
			auto
				operator!=( const _Rhs &rhs )->bool
			{
				return !( ( *this ) == rhs );
			}

			template<typename _Rhs>
			auto
				operator&&( _Rhs &&rhs )->bool
			{
				return ( *ptr_ ) && std::forward<_Rhs>( rhs );
			}

			auto
				operator&&( const self_type &rhs )->self_type&
			{
				return ( *ptr_ ) && ( *rhs.ptr_ );
			}

			template<typename _Rhs>
			auto
				operator||( _Rhs &&rhs )->bool
			{
				return ( *ptr_ ) || std::forward<_Rhs>( rhs );
			}

			auto
				operator||( const self_type &rhs )->self_type&
			{
				return ( *ptr_ ) || ( *rhs.ptr_ );
			}

		private:
			t_type *ptr_;
			lock_type lock_;
		};

		template<typename _Mtx>
		struct mtx_traits
		{
			using mutex_type = _Mtx;
			static constexpr auto is_standard_mutex = std::is_base_of<std::_Mutex_base, mutex_type>::value;
		};

	}


	template<typename T,
		typename _Mtx = std::timed_mutex /* important, try_access_for and try_access_until is only available with timed mutexes */>
		class synchronized_object
	{
	public:
		using t_type = T;
		using mutex_type = _Mtx;
		using guard_type = details::synchro_guard<synchronized_object<t_type, mutex_type>>;
	public:
		synchronized_object( std::nullptr_t )
			: t_( ), mtx_( )
		{ }

		template<typename..._Inits>
		synchronized_object( _Inits&&...inits )
			: t_( std::forward<_Inits>( inits )... ), mtx_( )
		{ }

		synchronized_object( )
			: t_( ), mtx_( )
		{ }

		synchronized_object( const synchronized_object &other )
			: t_( other.t_ ), mtx_( )
		{ }

		synchronized_object( synchronized_object &&other )
			: t_( std::move( other.t_ ) ), mtx_( )
		{ }

		static constexpr auto
			is_standard_mutex( )->bool
		{
			return details::mtx_traits<mutex_type>::is_standard_mutex;
		}

		static constexpr auto
			is_t_arithmetic( )->bool
		{
			return std::is_arithmetic<t_type>::value;
		}

		static constexpr auto
			is_t_integral( )->bool
		{
			return std::is_integral<t_type>::value;
		}

		static constexpr auto
			is_t_floating( )->bool
		{
			return std::is_floating_point<t_type>::value;
		}

		static constexpr auto
			is_t_reference( )->bool
		{
			return std::is_reference<t_type>::value;
		}

		static constexpr auto
			is_t_pointer( )->bool
		{
			return std::is_pointer<t_type>::value;
		}

		static constexpr auto
			is_t_abstract( )->bool
		{
			return std::is_abstract<t_type>::value;
		}

		static constexpr auto
			is_t_array( )->bool
		{
			return std::is_array<t_type>::value;
		}
		auto
			get_mutex( )->mutex_type&
		{
			return mtx_;
		}

		auto
			try_lock( )->bool
		{
			return mtx_.try_lock( );
		}

		auto
			lock( )->void
		{
			return mtx_.lock( );
		}

		auto
			unlock( )->void
		{
			return mtx_.unlock( );
		}

		auto
			read_only()->const t_type*
		{
			return &t_;
		}

		auto
			operator->( )->guard_type
		{
			return guard_type( mtx_, t_ );
		}

		auto
			operator->( ) const->guard_type
		{
			return guard_type( mtx_, t_ );
		}

		auto
			operator*( )->guard_type
		{
			return guard_type( mtx_, t_ );
		}

		auto
			operator*( ) const->guard_type
		{
			return guard_type( mtx_, t_ );
		}

		template<typename _Fun, typename..._Ax>
		auto
			access( _Fun &&fun, _Ax&&...ax )->typename std::result_of<
			_Fun( t_type&, _Ax... )
			>::type
		{
			details::lock_guard<mutex_type> guard{ mtx_ };
			return std::forward<_Fun>( fun )( t_, std::forward<_Ax>( ax )... );
		}

		template<typename _Fun, typename..._Ax>
		auto
			caccess( _Fun &&fun, _Ax&&...ax )->typename std::result_of<
			_Fun( const t_type&, _Ax... )
			>::type
		{
			details::lock_guard<mutex_type> guard{ mtx_ };
			return std::forward<_Fun>( fun )( t_, std::forward<_Ax>( ax )... );
		}

		template<typename _Fun, typename..._Ax>
		auto
			try_access( _Fun &&fun, _Ax&&...ax )->bool
		{
			details::lock_guard<mutex_type> guard{ mtx_, std::try_to_lock };
			if ( guard.owns_lock( ) )
			{
				std::forward<_Fun>( fun )( t_, std::forward<_Ax>( ax )... );
				return true;
			}
			return false;
		}

		template<typename _Rep, typename _Period, typename _Fun, typename..._Ax>
		auto
			try_access_for( const std::chrono::duration<_Rep, _Period> &duration,
							_Fun &&fun,
							_Ax&&...ax )->bool
		{
			details::lock_guard<mutex_type> guard{ mtx_, duration };
			if ( guard.owns_lock( ) )
			{
				std::forward<_Fun>( fun )( t_, std::forward<_Ax>( ax )... );
				return true;
			}
			return false;
		}

		template<typename _Clock, typename _Duration, typename _Fun, typename..._Ax>
		auto
			try_access_until( const std::chrono::time_point<_Clock, _Duration> &time_point,
							  _Fun &&fun,
							  _Ax&&...ax )
		{
			details::lock_guard<mutex_type> guard{ mtx_, time_point };
			if ( guard.owns_lock( ) )
			{
				std::forward<_Fun>( fun )( t_, std::forward<_Ax>( ax )... );
				return true;
			}
			return false;
		}


	private:
		t_type t_;
		mutex_type mtx_;
	};


} }
