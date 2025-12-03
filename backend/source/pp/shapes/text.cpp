#include <cassert>
#include <cmath>
#include <iostream>

#include "dr4/math/rect.hpp"
#include "dr4/math/vec2.hpp"
#include "pp/shapes/text.hpp"

pp::impl::Text::Text( dr4::Window* window, const ::pp::ControlsTheme& theme, ::pp::Canvas* cvs )
    : rect_( window->CreateRectangle() ),
      text_( window->CreateText() ),
      cursor_( window->CreateRectangle() ),
      selection_rect_( window->CreateRectangle() ),
      cvs_( cvs )
{
    rect_->SetBorderThickness( RectBorderThickness );
    rect_->SetBorderColor( { 118, 185, 0 } );
    rect_->SetFillColor( { 0, 0, 0 } );

    text_->SetFont( window->GetDefaultFont() );
    text_->SetColor( { 255, 255, 255 } );
    text_->SetFontSize( cvs_->GetControlsTheme().baseFontSize );

    cursor_->SetFillColor( { 255, 255, 255 } );

    selection_rect_->SetFillColor( { 50, 100, 200, 100 } );
}

bool
pp::impl::Text::OnMouseDown( const dr4::Event::MouseButton& evt )
{
    if ( evt.button != dr4::MouseButtonType::LEFT )
    {
        return false;
    }

    if ( onMe( evt.pos ) )
    {
        if ( is_drawing_ )
        {
            insertCursor( evt );
        }

        OnSelect();
        is_dragged_ = true;
        return true;
    }

    return false;
}

bool
pp::impl::Text::OnMouseUp( const dr4::Event::MouseButton& evt )
{
    if ( evt.button != dr4::MouseButtonType::LEFT )
    {
        return false;
    }

    if ( is_drawing_ )
    {
    }

    is_dragged_ = false;

    return true;
}

bool
pp::impl::Text::OnMouseMove( const dr4::Event::MouseMove& evt )
{
    if ( is_dragged_ )
    {
        SetPos( GetPos() + evt.rel );
        return true;
    }

    return false;
}

bool
pp::impl::Text::OnKeyDown( const dr4::Event::KeyEvent& evt )
{
    if ( evt.sym == dr4::KEYCODE_BACKSPACE )
    {
        Backspace();
        return true;
    }

    if ( evt.sym == dr4::KEYCODE_DELETE )
    {
        Delete();
        return true;
    }

    if ( evt.sym == dr4::KEYCODE_LEFT )
    {
        DecrementCursor();
        return true;
    }

    if ( evt.sym == dr4::KEYCODE_RIGHT )
    {
        IncrementCursor();
        return true;
    }

    return false;
}

void
pp::impl::Text::OnSelect()
{
    if ( cvs_->GetSelectedShape() != nullptr )
    {
        if ( cvs_->GetSelectedShape() == this )
        {
            return;
        }

        cvs_->GetSelectedShape()->OnDeselect();
    }

    cvs_->SetSelectedShape( this );
}

void
pp::impl::Text::OnDeselect()
{
    is_drawing_ = false;
}

void
pp::impl::Text::SetIsDrawing( bool state )
{
    is_drawing_ = state;
}

void
pp::impl::Text::DrawOn( dr4::Texture& texture ) const
{
    if ( is_drawing_ )
    {
        rect_->DrawOn( texture );
    }

    text_->DrawOn( texture );

    if ( is_drawing_ )
    {
        double now = cvs_->GetWindow()->GetTime();
        if ( now - cursor_last_blink_time_ >= cursor_blink_period_ )
        {
            cursor_visible_         = !cursor_visible_;
            cursor_last_blink_time_ = now;
        }

        if ( cursor_visible_ )
        {
            cursor_->DrawOn( texture );
        }
    }
}

void
pp::impl::Text::SetPos( dr4::Vec2f pos )
{
    text_->SetPos( pos );
    updateRect();
}

dr4::Vec2f
pp::impl::Text::GetPos() const
{
    return text_->GetPos();
}

bool
pp::impl::Text::onMe( dr4::Vec2f rel ) const
{
    dr4::Vec2f rect_size = rect_->GetSize();
    dr4::Vec2f rect_pos  = rect_->GetPos();

    if ( rect_size.x < 0 )
    {
        rect_size.x = -rect_size.x;
        rect_pos.x -= rect_size.x;
    }

    if ( rect_size.y < 0 )
    {
        rect_size.y = -rect_size.y;
        rect_pos.y -= rect_size.y;
    }

    return dr4::Rect2f( rect_pos, rect_size ).Contains( rel );
}

void
pp::impl::Text::updateRect()
{
    dr4::Vec2f baseline = text_->GetPos();

    const dr4::Font* font      = text_->GetFont();
    float            font_size = text_->GetFontSize();

    float raw_ascent  = 1.3 * font->GetAscent( font_size );
    float raw_descent = 1.3 * font->GetDescent( font_size );

    float asc_up  = std::fabs( raw_ascent );
    float desc_dn = std::fabs( raw_descent );

    float line_height = asc_up + desc_dn;

    dr4::Vec2f bounds     = text_->GetBounds();
    float      text_width = bounds.x;

    rect_->SetPos( { baseline.x - RectMarginX, baseline.y - RectMarginY } );
    rect_->SetSize( { text_width + 2.0f * RectMarginX, line_height + 2.0f * RectMarginY } );

    const float cursor_width = 2.0f;
    cursor_->SetSize( { cursor_width, line_height } );
    cursor_->SetPos( { getCursorX(), baseline.y } );
}

void
pp::impl::Text::Insert( char c )
{
    if ( hasSelection() )
    {
        auto [begin, end] = getSelectionRange();
        string_.erase( begin, end - begin );
        cursor_pos_       = begin;
        selection_active_ = false;
    }

    string_.insert( string_.begin() + cursor_pos_, c );
    ++cursor_pos_;
    text_->SetText( string_ );
    updateRect();
}

void
pp::impl::Text::Backspace()
{
    if ( string_.empty() )
    {
        return;
    }

    if ( hasSelection() )
    {
        auto [begin, end] = getSelectionRange();
        string_.erase( begin, end - begin );
        cursor_pos_       = begin;
        selection_active_ = false;
        text_->SetText( string_ );
        updateRect();
        return;
    }

    if ( cursor_pos_ == 0 )
    {
        return;
    }

    string_.erase( string_.begin() + cursor_pos_ - 1 );
    --cursor_pos_;
    text_->SetText( string_ );
    updateRect();
}

void
pp::impl::Text::Delete()
{
    if ( string_.empty() )
    {
        return;
    }

    if ( hasSelection() )
    {
        auto [begin, end] = getSelectionRange();
        string_.erase( begin, end - begin );
        cursor_pos_       = begin;
        selection_active_ = false;
        text_->SetText( string_ );
        updateRect();
        return;
    }

    if ( cursor_pos_ == string_.size() )
    {
        return;
    }

    string_.erase( string_.begin() + cursor_pos_ );
    text_->SetText( string_ );
    updateRect();
}

void
pp::impl::Text::DecrementCursor()
{
    if ( cursor_pos_ != 0 )
    {
        --cursor_pos_;
    }

    updateRect();
}

void
pp::impl::Text::IncrementCursor()
{
    if ( cursor_pos_ != string_.size() )
    {
        ++cursor_pos_;
    }

    updateRect();
}

float
pp::impl::Text::getCursorX() const
{
    dr4::Vec2f baseline = text_->GetPos();

    if ( string_.empty() || cursor_pos_ == 0 )
    {
        return baseline.x;
    }

    if ( cursor_pos_ >= string_.size() )
    {
        return baseline.x + text_->GetBounds().x;
    }

    tmp_string_ = string_;
    text_->SetText( std::string( string_.data(), cursor_pos_ ) );
    float prefix_w = text_->GetBounds().x;
    text_->SetText( tmp_string_ );

    return baseline.x + prefix_w;
}

void
pp::impl::Text::insertCursor( const dr4::Event::MouseButton& evt )
{
    const dr4::Window* win = cvs_->GetWindow();

    dr4::Vec2f mouse    = evt.pos;
    dr4::Vec2f text_pos = text_->GetPos();

    float local_x = mouse.x - text_pos.x;

    if ( local_x <= 0.0f || string_.empty() )
    {
        cursor_pos_ = 0;
        return;
    }

    tmp_string_ = text_->GetText();

    size_t best_index = string_.size();
    float  prev_w     = 0.0f;
    float  cur_w      = 0.0f;

    for ( size_t i = 1; i <= string_.size(); ++i )
    {
        text_->SetText( std::string( string_.data(), i ) );
        cur_w = text_->GetBounds().x;

        if ( cur_w >= local_x )
        {
            float mid = ( prev_w + cur_w ) * 0.5f;
            if ( local_x < mid )
            {
                best_index = i - 1;
            } else
            {
                best_index = i;
            }
            break;
        }

        prev_w = cur_w;
    }

    text_->SetText( tmp_string_ );

    cursor_pos_ = best_index;

    updateRect();
}

std::pair<size_t, size_t>
pp::impl::Text::getSelectionRange() const
{
    if ( !selection_active_ )
    {
        return { 0, 0 };
    }

    return {
        std::min( cursor_pos_, selection_pos_ ),
        std::max( cursor_pos_, selection_pos_ ),
    };
}

bool
pp::impl::Text::hasSelection() const
{
    return selection_active_ && ( selection_pos_ != cursor_pos_ );
}

void
pp::impl::Text::clearSelection()
{
    selection_active_ = false;
}
