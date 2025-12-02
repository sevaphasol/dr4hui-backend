#pragma once

#include "pp/canvas.hpp"
#include "pp/shapes/circle.hpp"
#include "pp/tool.hpp"

namespace pp {
namespace impl {

class CircleTool final : public pp::Tool {

  public:
    CircleTool( ::pp::Canvas* cvs );

    virtual std::string_view
    Icon() const override;
    virtual std::string_view
    Name() const override;
    virtual bool
    IsCurrentlyDrawing() const override;

    virtual void
    OnStart() override;
    virtual void
    OnBreak() override;
    virtual void
    OnEnd() override;

    virtual bool
    OnMouseDown( const dr4::Event::MouseButton& evt ) override;
    virtual bool
    OnMouseUp( const dr4::Event::MouseButton& evt ) override;
    virtual bool
    OnMouseMove( const dr4::Event::MouseMove& evt ) override;

  private:
    pp::Canvas*       cvs_;
    pp::impl::Circle* circle_;
    bool              is_drawing_;
};

} // namespace impl
} // namespace pp
