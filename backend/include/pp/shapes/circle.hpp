#pragma once

#include <memory>

#include "dr4/math/vec2.hpp"
#include "dr4/texture.hpp"
#include "dr4/window.hpp"

#include "pp/canvas.hpp"
#include "pp/shape.hpp"

namespace pp {
namespace impl {

class Circle final : public ::pp::Shape {
    enum ResizeCircle {
        Lft   = 0,
        Top   = 1,
        Rht   = 2,
        Bot   = 3,
        Count = 4,
    };

  public:
    Circle( dr4::Window* window, const pp::ControlsTheme& theme, pp::Canvas* cvs );

    bool
    OnMouseDown( const dr4::Event::MouseButton& evt ) override final;
    bool
    OnMouseUp( const dr4::Event::MouseButton& evt ) override final;
    bool
    OnMouseMove( const dr4::Event::MouseMove& evt ) override final;
    virtual bool
    OnKeyDown( const dr4::Event::KeyEvent& evt ) override final;
    virtual bool
    OnKeyUp( const dr4::Event::KeyEvent& evt ) override final;

    void
    OnSelect() override final;
    void
    OnDeselect() override final;

    void
    DrawOn( dr4::Texture& texture ) const override final;

    void
    SetPos( dr4::Vec2f pos ) override final;
    dr4::Vec2f
    GetPos() const override final;

    void
    SetCenter( dr4::Vec2f pos );
    dr4::Vec2f
    GetCenter() const;

    void
    SetSize( dr4::Vec2f size );
    dr4::Vec2f
    GetSize() const;

  private:
    void
    resize( dr4::Vec2f offset );

    bool
    onMe( dr4::Vec2f rel ) const;

    void
    setResizeCircles();
    ResizeCircle
    getResizeCircle( dr4::Vec2f pos ) const;
    void
    drawResizeCircles( dr4::Texture& tex ) const;

  private:
    static constexpr float CircleBorderThickness = 2.0f;
    static constexpr float ResizeCircleRadius    = 5.0f;

    std::unique_ptr<dr4::Circle> circle_;

    std::unique_ptr<dr4::Circle> rsz_circles_[ResizeCircle::Count];

    ResizeCircle active_rsz_circle_;

    pp::Canvas* cvs_;

    bool is_resized_ = false;
    bool is_dragged_ = false;
};

} // namespace impl
} // namespace pp
