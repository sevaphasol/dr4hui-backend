#include "dr4/texture_impl.hpp"
#include "dr4/font_impl.hpp"
#include "dr4/img_impl.hpp"
#include "dr4/math/color.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transform.hpp>

void
dr4::impl::Texture::SetSize( dr4::Vec2f size )
{
    // fprintf( stderr,
    //  "debug in %s:%d:%s : %f %f\n",
    //  __FILE__,
    //  __LINE__,
    //  __PRETTY_FUNCTION__,
    //  size.x,
    //  size.y );

    impl_.create( static_cast<uint>( size.x ), static_cast<uint>( size.y ) );
    impl_.clear({0,0,0,0});
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
dr4::impl::Texture::Clear(dr4::Color color)
{
	impl_.clear({color.r, color.g, color.b, color.a });
}

void
dr4::impl::Texture::Draw( const dr4::Rectangle& rect )
{
    sf::RectangleShape sf_rect;

    sf_rect.setSize( { rect.rect.size.x - 2 * rect.borderThickness,
                       rect.rect.size.y - 2 * rect.borderThickness } );
    sf_rect.setPosition(
        { rect.rect.pos.x + rect.borderThickness, rect.rect.pos.y + rect.borderThickness } );
    sf_rect.setFillColor( { rect.fill.r, rect.fill.g, rect.fill.b, rect.fill.a } );
    sf_rect.setOutlineColor(
        { rect.borderColor.r, rect.borderColor.g, rect.borderColor.b, rect.borderColor.a } );
    sf_rect.setOutlineThickness( rect.borderThickness );

    // fprintf( stderr, "pos  = %f %f\n", sf_rect.getPosition().x, sf_rect.getPosition().y );
    // fprintf( stderr, "size = %f %f\n", sf_rect.getSize().x, sf_rect.getSize().y );
    // fprintf( stderr,
    //  "fill = %d %d %d\n",
    //  sf_rect.getFillColor().r,
    //  sf_rect.getFillColor().g,
    //  sf_rect.getFillColor().b );
    // fprintf( stderr,
    //  "out  = %d %d %d\n",
    //  sf_rect.getOutlineColor().r,
    //  sf_rect.getOutlineColor().g,
    //  sf_rect.getOutlineColor().b );
    // fprintf( stderr, "thik = %f\n", sf_rect.getOutlineThickness() );

    impl_.draw( sf_rect );
    impl_.display();
}

void
dr4::impl::Texture::Draw( const dr4::Text& text )
{
    sf::Text sf_text;

    const dr4::impl::Font& my_font = dynamic_cast<const dr4::impl::Font&>( *text.font );

    sf_text.setCharacterSize( text.fontSize );
    sf_text.setFont( my_font.impl_ );
    sf_text.setPosition( { text.pos.x, text.pos.y } );
    sf_text.setFillColor( { text.color.r, text.color.g, text.color.b, text.color.a } );
    sf_text.setString( text.text );

    impl_.draw( sf_text );
    impl_.display();
}

void
dr4::impl::Texture::Draw( const dr4::Image& img, const dr4::Vec2f& pos )
{
    const dr4::impl::Image& my_img = dynamic_cast<const dr4::impl::Image&>( img );

    sf::Transform transform = sf::Transform::Identity;

    transform.translate( { pos.x, pos.y } );
    // fprintf( stderr, "debug in %s:%d:%s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__ );
    impl_.draw( my_img.impl_.data(), my_img.impl_.size(), sf::PrimitiveType::Points );
    // fprintf( stderr, "debug in %s:%d:%s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__ );
    impl_.display();
}

void
dr4::impl::Texture::Draw( const dr4::Texture& texture, const dr4::Vec2f& pos )
{
    const dr4::impl::Texture& my_texture = dynamic_cast<const dr4::impl::Texture&>( texture );

    sf::Sprite sprite( my_texture.impl_.getTexture() );
    sprite.setPosition( { pos.x, pos.y } );
    impl_.draw( sprite );
    impl_.display();
}
