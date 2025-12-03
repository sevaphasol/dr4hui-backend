#pragma once

#include <memory>

#include "dr4/event.hpp"
#include "dr4/math/vec2.hpp"
#include "dr4/texture.hpp"
#include "dr4/window.hpp"

#include "pp/canvas.hpp"
#include "pp/shape.hpp"

namespace pp {
namespace impl {

class Text final : public pp::Shape {
  public:
    Text( dr4::Window* window, const pp::ControlsTheme& theme, pp::Canvas* cvs );

    bool
    OnMouseDown( const dr4::Event::MouseButton& evt ) override final;
    bool
    OnMouseUp( const dr4::Event::MouseButton& evt ) override final;
    bool
    OnMouseMove( const dr4::Event::MouseMove& evt ) override final;
    bool
    OnKeyDown( const dr4::Event::KeyEvent& evt ) override final;

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
    SetIsDrawing( bool state );
    void
    Insert( char c );
    void
    Backspace();
    void
    Delete();
    void
    DecrementCursor();
    void
    IncrementCursor();

  private:
    bool
    onMe( dr4::Vec2f rel ) const;

    void
    updateRect();

    void
    insertCursor( const dr4::Event::MouseButton& evt );
    float
    getCursorX() const;

    std::pair<size_t, size_t>
    getSelectionRange() const;

    bool
    hasSelection() const;
    void
    clearSelection();

  private:
    static constexpr float RectBorderThickness = 2.0f;
    static constexpr float RectMarginX         = 10.0f;
    static constexpr float RectMarginY         = 10.0f;

    std::string         string_;
    mutable std::string tmp_string_;

    std::unique_ptr<dr4::Text>      text_;
    std::unique_ptr<dr4::Rectangle> rect_;

    pp::Canvas* cvs_ = nullptr;

    std::unique_ptr<dr4::Rectangle> cursor_;

    double         cursor_blink_period_    = 0.5;
    mutable double cursor_last_blink_time_ = 0.0;
    mutable bool   cursor_visible_         = false;
    size_t         cursor_pos_             = 0;

    size_t                          selection_pos_    = 0;
    bool                            selection_active_ = false;
    bool                            is_selecting_     = false;
    std::unique_ptr<dr4::Rectangle> selection_rect_;

    bool is_drawing_ = false;
    bool is_dragged_ = false;

    float ascent_;
    float descent_;
};

} // namespace impl
} // namespace pp
