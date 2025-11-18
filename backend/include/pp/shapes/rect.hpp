#pragma once

#include <memory>

#include "dr4/math/vec2.hpp"
#include "dr4/texture.hpp"
#include "dr4/window.hpp"

#include "pp/canvas.hpp"
#include "pp/shape.hpp"

namespace pp {
namespace impl {

enum Side {
    UNKNOWN = -1,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
};

inline static constexpr float  OutlineThickness    = 5;
inline static const dr4::Vec2f OutlineThicknessVec = { 2, 2 };

class Rect final : public ::pp::Shape {

  public:
    Rect( dr4::Window* dr4Window, const ::pp::ControlsTheme& theme, ::pp::State* state );

    virtual bool
    OnMouseDown( const dr4::Event::MouseButton& evt ) override;
    virtual bool
    OnMouseUp( const dr4::Event::MouseButton& evt ) override;
    virtual bool
    OnMouseMove( const dr4::Event::MouseMove& evt ) override;

    virtual void
    OnSelect() override;
    virtual void
    OnDeselect() override;

    virtual void
    DrawOn( ::dr4::Texture& texture ) const override;

    virtual void
    SetPos( ::dr4::Vec2f pos ) override;

    virtual ::dr4::Vec2f
    GetPos() const override;

    void
    SetRemoved( bool isRemoved );
    bool
    isRemoved();

    void
    Resize( dr4::Vec2f offset );

  private:
    std::unique_ptr<dr4::Rectangle> rect_;
    std::unique_ptr<dr4::Rectangle> selectRect_;

    ::pp::State* const state_;

    bool isResized_;
    Side activeSide_;

    bool isDragged_;

    bool isRemoved_;

  private:
    bool
    OnMe( dr4::Vec2f relCoord ) const;
    bool
    OnOutline( dr4::Vec2f relCoord ) const;
    Side
    ClosestSide( dr4::Vec2f relCoord ) const;

    void
    UpdateSelectRect();
};

} // namespace impl
} // namespace pp
