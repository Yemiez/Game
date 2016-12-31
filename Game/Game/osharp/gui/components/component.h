#pragma once
#include "../../formatting/utf.h"
#include "../dimensions.h"
#include "../d3d9.h"
#include "../../concurrency/functional.h"

namespace osharp
{
	namespace gui
	{
		template<typename _Codec>
		struct event_data;

		template<typename _Codec>
		class window;
	}
}

namespace osharp { namespace gui { namespace components {

	template<typename _Codec>
	class component;

	enum ComponentState : std::uint32_t
	{
		EMPTY = 1,
		DRAGGING = 2,
		HOVERING = 4,
		CLICKING = 8,
		RESIZING = 16,
		CHECKED = 32,
		TYPING = 64,
		DISABLED = 128,
	};

	ComponentState operator|( ComponentState lhs, ComponentState rhs )
	{
		return static_cast< ComponentState >( static_cast< std::uint32_t >( lhs ) | static_cast< std::uint32_t >( rhs ) );
	}

	template<typename _Codec>
	struct component_event
	{
		component<_Codec> &sender;
		gui::window<_Codec> *window;
		bool handled;

		component_event( component<_Codec> &sender, gui::window<_Codec> *window )
			: sender( sender ), window( window ), handled( false )
		{ }
	};

	template<typename _Codec = cvt::codec_cvt<>>
	class component
		: public d3d9_renderable
	{
	public:
		using codec_cvt = _Codec;
		using string_type = typename _Codec::string_type;
		using component_event = component_event<codec_cvt>;
	public:
		component( string_type identifier,
				   string_type caption,
				   string_type text,
				   Vector2i position,
				   Vector2i size,
				   component<codec_cvt> &parent,
				   std::uint32_t state,
				   d3d9_font caption_font,
				   d3d9_font text_font )
			: caption_( std::move( caption ) ),
			short_caption_( ),
			text_( std::move( text ) ),
			short_text_( ),
			identifier_( std::move( identifier ) ),
			identifier_id_( std::hash<string_type>{}( identifier_ ) ),
			position_( position ),
			size_( size ),
			parent_( &parent ),
			state_( state ),
			caption_font_( caption_font ),
			text_font_( text_font )
		{ }
		component( string_type identifier,
				   string_type caption,
				   string_type text,
				   Vector2i position,
				   Vector2i size,
				   component<codec_cvt> *parent,
				   std::uint32_t state,
				   d3d9_font caption_font,
				   d3d9_font text_font )
			: caption_( std::move( caption ) ),
			short_caption_( ),
			text_( std::move( text ) ),
			short_text_( ),
			identifier_( std::move( identifier ) ),
			identifier_id_( std::hash<string_type>{}( identifier_ ) ),
			position_( position ),
			size_( size ),
			parent_( parent ),
			state_( state ),
			caption_font_( caption_font ),
			text_font_( text_font )
		{ 
			parent_->add_child( this );
		}
		component( )
			: caption_( ),
			short_caption_( ),
			text_( ),
			identifier_( ),
			identifier_id_( ),
			position_( ),
			size_( ),
			parent_( nullptr ),
			state_( DISABLED ),
			caption_font_( ),
			text_font_( )
		{ }

		// virtuals
	public:
		virtual void draw( const d3d9 &renderer, const window &target ) const
		{ }

		virtual void update( ) const
		{ }

		virtual void set_events( const std::function<concurrency::event<void( event_data<codec_cvt> & ), codec_cvt>&( const string_type & )> &event_function ) const
		{ }

		virtual void erase_events( const std::function<concurrency::event<void( event_data<codec_cvt> & ), codec_cvt>&( const string_type & )> &event_function ) const
		{ }
		

	public:
		bool is_enabled( ) const
		{
			return !(state_ & DISABLED);
		}

		bool is_hovering( ) const
		{
			return ( state_ & HOVERING ) > 0;
		}

		bool is_clicking( ) const
		{
			return ( state_ & CLICKING ) > 0;
		}

		bool is_dragging( ) const
		{
			return ( state_ & DRAGGING ) > 0;
		}

		bool is_resizing( ) const
		{
			return ( state_ & RESIZING ) > 0;
		}
		
		bool is_checked( ) const
		{
			return ( state_ & CHECKED ) > 0;
		}

		bool is_typing( ) const
		{
			return ( state_ & TYPING ) > 0;
		}

		void aflag( const std::uint32_t &mask )
		{
			state_ |= mask;
		}

		void rflag( const std::uint32_t &mask )
		{
			state_ &= ~mask;
		}

	private:
		const string_type &get_caption( ) const
		{
			return caption_;
		}

		string_type &get_caption( )
		{
			return caption_;
		}

		const string_type &get_short_caption( ) const
		{
			return short_caption_;
		}

		const string_type &get_text( ) const
		{
			return text_;
		}

		string_type &get_text( )
		{
			return text_;
		}

		const string_type &get_text_short( ) const
		{
			return text_;
		}

		const string_type &get_identifier( ) const
		{
			return identifier_;
		}

		string_type &get_identifier( ) 
		{
			return identifier_;
		}

		const std::uint32_t &get_identifier_id( ) const
		{
			return identifier_id_;
		}

		const std::uint32_t &get_state( ) const
		{
			return state_;
		}

		std::uint32_t &get_state( )
		{
			return state_;
		}

		const Vector2i &get_position( ) const
		{
			return position_;
		}

		Vector2i &get_position( )
		{
			return position_;
		}

		const Vector2i &get_size( ) const
		{
			return size_;
		}

		Vector2i &get_size( )
		{
			return size_;
		}

		const component<codec_cvt> *get_parent( ) const
		{
			return parent_;
		}

		component<codec_cvt> *get_parent( )
		{
			return parent_;
		}

		const d3d9_font &get_caption_font( ) const
		{
			return caption_font_;
		}

		d3d9_font &get_caption_font( )
		{
			return caption_font_;
		}

		const d3d9_font &get_text_font( ) const
		{
			return text_font_;
		}

		d3d9_font &get_text_font( )
		{
			return text_font_;
		}

		const std::vector<component<codec_cvt>> &get_children( ) const
		{
			return children_;
		}

		std::vector<component<codec_cvt>> &get_children( )
		{
			return children_;
		}

	public:
		void set_parent( component<codec_cvt> *parent )
		{
			if ( parent_ )
				parent_->remove_child( this );
			parent_ = parent;
			parent->add_child( this );
		}

		void add_child( component<codec_cvt> *child )
		{
			if ( child_exists( child ) )
				return;
			children_.emplace_back( child );
			on_child_added_.( component_event{ *this, nullptr } );
		}

		void add_brother( component<codec_cvt> *brother )
		{
			if ( brother_exists( brother ) )
				return;
			brethren_.emplace_back( brother );
			on_brother_added_( component_event{ *this nullptr } );
		}

		bool child_exists( component<codec_cvt> *child ) const
		{
			return child_exists( child->get_identifier_id( ) );
		}

		bool child_exists( const string_type &child_name ) const
		{
			return child_exists( std::hash<string_type>( )( child_name ) );
		}

		bool child_exists( const std::uint32_t &child_id ) const
		{
			for ( auto &x : children_ )
				if ( x.get_identifier_id( ) == child_id )
					return true;
			return false;
		}

		bool brother_exists( component<codec_cvt> *child ) const
		{
			return brother_exists( child->get_identifier_id( ) );
		}

		bool brother_exists( const string_type &child_name ) const
		{
			return brother_exists( std::hash<string_type>( )( child_name ) );
		}

		bool brother_exists( const std::uint32_t &child_id ) const
		{
			for ( auto &x : brethren_ )
				if ( x.get_identifier_id( ) == child_id )
					return true;
			return false;
		}

		void remove_child( const std::uint32_t &id )
		{
			for ( auto it = children_.begin( ), end = children_.end( );
				  it < end;
				  ++it )
				if ( it->get_identifier_id( ) == id )
				{
					children_.erase( it );
					break;
				}
		}

		void remove_child( const component<codec_cvt> *child )
		{
			return remove_child( child->get_identifier_id( ) );
		}

		void remove_child( const string_type &id )
		{
			return remove_child( std::hash<string_type>( )( id ) );
		}

		void remove_brother( const std::uint32_t &id )
		{
			for ( auto it = brethren_.begin( ), end = brethren_.end( );
				  it < end;
				  ++it )
				if ( it->get_identifier_id( ) == id )
				{
					brethren_.erase( it );
					break;
				}
		}

		void remove_brother( const component<codec_cvt> *brother )
		{
			return remove_brother( brother->get_identifier_id( ) );
		}

		void remove_brother( const string_type &id )
		{
			return remove_brother( std::hash<string_type>( )( id ) );
		}

	private:
		string_type caption_,
			short_caption_,
			text_,
			short_text_,
			identifier_;
		std::uint32_t identifier_id_;
		Vector2i position_,
			size_;
		component<codec_cvt> *parent_;
		std::uint32_t state_;
		d3d9_font caption_font_,
			text_font_;
		std::vector<component<codec_cvt>> children_,
			brethren_;

	private:
		concurrency::event<void( component_event& ), _Codec> on_child_added_,
			on_child_removed_,
			on_brother_added_;
	};


}}}