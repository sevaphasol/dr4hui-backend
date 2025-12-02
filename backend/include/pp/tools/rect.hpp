#pragma once

#include "pp/canvas.hpp"
#include "pp/shapes/rect.hpp"
#include "pp/tool.hpp"

namespace pp {
namespace impl {

class RectTool final : public ::pp::Tool {

  public:
    RectTool( ::pp::Canvas* cvs );

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

    virtual bool
    OnKeyDown( const dr4::Event::KeyEvent& evt ) override final;
    virtual bool
    OnKeyUp( const dr4::Event::KeyEvent& evt ) override final;
    virtual bool
    OnText( const dr4::Event::TextEvent& evt ) override final;

  private:
    ::pp::Canvas*   cvs_;
    pp::impl::Rect* rect_;
    bool            is_drawing_;
};

} // namespace impl
} // namespace pp
