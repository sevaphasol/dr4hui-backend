#pragma once

#include "dr4/math/vec2.hpp"
#include "dr4/texture.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

namespace dr4 {
namespace impl {

class Window;

class Texture : public dr4::Texture {
    friend dr4::impl::Window;

  public:
    ~Texture() = default;

    void
    SetSize( dr4::Vec2f size ) override final;
    dr4::Vec2f
    GetSize() const override final;

    float
    GetWidth() const override final;
    float
    GetHeight() const override final;

    void
    Draw( const dr4::Rectangle& rect ) override final;
    void
    Draw( const dr4::Text& text ) override final;
    virtual void
    Draw( const Image& img, const Vec2f& pos ) override final;
    void
    Draw( const dr4::Texture& texture, const dr4::Vec2f& pos ) override final;

  private:
    sf::RenderTexture impl_;
};

} // namespace impl
} // namespace dr4
