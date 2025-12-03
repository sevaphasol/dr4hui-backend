#pragma once

#include "pp/canvas.hpp"
#include "pp/shapes/text.hpp"
#include "pp/tool.hpp"

namespace pp {
namespace impl {

class TextTool final : public pp::Tool {

  public:
    TextTool( pp::Canvas* cvs );

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
    OnKeyDown( const dr4::Event::KeyEvent& evt ) override;
    virtual bool
    OnKeyUp( const dr4::Event::KeyEvent& evt ) override;
    virtual bool
    OnText( const dr4::Event::TextEvent& evt ) override;

  private:
    pp::Canvas*     cvs_        = nullptr;
    pp::impl::Text* text_       = nullptr;
    bool            is_drawing_ = false;
};

} // namespace impl
} // namespace pp
