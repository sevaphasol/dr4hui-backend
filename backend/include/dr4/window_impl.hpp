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
    Window();

    virtual void
    SetTitle( const std::string& title ) override final;
    virtual const std::string&
    GetTitle() const override final;

    virtual Vec2f
    GetSize() const override final;
    virtual void
    SetSize( Vec2f size ) override final;

    virtual void
    Open() override final;
    virtual bool
    IsOpen() const override final;
    virtual void
    Close() override final;

    virtual void
    Clear( Color color ) override final;
    virtual void
    Draw( const Texture& texture ) override final;
    virtual void
    Display() override final;

    virtual Texture*
    CreateTexture() override final;
    virtual Image*
    CreateImage() override final;
    virtual Font*
    CreateFont() override final;
    virtual Line*
    CreateLine() override final;
    virtual Circle*
    CreateCircle() override final;
    virtual Rectangle*
    CreateRectangle() override final;
    virtual Text*
    CreateText() override final;

    void
    SetDefaultFont( Font* font ) override final;

    Font*
    GetDefaultFont() override final;

    virtual void
    StartTextInput() override final
    {
    }

    virtual void
    StopTextInput() override final
    {
    }

    virtual double
    GetTime() override final;
    virtual void
    Sleep( double time ) override final;

    virtual std::optional<Event>
    PollEvent() override final;

  private:
    dr4::Event
    sfmlEventConvert( const sf::Event& sf_event );

    static void
    utf8_encode( uint32_t sym, char buffer[5] );

  private:
    char utf8_buffer_[5];

    double init_time_;

    dr4::Font*       font_;
    dr4::Vec2f       size_;
    std::string      title_;
    sf::RenderWindow impl_;
};

} // namespace impl
} // namespace dr4
