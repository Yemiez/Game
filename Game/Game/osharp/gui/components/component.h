#pragma once
#include "../../formatting/utf.h"
#include "../dimensions.h"
#include "../d3d9.h"
#include "../../concurrency/functional.h"
#include "pallete.h"

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

	static ComponentState operator|( ComponentState lhs, ComponentState rhs )
	{
		return static_cast< ComponentState >( static_cast< std::uint32_t >( lhs ) | static_cast< std::uint32_t >( rhs ) );
	}

	template<typename _Codec>
	struct component_event_data
	{
		component<_Codec> &sender;
		bool handled;

		component_event_data( component<_Codec> &sender )
			: sender( sender ), handled( false )
		{ }
	};

	template<typename _Codec = cvt::codec_cvt<>>
	class component
	{
	public:
		using codec = _Codec;
		using string_type = typename _Codec::string_type;
		using component_event = component_event_data<codec>;
	public:
		component( string_type identifier,
				   string_type caption,
				   string_type text,
				   Vector2i position,
				   Vector2i size,
				   component<codec> &parent,
				   std::uint32_t state,
				   d3d9_font caption_font,
				   d3d9_font text_font,
				   std::shared_ptr<color_palette<codec>> palette )
			: component( identifier, caption, text, position, size, &parent, state, caption_font, text_font, palette )
		{ }
		component( string_type identifier,
				   string_type caption,
				   string_type text,
				   Vector2i position,
				   Vector2i size,
				   component<codec> *parent,
				   std::uint32_t state,
				   d3d9_font caption_font,
				   d3d9_font text_font,
				   std::shared_ptr<color_palette<codec>> palette )
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
			text_font_( text_font ),
			palette_( std::move( palette ) )
		{ 
			if ( parent_ )
				parent_->add_child( this );
			++ID_COUNTER;
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
		{ 
			++ID_COUNTER;
		}

		// virtuals
	public:
		virtual void draw( const d3d9 &renderer )
		{ }

		virtual void update( ) const
		{ }

		virtual void set_events( const std::function<concurrency::event<void( event_data<codec> & ), codec>&( const string_type & )> &event_function )
		{ 
			events_.at( codec::cvt( "child_added" ) ) = &on_child_added_;
			events_.at( codec::cvt( "child_removed" ) ) = &on_child_removed_;
		}

		virtual void erase_events( const std::function<concurrency::event<void( event_data<codec> & ), codec>&( const string_type & )> &event_function )
		{
			// ... Doesn't set any events
		}
		

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

	public:
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

		const component<codec> *get_parent( ) const
		{
			return parent_;
		}

		component<codec> *get_parent( )
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

		const std::vector<component<codec>> &get_children( ) const
		{
			return children_;
		}

		std::vector<component<codec>> &get_children( )
		{
			return children_;
		}

	public:
		// trim any type of space character from right to left (end to begin)
		void trimr( string_type &target )
		{
			target.erase( std::find_if( target.rbegin( ),
										target.rend( ), 
										[]( const auto &fmt )
			{
				return !codec::is_space( fmt );
			} ).base( ),
						  target.end( ) );
		}

		virtual void cut_caption( const Vector2i &clip )
		{
			if ( caption_font_.valid( ) )
			{
				auto size = caption_font_.get_size( caption_, d3d9_font::CalcFlags::WithoutTrailingSpaces );
				auto spaces = caption_font_.get_size( caption_, d3d9_font::CalcFlags::OnlyTrailingSpaces );

				short_caption_ = caption_;

				if ( spaces != Vector2i{ 0, 0 } )
					trimr( short_caption_ );
				
				auto last = short_caption_.end( );
				while ( size.x > clip.x )
				{
					short_caption_.replace( last - 1, short_caption_.end( ), "..." );
					last = short_caption_.end( ) - 3;
					size = caption_font_.get_size( short_caption_, d3d9_font::CalcFlags::WithoutTrailingSpaces );
				}
			}
		}

		virtual void cut_text( const Vector2i &clip )
		{
			if ( text_font_.valid( ) )
			{
				auto size = text_font_.get_size( text_, d3d9_font::CalcFlags::WithoutTrailingSpaces );
				auto spaces = text_font_.get_size( text_, d3d9_font::CalcFlags::OnlyTrailingSpaces );

				short_text_ = text_;

				if ( spaces != Vector2i{ 0, 0 } )
					trimr( short_text_ );

				auto last = short_text_.end( );
				while ( size.x > clip.x )
				{
					short_text_.replace( last - 1, short_text_.end( ), "..." );
					last = short_text_.end( ) - 3;
					size = caption_font_.get_size( short_text_, d3d9_font::CalcFlags::WithoutTrailingSpaces );
				}
			}
		}

	public:
		void set_parent( component<codec> *parent )
		{
			if ( parent_ )
				parent_->remove_child( this );
			parent_ = parent;
			parent->add_child( this );
		}

		void add_child( component<codec> *child )
		{
			if ( child_exists( child ) )
				return;
			children_.emplace_back( child );
			on_child_added_( component_event{ *this } );
		}

		bool child_exists( component<codec> *child ) const
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
				if ( x->get_identifier_id( ) == child_id )
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
					on_child_removed_( component_event{ *this } );
					children_.erase( it );
					break;
				}
		}

		void remove_child( const component<codec> *child )
		{
			return remove_child( child->get_identifier_id( ) );
		}

		void remove_child( const string_type &id )
		{
			return remove_child( std::hash<string_type>( )( id ) );
		}

		concurrency::event<void( component_event& ), codec> &get_event( const string_type &event )
		{
			auto it = events_.find( event );
			if ( it == events_.end( ) )
				throw;
			return *it->second;
		}

	protected:
		string_type caption_,
			short_caption_,
			text_,
			short_text_,
			identifier_;
		std::uint32_t identifier_id_;
		Vector2i position_,
			size_;
		component<codec> *parent_;
		std::uint32_t state_;
		d3d9_font caption_font_,
			text_font_;
		std::vector<component<codec>*> children_;
		std::shared_ptr<color_palette<codec>> palette_;
		std::unordered_map<string_type, concurrency::event<void( component_event_data<codec>& ), codec>*> events_;
	private:
		concurrency::event<void( component_event& ), _Codec> on_child_added_,
			on_child_removed_;
		static int ID_COUNTER;
	};

	template<typename codec>
	int component<codec>::ID_COUNTER = 1;

}}}