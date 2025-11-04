#pragma once

#include "dr4/math/color.hpp"
#include "dr4/math/vec2.hpp"
#include "dr4/texture.hpp"
#include "dr4/texture_impl.hpp"

#include <SFML/Graphics/VertexArray.hpp>

namespace dr4 {
namespace impl {

class Window;

class Image : public dr4::Image {
    friend dr4::impl::Texture;

  public:
    virtual ~Image() = default;

    virtual void
    SetPixel( unsigned x, unsigned y, dr4::Color color ) override final;
    virtual dr4::Color
    GetPixel( unsigned x, unsigned y ) const override final;

    virtual void
    SetSize( dr4::Vec2f size ) override final;
    virtual dr4::Vec2f
    GetSize() const override final;
    virtual float
    GetWidth() const override final;
    virtual float
    GetHeight() const override final;

  private:
    sf::VertexArray impl_;

    float w_ = 0;
    float h_ = 0;
};

} // namespace impl
} // namespace dr4
