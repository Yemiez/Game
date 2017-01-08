#pragma once
#include <chrono>


namespace osharp { namespace concurrency {

	template<typename _Clock = std::chrono::steady_clock>
	class frame_timer
	{
	public:
		using clock = _Clock;
		using time_point = typename _Clock::time_point;

	public:
		frame_timer( )
			: last_( clock::now() ),
			dur_( ),
			dt_prog_( 0.0f ),
			frame_count_( 0u ),
			frame_average_( 0u )
		{}

		float mark( )
		{
			++frame_count_;
			auto tmp = last_;
			last_ = clock::now( );
			dur_ = std::chrono::duration_cast<std::chrono::duration<float>>(last_ - tmp);
			auto dt = dur_.count( );
			dt_prog_ += dt;
			if ( dt_prog_ >= 1.0f )
			{
				frame_average_ = frame_count_;
				frame_count_ = 0;
				dt_prog_ = 0.0f;
			}
			return dt;
		}

		const std::chrono::duration<float> &get_duration( ) const
		{
			return dur_;
		}

		const auto &get_last( ) const
		{
			return last_;
		}

		uint32_t get_fps( ) const
		{
			return frame_average_;
		}

	private:
		time_point last_;
		std::chrono::duration<float> dur_;
		float dt_prog_;
		uint32_t frame_count_,
			frame_average_;
	};

}}