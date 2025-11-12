#pragma once

#include "dr4/math/color.hpp"
#include "dr4/math/vec2.hpp"
#include "dr4/texture.hpp"

#include <SFML/Graphics/Vertex.hpp>
#include <vector>

namespace dr4 {
namespace impl {

class Window;

class Image : public dr4::Image {
  public:
    virtual void
    SetPixel( size_t x, size_t y, dr4::Color color ) override final;
    virtual dr4::Color
    GetPixel( size_t x, size_t y ) const override final;

    virtual void
    SetSize( dr4::Vec2f size ) override final;
    virtual dr4::Vec2f
    GetSize() const override final;
    virtual float
    GetWidth() const override final;
    virtual float
    GetHeight() const override final;

    virtual void
    DrawOn( dr4::Texture& texture ) const override final;
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
    std::vector<sf::Vertex> impl_;

    float w_ = 0;
    float h_ = 0;
    float x_ = 0;
    float y_ = 0;
};

} // namespace impl
} // namespace dr4
