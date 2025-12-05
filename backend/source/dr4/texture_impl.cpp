#include "dr4/texture_impl.hpp"
#include "dr4/math/color.hpp"
#include "dr4/texture.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <iostream>

void
dr4::impl::Texture::SetSize( dr4::Vec2f size )
{
    impl_.create( static_cast<uint>( size.x ), static_cast<uint>( size.y ) );
    impl_.clear( { 0, 0, 0, 0 } );
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
    return impl_.getSize().x;
}

float
dr4::impl::Texture::GetHeight() const
{
    return impl_.getSize().y;
}

void
dr4::impl::Texture::Clear( dr4::Color color )
{
    impl_.clear( { color.r, color.g, color.b, color.a } );
}

void
dr4::impl::Texture::SetZero( Vec2f pos )
{
    zero_ = pos;
}

dr4::Vec2f
dr4::impl::Texture::GetZero() const
{
    return zero_;
}

void
dr4::impl::Texture::SetPos( Vec2f pos )
{
    pos_ = pos;
}

dr4::Vec2f
dr4::impl::Texture::GetPos() const
{
    return pos_;
}

void
dr4::impl::Texture::SetClipRect( Rect2f rect )
{
    std::cerr << "Sorry, " << __func__ << "unimplemented" << std::endl;
}

void
dr4::impl::Texture::RemoveClipRect()
{
    std::cerr << "Sorry, " << __func__ << "unimplemented" << std::endl;
}

dr4::Rect2f
dr4::impl::Texture::GetClipRect() const
{
    std::cerr << "Sorry, " << __func__ << "unimplemented" << std::endl;

    return { 0, 0, 0, 0 };
}

void
dr4::impl::Texture::DrawOn( dr4::Texture& texture ) const
{
    auto& my_texture = dynamic_cast<dr4::impl::Texture&>( texture );

    impl_.display();

    sf::Sprite sprite;

    if ( &my_texture == this )
    {
        sf::Texture copy = impl_.getTexture();
        sprite.setTexture( copy );
    } else
    {
        sprite.setTexture( impl_.getTexture() );
    }

    sf::Transform sf_transform;

    auto tex_zero = my_texture.GetZero();
    sprite.setPosition( pos_.x + tex_zero.x, pos_.y + tex_zero.y );

    my_texture.GetImpl().draw( sprite );
}

dr4::Image*
dr4::impl::Texture::GetImage() const
{
    std::cerr << "Sorry, " << __func__ << "unimplemented" << std::endl;

    return nullptr;
}
