#pragma once
#include "component.h"

namespace osharp { namespace gui { namespace components {

	template<typename _Codec = cvt::codec_cvt<>>
	class label : public component<_Codec>
	{
	public:
		using codec = _Codec;
		using base = component<codec>;
		using string_type = typename base::string_type;
		using component_event = typename base::component_event;

	public:
		label( string_type identifier, 
			   string_type text,
			   Vector2i position,
			   Vector2i size,
			   d3d9_font font,
			   std::shared_ptr<color_palette<codec>> palette )
			: base( std::move( identifier ),
					std::move( text ),
					{},
					position,
					size,
					nullptr,
					ComponentState::EMPTY,
					std::move( font ),
					{},
					palette )

		{}
		label( string_type identifier,
			   string_type text,
			   Vector2i position,
			   Vector2i size,
			   component<codec> &parent,
			   d3d9_font font,
			   std::shared_ptr<color_palette<codec>> palette )
			: base( std::move( identifier ),
					std::move( text ),
					{},
					position,
					size,
					parent,
					ComponentState::EMPTY,
					std::move( font ),
					{},
					palette )

		{}
		label( string_type identifier,
			   string_type text,
			   Vector2i position,
			   Vector2i size,
			   component<codec> *parent,
			   d3d9_font font,
			   std::shared_ptr<color_palette<codec>> palette )
			: base( std::move( identifier ),
					std::move( text ),
					{},
					position,
					size,
					parent,
					ComponentState::EMPTY,
					std::move( font ),
					{},
					palette )

		{}
		label()
			: base()
		{}

	public:
		virtual void draw( const d3d9 &gfx )
		{
			auto pos = ( this->parent_ ? this->parent_->get_position( ) : Vector2i{ 0, 0 } ) + this->position_;
			gfx.draw_string( this->caption_font_,
							 this->caption_,
							 this->palette_->get_or_default( "label", "foreground", { d3d9_color::White } ).raw,
							 pos,
							 this->size_ );
		}

		virtual void set_events( const std::function<concurrency::event<void( event_data<codec> & ), codec>&( const string_type & )> &event_function )
		{
			event_function( "mouse" ) += [this]( event_data<codec> &ev )
			{
				this->mouse_event( reinterpret_cast<window_mouse_data<codec>&>( ev ) );
			};
		}

		virtual void erase_events( const std::function<concurrency::event<void( event_data<codec> & ), codec>&( const string_type & )> &event_function )
		{

		}


	public:
		void mouse_event( window_mouse_data<codec> &ev )
		{

		}

	};

}}}