#include "dr4/window_impl.hpp"
#include "dr4/img_impl.hpp"
#include "dr4/texture.hpp"
#include "dr4/texture_impl.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>

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
dr4::impl::Window::SetSize( const dr4::Vec2f& size )
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
dr4::impl::Window::Clear( const dr4::Color& color )
{
    impl_.clear( sf::Color( color.r, color.g, color.b, color.a ) );
}

void
dr4::impl::Window::Draw( const dr4::Texture& texture, dr4::Vec2f pos )
{
    const dr4::impl::Texture& my_texture = dynamic_cast<const dr4::impl::Texture&>( texture );

    sf::Sprite sprite;
    sprite.setTexture( my_texture.impl_.getTexture() );
    sprite.setPosition( { pos.x, pos.y } );
    impl_.draw( sprite );
}

void
dr4::impl::Window::Display()
{
    impl_.display();
}

dr4::Image*
dr4::impl::Window::CreateImage()
{
    return new dr4::impl::Image();
}

dr4::Texture*
dr4::impl::Window::CreateTexture()
{
    return new dr4::impl::Texture();
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
        // case sf::Event::TextEntered:
        // event.type         = Event::TextEntered;
        // event.text.unicode = sf_event.text.unicode;
        // break;
        case sf::Event::KeyPressed:
            event.type = dr4::Event::Type::KEY_DOWN;
            // event.key.code = detail::fromSFML( sf_event.key.code );
            break;
        case sf::Event::KeyReleased:
            event.type = dr4::Event::Type::KEY_UP;
            // event.key.code = detail::fromSFML( sf_event.key.code );
            break;
        case sf::Event::MouseButtonPressed:
            event.type = dr4::Event::Type::MOUSE_DOWN;
            // event.mouse_button.button = detail::fromSFML( sf_event.mouseButton.button );
            // event.mouse_button.x      = sf_event.mouseButton.x;
            // event.mouse_button.y      = sf_event.mouseButton.y;
            break;
        case sf::Event::MouseButtonReleased:
            event.type = dr4::Event::Type::MOUSE_UP;
            // event.mouse_button.button = detail::fromSFML( sf_event.mouseButton.button );
            // event.mouse_button.x      = sf_event.mouseButton.x;
            // event.mouse_button.y      = sf_event.mouseButton.y;
            break;
        case sf::Event::MouseMoved:
            event.type = dr4::Event::Type::MOUSE_MOVE;
            // event.mouse_move.x = sf_event.mouseMove.x;
            // event.mouse_move.y = sf_event.mouseMove.y;
            break;
        default:
            event.type = dr4::Event::Type::UNKNOWN;
            break;
    }

    return event;
}
