#include "dr4/window_impl.hpp"
#include "dr4/converts.hpp"
#include "dr4/event.hpp"
#include "dr4/font_impl.hpp"
#include "dr4/img_impl.hpp"
#include "dr4/primitives_impl.hpp"
#include "dr4/text_impl.hpp"
#include "dr4/texture.hpp"
#include "dr4/texture_impl.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>
#include <chrono>
#include <iostream>
#include <thread>

dr4::impl::Window::Window()
{
    auto current_time        = std::chrono::system_clock::now();
    auto duration_in_seconds = std::chrono::duration<double>( current_time.time_since_epoch() );

    init_time_ = duration_in_seconds.count();
}

void
dr4::impl::Window::SetTitle( const std::string& title )
{
    title_ = title;
    impl_.setTitle( title_ );
}

const std::string&
dr4::impl::Window::GetTitle() const
{
    return title_;
}

void
dr4::impl::Window::SetSize( dr4::Vec2f size )
{
    size_ = size;
    impl_.setSize( { static_cast<uint>( size.x ), static_cast<uint>( size.y ) } );
}

dr4::Vec2f
dr4::impl::Window::GetSize() const
{
    return dr4::Vec2f( size_.x, size_.y );
}

void
dr4::impl::Window::Open()
{
    impl_.create( sf::VideoMode( size_.x, size_.y ), title_ );
    impl_.setFramerateLimit( 60 );
}

bool
dr4::impl::Window::IsOpen() const
{
    return impl_.isOpen();
}

void
dr4::impl::Window::Close()
{
    impl_.close();
}

void
dr4::impl::Window::Clear( dr4::Color color )
{
    impl_.clear( sf::Color( color.r, color.g, color.b, color.a ) );
}

void
dr4::impl::Window::Draw( const dr4::Texture& texture )
{
    const dr4::impl::Texture& my_texture = dynamic_cast<const dr4::impl::Texture&>( texture );

    auto& sf_texture = my_texture.GetImpl();

    sf_texture.display();
    sf::Sprite sprite( sf_texture.getTexture() );
    impl_.draw( sprite );
}

void
dr4::impl::Window::Display()
{
    impl_.display();
}

double
dr4::impl::Window::GetTime()
{
    auto current_time        = std::chrono::system_clock::now();
    auto duration_in_seconds = std::chrono::duration<double>( current_time.time_since_epoch() );

    return duration_in_seconds.count() - init_time_;
}

void
dr4::impl::Window::Sleep( double time )
{
    std::this_thread::sleep_for( std::chrono::duration<double>( time ) );
}

dr4::Texture*
dr4::impl::Window::CreateTexture()
{
    return new dr4::impl::Texture();
}

dr4::Image*
dr4::impl::Window::CreateImage()
{
    return new dr4::impl::Image();
}

dr4::Font*
dr4::impl::Window::CreateFont()
{
    return new dr4::impl::Font();
}

dr4::Line*
dr4::impl::Window::CreateLine()
{
    return new dr4::impl::Line();
}

dr4::Circle*
dr4::impl::Window::CreateCircle()
{
    return new dr4::impl::Circle();
}

void
dr4::impl::Window::SetDefaultFont( dr4::Font* font )
{
    font_ = font;
}

dr4::Font*
dr4::impl::Window::GetDefaultFont()
{
    return font_;
}

dr4::Rectangle*
dr4::impl::Window::CreateRectangle()
{
    return new dr4::impl::Rectangle();
}

dr4::Text*
dr4::impl::Window::CreateText()
{
    return new dr4::impl::Text();
}

std::optional<dr4::Event>
dr4::impl::Window::PollEvent()
{
    sf::Event sf_event;

    if ( !impl_.pollEvent( sf_event ) )
    {
        return std::nullopt;
    }

    return sfmlEventConvert( sf_event );
}

dr4::Event
dr4::impl::Window::sfmlEventConvert( const sf::Event& sf_event )
{
    dr4::Event event = {};

    switch ( sf_event.type )
    {
        case sf::Event::Closed:
            event.type = dr4::Event::Type::QUIT;
            break;
        case sf::Event::TextEntered:
            event.type = dr4::Event::Type::TEXT_EVENT;
            utf8_encode( sf_event.text.unicode, utf8_buffer_ );
            event.text.unicode = utf8_buffer_;
            break;
        case sf::Event::KeyPressed:
            event.type     = dr4::Event::Type::KEY_DOWN;
            event.key.sym  = dr4::impl::detail::fromSFML( sf_event.key.code );
            event.key.mods = dr4::impl::detail::fromSFML( sf_event.key );
            break;
        case sf::Event::KeyReleased:
            event.type     = dr4::Event::Type::KEY_UP;
            event.key.sym  = dr4::impl::detail::fromSFML( sf_event.key.code );
            event.key.mods = dr4::impl::detail::fromSFML( sf_event.key );
            break;
        case sf::Event::MouseButtonPressed:
            event.type               = dr4::Event::Type::MOUSE_DOWN;
            event.mouseButton.button = dr4::impl::detail::fromSFML( sf_event.mouseButton.button );
            event.mouseButton.pos.x  = sf_event.mouseButton.x;
            event.mouseButton.pos.y  = sf_event.mouseButton.y;
            break;
        case sf::Event::MouseButtonReleased:
            event.type               = dr4::Event::Type::MOUSE_UP;
            event.mouseButton.button = dr4::impl::detail::fromSFML( sf_event.mouseButton.button );
            event.mouseButton.pos.x  = sf_event.mouseButton.x;
            event.mouseButton.pos.y  = sf_event.mouseButton.y;
            break;
        case sf::Event::MouseMoved:
            static ::dr4::Vec2f prev_mouse_pos = { 0, 0 };

            event.type      = ::dr4::Event::Type::MOUSE_MOVE;
            event.mouseMove = {
                .pos = { static_cast<float>( sf_event.mouseMove.x ),
                         static_cast<float>( sf_event.mouseMove.y ) },
                .rel = { static_cast<float>( sf_event.mouseMove.x - prev_mouse_pos.x ),
                         static_cast<float>( sf_event.mouseMove.y - prev_mouse_pos.y ) } };

            prev_mouse_pos = event.mouseMove.pos;
            break;
        default:
            event.type = dr4::Event::Type::UNKNOWN;
            break;
    }

    return event;
}

void
dr4::impl::Window::utf8_encode( uint32_t sym, char buffer[5] )
{
    if ( sym <= 0x7F )
    {
        buffer[0] = static_cast<char>( sym );
        buffer[1] = '\0';
    } else if ( sym <= 0x7FF )
    {
        buffer[0] = static_cast<char>( 0xC0 | ( ( sym >> 6 ) & 0x1F ) );
        buffer[1] = static_cast<char>( 0x80 | ( sym & 0x3F ) );
        buffer[2] = '\0';
    } else if ( sym <= 0xFFFF )
    {
        buffer[0] = static_cast<char>( 0xE0 | ( ( sym >> 12 ) & 0x0F ) );
        buffer[1] = static_cast<char>( 0x80 | ( ( sym >> 6 ) & 0x3F ) );
        buffer[2] = static_cast<char>( 0x80 | ( sym & 0x3F ) );
        buffer[3] = '\0';
    } else
    {
        buffer[0] = static_cast<char>( 0xF0 | ( ( sym >> 18 ) & 0x07 ) );
        buffer[1] = static_cast<char>( 0x80 | ( ( sym >> 12 ) & 0x3F ) );
        buffer[2] = static_cast<char>( 0x80 | ( ( sym >> 6 ) & 0x3F ) );
        buffer[3] = static_cast<char>( 0x80 | ( sym & 0x3F ) );
        buffer[4] = '\0';
    }
}
