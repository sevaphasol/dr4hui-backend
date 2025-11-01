#pragma once

#include "dr4/math/vec2.hpp"
#include "dr4/texture.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>

namespace gfx {

class Window;

class Texture : public dr4::Texture {
    friend Window;

  public:
    ~Texture() = default;

    virtual void
    SetSize( dr4::Vec2f size ) override;

    virtual dr4::Vec2f
    GetSize() const override;

    virtual float
    GetWidth() const override;

    virtual float
    GetHeight() const override;

    virtual void
    Draw( const dr4::Rectangle& rect ) override;

    virtual void
    Draw( const dr4::Text& text ) override;

    virtual void
    Draw( const dr4::Texture& texture, const dr4::Vec2f& pos ) override;

  private:
    sf::Font          font_;
    sf::RenderTexture impl_;
};

} // namespace gfx
