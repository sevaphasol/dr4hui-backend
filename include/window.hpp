#pragma once

#include "dr4/event.hpp"
#include "dr4/math/vec2.hpp"
#include "dr4/texture.hpp"
#include "dr4/window.hpp"
#include "texture.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <memory>
#include <optional>

namespace gfx {

class Window : public dr4::Window {
  public:
    ~Window() = default;

    virtual void
    SetTitle( const std::string& title ) override;

    virtual const std::string&
    GetTitle() const override;

    virtual void
    SetSize( const dr4::Vec2f& size ) override;

    virtual dr4::Vec2f
    GetSize() const override;

    virtual void
    Open() override;

    virtual bool
    IsOpen() const override;

    virtual void
    Close() override;

    virtual void
    Clear( const dr4::Color& color ) override;

    virtual void
    Draw( const dr4::Texture& texture, dr4::Vec2f pos ) override;

    virtual void
    Display() override;

    virtual dr4::Texture*
    CreateTexture() override;

    virtual std::optional<dr4::Event>
    PollEvent() override;

  private:
    static dr4::Event
    sfmlEventConvert( const sf::Event& sf_event );

  private:
    dr4::Vec2f       size_;
    std::string      title_;
    sf::RenderWindow impl_;
};

} // namespace gfx
