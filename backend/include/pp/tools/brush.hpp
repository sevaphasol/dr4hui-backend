#pragma once

#include "pp/canvas.hpp"
#include "pp/shapes/brush.hpp"
#include "pp/tool.hpp"

namespace pp {
namespace impl {

class BrushTool final : public pp::Tool {
  public:
    BrushTool( pp::Canvas* cvs );

    std::string_view
    Icon() const override;
    std::string_view
    Name() const override;
    bool
    IsCurrentlyDrawing() const override
    {
        return is_drawing_;
    }

    void
    OnStart() override;
    void
    OnBreak() override;
    void
    OnEnd() override;

    bool
    OnMouseDown( const dr4::Event::MouseButton& evt ) override;
    bool
    OnMouseUp( const dr4::Event::MouseButton& evt ) override;
    bool
    OnMouseMove( const dr4::Event::MouseMove& evt ) override;

  private:
    void
    AddSegment( dr4::Vec2f from, dr4::Vec2f to );

  private:
    pp::Canvas*           cvs_   = nullptr;
    pp::impl::BrushShape* brush_ = nullptr;
    dr4::Vec2f            last_pos_;
    bool                  is_drawing_ = false;
};

} // namespace impl
} // namespace pp
