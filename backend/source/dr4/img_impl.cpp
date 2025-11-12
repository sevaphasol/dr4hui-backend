#include "dr4/img_impl.hpp"
#include "dr4/math/color.hpp"
#include "dr4/texture_impl.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

void
dr4::impl::Image::SetPixel( size_t x, size_t y, dr4::Color color )
{
    impl_[y * w_ + x].color      = { color.r, color.g, color.b, color.a };
    impl_[y * w_ + x].position.x = x;
    impl_[y * w_ + x].position.y = y;
}

dr4::Color
dr4::impl::Image::GetPixel( size_t x, size_t y ) const
{
    const auto& v = impl_[y * w_ + x];

    return { v.color.r, v.color.g, v.color.b, v.color.a };
}

void
dr4::impl::Image::SetSize( dr4::Vec2f size )
{
    w_ = size.x;
    h_ = size.y;

    impl_.resize( size.x * size.y );
}

dr4::Vec2f
dr4::impl::Image::GetSize() const
{
    return { w_, h_ };
}

float
dr4::impl::Image::GetWidth() const
{
    return w_;
}

float
dr4::impl::Image::GetHeight() const
{
    return h_;
}

void
dr4::impl::Image::DrawOn( dr4::Texture& texture ) const
{
    auto& my_texture = dynamic_cast<dr4::impl::Texture&>( texture );

    sf::Transform transform = sf::Transform::Identity;

    auto tex_zero = my_texture.GetZero();

    transform.translate( { x_ + tex_zero.x, y_ + tex_zero.y } );

    my_texture.GetImpl().draw( impl_.data(), impl_.size(), sf::PrimitiveType::Points, transform );
}

void
dr4::impl::Image::SetPos( Vec2f pos )
{
    x_ = pos.x;
    y_ = pos.y;
}

dr4::Vec2f
dr4::impl::Image::GetPos() const
{
    return { x_, y_ };
}
