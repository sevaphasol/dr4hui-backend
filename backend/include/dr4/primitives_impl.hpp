#pragma once

#include "dr4/texture.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

namespace dr4 {
namespace impl {

class Line : public dr4::Line {
  public:
    virtual void
    SetStart( Vec2f start ) override final;
    virtual void
    SetEnd( Vec2f end ) override final;
    virtual void
    SetColor( Color color ) override final;
    virtual void
    SetThickness( float thickness ) override final;

    virtual Vec2f
    GetStart() const override final;
    virtual Vec2f
    GetEnd() const override final;
    virtual Color
    GetColor() const override final;
    virtual float
    GetThickness() const override final;

    virtual void
    DrawOn( Texture& texture ) const override final;
    virtual void
    SetPos( Vec2f pos ) override final;
    virtual Vec2f
    GetPos() const override final;

    auto&
    GetImpl()
    {
        return impl_;
    }

    const auto&
    GetImpl() const
    {
        return impl_;
    }

  private:
    void
    update() const;

  private:
    mutable bool dirty_;

    mutable sf::VertexArray impl_;

    sf::Color sf_color_;

    float thickness_ = 1.0f;

    Vec2f start_ = { 0.0f, 0.0f };
    Vec2f end_   = { 0.0f, 1.0f };
};

class Circle : public dr4::Circle {

  public:
    virtual void
    SetCenter( Vec2f center ) override final;
    virtual void
    SetRadius( float radius ) override final;
    virtual void
    SetFillColor( Color color ) override final;
    virtual void
    SetBorderColor( Color color ) override final;
    virtual void
    SetBorderThickness( float thickness ) override final;

    virtual Vec2f
    GetCenter() const override final;
    virtual float
    GetRadius() const override final;
    virtual Color
    GetFillColor() const override final;
    virtual Color
    GetBorderColor() const override final;
    virtual float
    GetBorderThickness() const override final;

    virtual void
    DrawOn( Texture& texture ) const override final;
    virtual void
    SetPos( Vec2f pos ) override final;
    virtual Vec2f
    GetPos() const override final;

    auto&
    GetImpl()
    {
        return impl_;
    }

    const auto&
    GetImpl() const
    {
        return impl_;
    }

  private:
    sf::CircleShape impl_;
};

class Rectangle : public dr4::Rectangle {

  public:
    virtual ~Rectangle() = default;

    virtual void
    SetSize( Vec2f size ) override final;
    virtual void
    SetFillColor( Color color ) override final;
    virtual void
    SetBorderThickness( float thickness ) override final;
    virtual void
    SetBorderColor( Color color ) override final;

    virtual Vec2f
    GetSize() const override final;
    virtual Color
    GetFillColor() const override final;
    virtual float
    GetBorderThickness() const override final;
    virtual Color
    GetBorderColor() const override final;

    virtual void
    DrawOn( Texture& texture ) const override final;
    virtual void
    SetPos( Vec2f pos ) override final;
    virtual Vec2f
    GetPos() const override final;

    auto&
    GetImpl()
    {
        return impl_;
    }

    const auto&
    GetImpl() const
    {
        return impl_;
    }

  private:
    sf::RectangleShape impl_;
};

} // namespace impl
} // namespace dr4
