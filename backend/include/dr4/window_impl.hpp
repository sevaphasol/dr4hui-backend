#pragma once

#include "dr4/event.hpp"
#include "dr4/math/vec2.hpp"
#include "dr4/texture.hpp"
#include "dr4/window.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace dr4 {
namespace impl {

class Window : public dr4::Window {
  public:
    ~Window() = default;

    void
    SetTitle( const std::string& title ) override final;
    const std::string&
    GetTitle() const override final;

    void
    SetSize( const dr4::Vec2f& size ) override final;
    dr4::Vec2f
    GetSize() const override final;

    void
    Open() override final;
    bool
    IsOpen() const override final;
    void
    Close() override final;

    void
    Clear( const dr4::Color& color ) override final;
    void
    Draw( const dr4::Texture& texture, dr4::Vec2f pos ) override final;
    void
    Display() override final;

    dr4::Image*
    CreateImage() override final;

    dr4::Texture*
    CreateTexture() override final;

    std::optional<dr4::Event>
    PollEvent() override final;

  private:
    static dr4::Event
    sfmlEventConvert( const sf::Event& sf_event );

  private:
    dr4::Vec2f       size_;
    std::string      title_;
    sf::RenderWindow impl_;
};

} // namespace impl
} // namespace dr4
