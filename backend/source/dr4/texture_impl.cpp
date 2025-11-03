#include "dr4/texture_impl.hpp"
#include "dr4/img_impl.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transform.hpp>

void
dr4::impl::Texture::SetSize( dr4::Vec2f size )
{
}

dr4::Vec2f
dr4::impl::Texture::GetSize() const
{
    auto sf_size = impl_.getSize();

    return dr4::Vec2f( sf_size.x, sf_size.y );
}

float
dr4::impl::Texture::GetWidth() const
{
    return GetSize().x;
}

float
dr4::impl::Texture::GetHeight() const
{
    return GetSize().y;
}

void
dr4::impl::Texture::Draw( const dr4::Rectangle& rect )
{
    sf::RectangleShape sf_rect;

    sf_rect.setSize( { rect.rect.size.x, rect.rect.size.y } );
    sf_rect.setPosition( { rect.rect.pos.x, rect.rect.pos.y } );
    sf_rect.setFillColor( { rect.fill.r, rect.fill.g, rect.fill.b, rect.fill.a } );
    sf_rect.setOutlineColor(
        { rect.borderColor.r, rect.borderColor.g, rect.borderColor.b, rect.borderColor.a } );
    sf_rect.setOutlineThickness( rect.borderThickness );

    impl_.draw( sf_rect );
}

void
dr4::impl::Texture::Draw( const dr4::Text& text )
{
    sf::Text sf_text;

    sf_text.setCharacterSize( text.fontSize );
    // sf_text.setFont( font_ );
    sf_text.setPosition( { text.pos.x, text.pos.y } );
    sf_text.setFillColor( { text.color.r, text.color.g, text.color.b, text.color.a } );
    sf_text.setString( text.text );

    impl_.draw( sf_text );
}

void
dr4::impl::Texture::Draw( const dr4::Image& img, const dr4::Vec2f& pos )
{
    const dr4::impl::Image& my_img = dynamic_cast<const dr4::impl::Image&>( img );

    sf::Transform transform = sf::Transform::Identity;

    transform.translate( { pos.x, pos.y } );

    impl_.draw( my_img.impl_, transform );
}

void
dr4::impl::Texture::Draw( const dr4::Texture& texture, const dr4::Vec2f& pos )
{
    const dr4::impl::Texture& my_texture = dynamic_cast<const dr4::impl::Texture&>( texture );

    sf::Sprite sprite;
    sprite.setTexture( my_texture.impl_.getTexture() );
    sprite.setPosition( { pos.x, pos.y } );
    impl_.draw( sprite );
}
