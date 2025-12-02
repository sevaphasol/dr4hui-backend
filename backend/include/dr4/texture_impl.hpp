#pragma once

#include "dr4/math/vec2.hpp"
#include "dr4/texture.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

namespace dr4 {
namespace impl {

class Texture : public dr4::Texture {
  public:
    void
    SetSize( dr4::Vec2f size ) override final;
    dr4::Vec2f
    GetSize() const override final;

    float
    GetWidth() const override final;
    float
    GetHeight() const override final;

    void
    Clear( dr4::Color color ) override final;

    virtual void
    DrawOn( dr4::Texture& texture ) const override final;
    virtual void
    SetPos( dr4::Vec2f pos ) override final;
    virtual dr4::Vec2f
    GetPos() const override final;

    virtual void
    SetZero( Vec2f pos ) override final;
    virtual Vec2f
    GetZero() const override final;

    virtual void
    SetClipRect( Rect2f rect ) override final;
    virtual void
    RemoveClipRect() override final;
    virtual Rect2f
    GetClipRect() const override final;

    auto&
    GetImpl() const
    {
        return impl_;
    }

  private:
    mutable sf::RenderTexture impl_;
    dr4::Vec2f                pos_;
    dr4::Vec2f                zero_ = { 0.0f, 0.0f };
};

} // namespace impl
} // namespace dr4
