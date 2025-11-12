#pragma once

#include "dr4/font_impl.hpp"
#include "dr4/math/vec2.hpp"
#include "dr4/texture.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

namespace dr4 {
namespace impl {

class Text : public dr4::Text {
  public:
    virtual void
    SetText( const std::string& text ) override final;
    virtual void
    SetColor( Color color ) override final;
    virtual void
    SetFontSize( float size ) override final;
    virtual void
    SetVAlign( dr4::Text::VAlign align ) override final;
    virtual void
    SetFont( const dr4::Font* font ) override final;

    virtual Vec2f
    GetBounds() const override final;
    virtual const std::string&
    GetText() const override final;
    virtual Color
    GetColor() const override final;
    virtual float
    GetFontSize() const override final;
    virtual dr4::Text::VAlign
    GetVAlign() const override final;
    virtual const dr4::Font&
    GetFont() const override final;

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
    sf::Vector2f    pos_;
    std::string     string_;
    dr4::impl::Font font_;
    sf::Text        impl_;
};

} // namespace impl
} // namespace dr4
