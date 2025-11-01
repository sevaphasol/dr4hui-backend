#include "texture.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>

namespace gfx {

void
Texture::SetSize( dr4::Vec2f size )
{
}

dr4::Vec2f
Texture::GetSize() const
{
    auto sf_size = impl_.getSize();

    return dr4::Vec2f( sf_size.x, sf_size.y );
}

float
Texture::GetWidth() const
{
    return GetSize().x;
}

float
Texture::GetHeight() const
{
    return GetSize().y;
}

void
Texture::Draw( const dr4::Rectangle& rect )
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
Texture::Draw( const dr4::Text& text )
{

    sf::Text sf_text;

    sf_text.setCharacterSize( text.fontSize );
    sf_text.setFont( font_ );
    sf_text.setPosition( { text.pos.x, text.pos.y } );
    sf_text.setFillColor( { text.color.r, text.color.g, text.color.b, text.color.a } );
    sf_text.setString( text.text );

    impl_.draw( sf_text );
}

void
Texture::Draw( const dr4::Texture& texture, const dr4::Vec2f& pos )
{
    const Texture& my_texture = dynamic_cast<const Texture&>( texture );

    sf::Sprite sprite;
    sprite.setTexture( my_texture.impl_.getTexture() );
    sprite.setPosition( { pos.x, pos.y } );
    impl_.draw( sprite );
}

} // namespace gfx
