#include "dr4/math/vec2.hpp"
#include "dr4/primitives_impl.hpp"
#include "dr4/texture_impl.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>
#include <memory>

void
dr4::impl::Line::SetStart( dr4::Vec2f start )
{
    dirty_ = true;

    start_ = start;
}

void
dr4::impl::Line::SetEnd( dr4::Vec2f end )
{
    dirty_ = true;

    end_ = end;
}

void
dr4::impl::Line::SetColor( dr4::Color color )
{
    dirty_ = true;

    sf_color_ = { color.r, color.g, color.b, color.a };
}

void
dr4::impl::Line::SetThickness( float thickness )
{
    dirty_ = true;

    thickness_ = thickness;
}

dr4::Vec2f
dr4::impl::Line::GetStart() const
{
    return start_;
}

dr4::Vec2f
dr4::impl::Line::GetEnd() const
{
    return end_;
}

dr4::Color
dr4::impl::Line::GetColor() const
{
    return { sf_color_.r, sf_color_.g, sf_color_.b, sf_color_.a };
}

float
dr4::impl::Line::GetThickness() const
{
    return thickness_;
}

void
dr4::impl::Line::SetPos( dr4::Vec2f pos )
{
    pos_ = pos;
}

dr4::Vec2f
dr4::impl::Line::GetPos() const
{
    return pos_;
}

void
dr4::impl::Line::DrawOn( dr4::Texture& texture ) const
{
    auto& my_texture = dynamic_cast<dr4::impl::Texture&>( texture );

    if ( dirty_ )
    {
        update();
    }

    sf::Transform sf_transform;

    auto tex_zero = my_texture.GetZero();

    sf_transform.translate( { pos_.x + tex_zero.x, pos_.y + tex_zero.y } );

    my_texture.GetImpl().draw( impl_, sf_transform );
}

void
dr4::impl::Line::update() const
{
    impl_.clear();
    impl_.setPrimitiveType( sf::Triangles );
    impl_.resize( 6 );

    const float dx = end_.x - start_.x;
    const float dy = end_.y - start_.y;

    const float inv_len = 1.0f / std::sqrt( dx * dx + dy * dy );

    const float half = thickness_ * 0.5f;

    const float offset_x = -dy * inv_len * half;
    const float offset_y = dx * inv_len * half;

    sf::Vector2f p1( start_.x + offset_x, start_.y + offset_y );
    sf::Vector2f p2( start_.x - offset_x, start_.y - offset_y );
    sf::Vector2f p3( end_.x - offset_x, end_.y - offset_y );
    sf::Vector2f p4( end_.x + offset_x, end_.y + offset_y );

    impl_[0].position = p1;
    impl_[1].position = p2;
    impl_[2].position = p3;

    impl_[3].position = p3;
    impl_[4].position = p4;
    impl_[5].position = p1;

    for ( size_t i = 0; i < 6; ++i )
    {
        impl_[i].color = sf_color_;
    }

    dirty_ = false;
}

void
dr4::impl::Circle::SetCenter( dr4::Vec2f center )
{
    auto  scale  = impl_.getScale();
    float radius = impl_.getRadius();

    float px = center.x - radius * scale.x;
    float py = center.y - radius * scale.y;

    impl_.setPosition( px, py );
}

void
dr4::impl::Circle::SetPos( dr4::Vec2f pos )
{
    impl_.setPosition( { pos.x, pos.y } );
}

void
dr4::impl::Circle::SetRadius( dr4::Vec2f radius )
{
    impl_.setRadius( radius.x );
    impl_.setScale( 1.0, radius.y / radius.x );
    // impl_.setPointCount( 100000 );
}

void
dr4::impl::Circle::SetFillColor( dr4::Color color )
{
    impl_.setFillColor( { color.r, color.g, color.b, color.a } );
}

void
dr4::impl::Circle::SetBorderColor( dr4::Color color )
{
    impl_.setOutlineColor( { color.r, color.g, color.b, color.a } );
}

void
dr4::impl::Circle::SetBorderThickness( float thickness )
{
    if ( thickness > 0 )
    {
        // std::cerr << "Warning: thickness > 0" << std::endl;
    }

    impl_.setOutlineThickness( thickness );
}

dr4::Vec2f
dr4::impl::Circle::GetCenter() const
{
    auto  pos    = impl_.getPosition();
    auto  scale  = impl_.getScale();
    float radius = impl_.getRadius();

    float cx = pos.x + radius * scale.x;
    float cy = pos.y + radius * scale.y;

    return { cx, cy };
}

dr4::Vec2f
dr4::impl::Circle::GetPos() const
{
    auto sf_pos = impl_.getPosition();

    return { sf_pos.x, sf_pos.y };
}

dr4::Vec2f
dr4::impl::Circle::GetRadius() const
{
    float radius   = impl_.getRadius();
    auto  sf_scale = impl_.getScale();

    return { radius, radius * sf_scale.y / sf_scale.x };
}

dr4::Color
dr4::impl::Circle::GetFillColor() const
{
    auto sf_color = impl_.getFillColor();

    return { sf_color.r, sf_color.g, sf_color.b, sf_color.a };
}

dr4::Color
dr4::impl::Circle::GetBorderColor() const
{
    auto sf_color = impl_.getOutlineColor();

    return { sf_color.r, sf_color.g, sf_color.b, sf_color.a };
}

float
dr4::impl::Circle::GetBorderThickness() const
{
    return impl_.getOutlineThickness();
}

void
dr4::impl::Circle::DrawOn( dr4::Texture& texture ) const
{
    auto& my_texture = dynamic_cast<dr4::impl::Texture&>( texture );

    sf::Transform sf_transform;

    auto tex_zero = my_texture.GetZero();

    sf_transform.translate( { tex_zero.x, tex_zero.y } );

    // std::cerr << "Position  " << impl_.getPosition().x << " " << impl_.getPosition().y <<
    // std::endl; std::cerr << "Radius    " << impl_.getRadius() << std::endl; std::cerr << "TexZero
    // " << tex_zero.x << " " << tex_zero.y << std::endl;

    my_texture.GetImpl().draw( impl_, sf_transform );
}

void
dr4::impl::Rectangle::SetPos( dr4::Vec2f pos )
{
    impl_.setPosition( { pos.x, pos.y } );
}

void
dr4::impl::Rectangle::SetSize( dr4::Vec2f size )
{
    impl_.setSize( { size.x, size.y } );
}

void
dr4::impl::Rectangle::SetFillColor( dr4::Color color )
{
    impl_.setFillColor( { color.r, color.g, color.b, color.a } );
}

void
dr4::impl::Rectangle::SetBorderColor( dr4::Color color )
{
    impl_.setOutlineColor( { color.r, color.g, color.b, color.a } );
}

void
dr4::impl::Rectangle::SetBorderThickness( float thickness )
{
    if ( thickness > 0 )
    {
        // std::cerr << "Warning: thickness > 0" << std::endl;
    }

    impl_.setOutlineThickness( thickness );
}

dr4::Vec2f
dr4::impl::Rectangle::GetPos() const
{
    auto sf_position = impl_.getPosition();

    return { sf_position.x, sf_position.y };
}

dr4::Vec2f
dr4::impl::Rectangle::GetSize() const
{
    auto sf_size = impl_.getSize();

    return { sf_size.x, sf_size.y };
}

dr4::Color
dr4::impl::Rectangle::GetFillColor() const
{
    auto sf_color = impl_.getFillColor();

    return { sf_color.r, sf_color.g, sf_color.b, sf_color.a };
}

dr4::Color
dr4::impl::Rectangle::GetBorderColor() const
{
    auto sf_color = impl_.getOutlineColor();

    return { sf_color.r, sf_color.g, sf_color.b, sf_color.a };
}

float
dr4::impl::Rectangle::GetBorderThickness() const
{
    return impl_.getOutlineThickness();
}

void
dr4::impl::Rectangle::DrawOn( dr4::Texture& texture ) const
{
    auto& my_texture = dynamic_cast<dr4::impl::Texture&>( texture );

    sf::Transform sf_transform;

    auto tex_zero = my_texture.GetZero();

    sf_transform.translate( { tex_zero.x, tex_zero.y } );

    my_texture.GetImpl().draw( impl_, sf_transform );
}
