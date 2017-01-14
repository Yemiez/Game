/********************************************************
*	*	*	*	*	*	*	*	*	*	*	*	*	*	*
*	*	*	*	*	*	*	*	*	*	*	*	*	*	*
*	*				   async_io.h				    *	*
*  	*			   Created by Yamiez				*	*
*  	*	Do not forget to give credit when using		*	*
*	*	*	*	*	*	*	*	*	*	*	*	*	*	*
*	*	*	*	*	*	*	*	*	*	*	*	*	*	*
*********************************************************/
#pragma once
#include <string>
#include <vector>
#include <type_traits>
#include <functional>
#include <future>
#include "synchronized_object.h"


namespace osharp { namespace concurrency { 

	template<typename _Codec>
	class io_task;
	template<typename _Fty, typename _Codec>
	using io_function = event_proxy<_Fty, _Codec>; 
	template<typename _Codec>
	class async_io;

	class io_error : public std::exception
	{
	public:
		using base = std::exception;

	public:
		io_error( const char *str )
			: base( str )
		{ }

		io_error( const std::string &str )
			: base( str.c_str( ) )
		{ }

		io_error( )
			: base( "unknown io error" )
		{ }
	};

	template<typename _Codec = cvt::codec_cvt<>>
	class io_task
	{
	public:
		using codec_cvt = _Codec;
		using string_type = typename codec_cvt::string_type;
		using self_type = io_task<codec_cvt>;
		using event_function = io_function<void( self_type & ), _Codec>;

	public:
		io_task( )
			: finished_( ), fut_( )
		{ }

		io_task( string_type name )
			: finished_( name, nullptr ), fut_( )
		{ }

		io_task( const io_task &other )
			: finished_( name, other.finished_.get_target( ) ), fut_( )
		{ }

		io_task( io_task &&other )
			: finished_( std::move( other.finished_ ) ), fut_( std::move( fut_ ) )
		{ }

		// methods
	public:
		auto
			is_alive( ) const->bool
		{
			return fut_.valid( ) &&
				fut_.wait_for( std::chrono::milliseconds( 0 ) ) != std::future_status::ready;
		}

		auto
			has_result( )->bool
		{
			return fut_.valid( ) &&
				fut_.wait_for( std::chrono::milliseconds( 0 ) ) == std::future_status::ready;
		}

		auto
			get_future( ) const->const std::future<void>&
		{
			return fut_;
		}

		auto
			get_future( )->std::future<void>&
		{
			return fut_;
		}

		template<typename _Fun, typename..._Ax>
		auto
			start_task( _Fun &&fun, _Ax&&...ax )->bool
		{
			if ( is_alive( ) )
				throw io_error( "io_task::start_task: task is already started" );
			fut_ = std::async( std::launch::async, [&]( )->void
			{
				std::forward<_Fun>( fun )( std::forward<_Ax>( ax )... );
				if ( finished_ )
					finished_( std::ref( *this ) );
			} );
			return fut_.valid( );
		}

		template<typename _Fun>
		auto
			then( _Fun &&fun )->self_type&
		{
			finished_ = std::forward<_Fun>( fun );
			return *this;
		}

		auto
			get_on_finished( ) const->const event_function&
		{
			return finished_;
		}

			auto
			get_on_finised( )->event_function&
		{
			return finished_;
		}

		auto
			operator=( const io_task &other )->io_task
		{
			finished_ = other.finished_;
			fut_ = std::future<void>( );
			return *this;
		}

	private:
		event_function finished_;
		std::future<void> fut_;
	};

	template<typename _Codec = cvt::codec_cvt<>>
	class async_io
	{
	public:
		using codec_cvt = _Codec;
		using string_type = typename codec_cvt::string_type;
		using self_type = async_io<codec_cvt>;
		using io_task = concurrency::io_task<codec_cvt>;

	public:
		async_io( )
			: tasks_( ), fut_( ), running_( false )
		{ }

		async_io( self_type &&other )
			: tasks_( std::move( other.tasks_ ) )
		{ }

		async_io( const self_type &other )
			: tasks_( other.tasks_ )
		{ }

		// io related
	public:
		auto
			is_alive( )->bool
		{
			return ( fut_.valid( ) &&
					 fut_.wait_for( std::chrono::milliseconds( 0 ) ) != std::future_status::ready ) ||
				*running_;
		}

		auto
			is_running( )->bool
		{
			return *running_;
		}


		auto
			run_async( )->void
		{
			fut_ = std::async( std::launch::async, &self_type::run_sync, this );
		}

		auto
			run_sync( )->void
		{
			if ( is_running( ) )
				return;
			*running_ = true;
			while ( *running_ )
			{
				tasks_.access( [this]( std::vector<std::shared_ptr<io_task>> &tasks )
				{
					for ( auto it = tasks.begin( ), end = tasks.end( );
						  it < end; )
					{
						if ( !( *it )->is_alive( ) )
						{
							it = tasks.erase( it );
							end = tasks.end( );
							if ( tasks.empty( ) )
								break;
						}
						else
							++it;
						std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
					}
				} );
				std::this_thread::sleep_for( std::chrono::milliseconds( 15 ) );
			}
		}

		auto
			stop_io( )->void
		{
			*running_ = false;
		}

		// task related
	public:
		template<typename _Fun, typename..._Ax>
		io_task& emplace_task( _Fun &&fun, _Ax&&...ax )
		{
			return tasks_.access( [&]( std::vector<std::shared_ptr<io_task>> &tasks )->io_task&
			{
				tasks.emplace_back( new io_task( ) );
				auto task = tasks.back( );
				task->start_task( std::forward<_Fun>( fun ), *this, std::forward<_Ax>( ax )... );
				return *task;
			} );
		}

		template<typename _Str, typename _Fun, typename..._Ax>
		io_task& emplace_task( _Str &&name, _Fun &&fun, _Ax&&...ax )
		{
			return tasks_.access( [&]( std::vector<std::shared_ptr<io_task>> &tasks )->io_task&
			{
				tasks.emplace_back( new io_task( std::forward<_Str>( name ) ) );
				auto task = tasks.back( );
				task->start_task( std::forward<_Fun>( fun ), *this, std::forward<_Ax>( ax )... );
				return *task;
			} );
		}

	private:
		synchronized_object<std::vector<std::shared_ptr<io_task>>> tasks_;
		std::future<void> fut_;
		synchronized_object<bool> running_;
	};

}}