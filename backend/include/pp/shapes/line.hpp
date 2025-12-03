#pragma once

#include <memory>

#include "dr4/math/vec2.hpp"
#include "dr4/texture.hpp"
#include "dr4/window.hpp"

#include "pp/canvas.hpp"
#include "pp/shape.hpp"

namespace pp {
namespace impl {

class Line final : public ::pp::Shape {
    enum ResizeCircle {
        Start = 0,
        End   = 1,
        Count = 2,
    };

  public:
    Line( dr4::Window* window, const pp::ControlsTheme& theme, pp::Canvas* cvs );

    bool
    OnMouseDown( const dr4::Event::MouseButton& evt ) override final;
    bool
    OnMouseUp( const dr4::Event::MouseButton& evt ) override final;
    bool
    OnMouseMove( const dr4::Event::MouseMove& evt ) override final;

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
    SetStart( dr4::Vec2f pos );
    dr4::Vec2f
    GetStart() const;
    void
    SetEnd( dr4::Vec2f pos );
    dr4::Vec2f
    GetEnd() const;

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
    static constexpr float LineThickness      = 2.0f;
    static constexpr float ResizeCircleRadius = 5.0f;

    std::unique_ptr<dr4::Line> line_;

    std::unique_ptr<dr4::Circle> rsz_circles_[ResizeCircle::Count];

    ResizeCircle active_rsz_circle_;

    pp::Canvas* cvs_ = nullptr;

    bool is_resized_ = false;
    bool is_dragged_ = false;
};

} // namespace impl
} // namespace pp
