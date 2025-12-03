#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <string_view>

#include "dr4/event.hpp"
#include "dr4/keycodes.hpp"
#include "dr4/math/rect.hpp"
#include "dr4/math/vec2.hpp"
#include "pp/shapes/text.hpp"
#include "pp/tools/text.hpp"

pp::impl::Text::Text( dr4::Window*               window,
                      const ::pp::ControlsTheme& theme,
                      pp::Canvas*                cvs,
                      pp::impl::TextTool*        text_tool )
    : rect_( window->CreateRectangle() ),
      text_( window->CreateText() ),
      cursor_( window->CreateRectangle() ),
      selection_rect_( window->CreateRectangle() ),
      cvs_( cvs ),
      text_tool_( text_tool )
{
    rect_->SetBorderThickness( RectBorderThickness );
    rect_->SetBorderColor( { 118, 185, 0 } );
    rect_->SetFillColor( cvs_->GetControlsTheme().shapeColor );

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
            insertCursor( evt.pos );
            clearSelection();
            is_selecting_ = true;
        } else
        {
            is_dragged_ = true;
        }

        OnSelect();

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

    if ( is_drawing_ && is_selecting_ )
    {
        is_selecting_     = false;
        selection_active_ = ( selection_pos_ != cursor_pos_ );
        return true;
    }

    is_dragged_ = false;

    return true;
}

bool
pp::impl::Text::OnMouseMove( const dr4::Event::MouseMove& evt )
{
    if ( is_drawing_ && is_selecting_ )
    {
        insertCursor( evt.pos );
        selection_active_ = ( selection_pos_ != cursor_pos_ );
        updateRect();

        return true;
    }

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
        ( ( evt.mods & dr4::KEYMOD_CTRL ) != 0 ) ? ctrlBackspace() : backspace();
        return true;
    }

    if ( evt.sym == dr4::KEYCODE_DELETE )
    {
        ( ( evt.mods & dr4::KEYMOD_CTRL ) != 0 ) ? ctrlDel() : del();

        return true;
    }

    if ( evt.sym == dr4::KEYCODE_LEFT || evt.sym == dr4::KEYCODE_RIGHT )
    {
        size_t old_pos = cursor_pos_;

        bool shift = ( evt.mods & dr4::KEYMOD_SHIFT ) != 0;

        if ( shift )
        {
            if ( !selection_active_ )
            {
                selection_pos_    = old_pos;
                selection_active_ = true;
            }
        } else
        {
            clearSelection();
        }

        if ( ( evt.mods & dr4::KEYMOD_CTRL ) != 0 )
        {
            ( evt.sym == dr4::KEYCODE_LEFT ) ? moveCursorWordLeft() : moveCursorWordRight();
        } else
        {
            ( evt.sym == dr4::KEYCODE_LEFT ) ? decrementCursor() : incrementCursor();
        }

        if ( selection_active_ && selection_pos_ == cursor_pos_ )
        {
            clearSelection();
        }

        cursor_visible_         = true;
        cursor_last_blink_time_ = cvs_->GetWindow()->GetTime();

        return true;
    }

    if ( ( evt.mods & dr4::KEYMOD_CTRL ) != 0 )
    {
        switch ( evt.sym )
        {
            case dr4::KEYCODE_X:
                ctrlX();
                break;
            case dr4::KEYCODE_C:
                ctrlC();
                break;
            case dr4::KEYCODE_V:
                ctrlV();
                break;
            case dr4::KEYCODE_A:
                ctrlA();
                break;
            case dr4::KEYCODE_W:
                ctrlW();
                break;
            default:
                break;
        }
    }

    return false;
}

bool
pp::impl::Text::OnText( const dr4::Event::TextEvent& evt )
{
    if ( evt.unicode != nullptr )
    {
        auto ch = *evt.unicode;

        if ( ( std::iscntrl( ch ) != 0 ) && ch != '\n' && ch != '\t' && ch != '\r' )
        {
            return false;
        }

        put( *evt.unicode );
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

    if ( is_drawing_ && selection_active_ )
    {
        selection_rect_->DrawOn( texture );
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

    float asc_up  = std::fabs( 1.3 * font->GetAscent( font_size ) );
    float desc_dn = std::fabs( 1.3 * font->GetDescent( font_size ) );

    float line_height = asc_up + desc_dn;

    dr4::Vec2f bounds     = text_->GetBounds();
    float      text_width = bounds.x;

    rect_->SetPos( { baseline.x - RectMarginX, baseline.y - RectMarginY } );
    rect_->SetSize( { text_width + 2.0f * RectMarginX, line_height + 2.0f * RectMarginY } );

    const float cursor_width = 2.0f;
    cursor_->SetSize( { cursor_width, line_height } );
    cursor_->SetPos( { getCursorX(), baseline.y } );

    if ( hasSelection() )
    {
        auto [begin, end] = getSelectionRange();

        tmp_string_ = text_->GetText();

        float start_w = 0.0f;
        if ( begin == 0 )
        {
            start_w = 0.0f;
        } else
        {
            text_->SetText( std::string( tmp_string_.data(), begin ) );
            start_w = text_->GetBounds().x;
        }

        float end_w = 0.0f;
        text_->SetText( std::string( tmp_string_.data(), end ) );
        end_w = text_->GetBounds().x;

        text_->SetText( tmp_string_ );

        float sel_x = baseline.x + start_w;
        float sel_y = baseline.y;
        float sel_w = end_w - start_w;
        float sel_h = line_height;

        selection_rect_->SetPos( { sel_x, sel_y } );
        selection_rect_->SetSize( { sel_w, sel_h } );
    }
}

void
pp::impl::Text::put( char c )
{
    if ( hasSelection() )
    {
        auto [begin, end] = getSelectionRange();
        string_.erase( begin, end - begin );
        cursor_pos_ = begin;
        clearSelection();
    }

    string_.insert( string_.begin() + cursor_pos_, c );
    ++cursor_pos_;
    text_->SetText( string_ );
    updateRect();
}

void
pp::impl::Text::backspace()
{
    if ( string_.empty() )
    {
        return;
    }

    if ( hasSelection() )
    {
        auto [begin, end] = getSelectionRange();
        string_.erase( begin, end - begin );
        cursor_pos_ = begin;
        clearSelection();
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
pp::impl::Text::del()
{
    if ( string_.empty() )
    {
        return;
    }

    if ( hasSelection() )
    {
        auto [begin, end] = getSelectionRange();
        string_.erase( begin, end - begin );
        cursor_pos_ = begin;
        clearSelection();
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
pp::impl::Text::decrementCursor()
{
    if ( cursor_pos_ != 0 )
    {
        --cursor_pos_;
    }

    updateRect();
}

void
pp::impl::Text::incrementCursor()
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
pp::impl::Text::insertCursor( dr4::Vec2f pos )
{
    dr4::Vec2f text_pos = text_->GetPos();

    float local_x = pos.x - text_pos.x;

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
    selection_pos_    = cursor_pos_;
}

std::string
pp::impl::Text::copySelected() const
{
    if ( !hasSelection() )
    {
        return "";
    }

    auto [begin, end] = getSelectionRange();

    return string_.substr( begin, end - begin );
}

void
pp::impl::Text::pasteFromClipboard()
{
    const std::string& clip = text_tool_->GetClipboard();

    if ( hasSelection() )
    {
        auto [begin, end] = getSelectionRange();
        string_.erase( begin, end - begin );
        cursor_pos_ = begin;
        clearSelection();
    }

    string_.insert( cursor_pos_, clip );
    cursor_pos_ += clip.size();
    text_->SetText( string_ );
    updateRect();
}

void
pp::impl::Text::eraseSelected()
{
    auto [begin, end] = getSelectionRange();
    string_.erase( begin, end - begin );
    cursor_pos_ = begin;
    clearSelection();
    text_->SetText( string_ );
    updateRect();
}

size_t
pp::impl::Text::getLeftWordPos()
{
    size_t pos = cursor_pos_;

    if ( pos == 0 )
    {
        return pos;
    }

    while ( pos > 0 && ( std::isspace( string_[pos - 1] ) != 0 ) )
    {
        --pos;
    }

    while ( pos > 0 && ( std::isspace( string_[pos - 1] ) == 0 ) )
    {
        --pos;
    }

    return pos;
}

size_t
pp::impl::Text::getRightWordPos()
{
    size_t pos = cursor_pos_;

    size_t n = string_.size();
    if ( pos >= n )
    {
        return pos;
    }

    while ( pos < n && ( std::isspace( string_[pos] ) != 0 ) )
    {
        ++pos;
    }

    while ( pos < n && ( std::isspace( string_[pos] ) == 0 ) )
    {
        ++pos;
    }

    return pos;
}

void
pp::impl::Text::moveCursorWordLeft()
{
    cursor_pos_ = getLeftWordPos();
    updateRect();
}

void
pp::impl::Text::moveCursorWordRight()
{
    cursor_pos_ = getRightWordPos();
    updateRect();
}

void
pp::impl::Text::ctrlBackspace()
{
    size_t start_pos = getLeftWordPos();
    if ( start_pos < cursor_pos_ )
    {
        size_t length = cursor_pos_ - start_pos;
        string_.erase( start_pos, length );
        cursor_pos_ = start_pos;
        clearSelection();
        text_->SetText( string_ );
        updateRect();
    }
}

void
pp::impl::Text::ctrlDel()
{
    size_t end_pos = getRightWordPos();
    if ( end_pos > cursor_pos_ )
    {
        size_t length = end_pos - cursor_pos_;
        string_.erase( cursor_pos_, length );
        clearSelection();
        text_->SetText( string_ );
        updateRect();
    }
}

void
pp::impl::Text::ctrlX()
{
    text_tool_->SetClipboard( copySelected() );
    text_tool_->SetClipboardSingleUse( true );
    eraseSelected();
}

void
pp::impl::Text::ctrlC()
{
    text_tool_->SetClipboard( copySelected() );
    text_tool_->SetClipboardSingleUse( false );
}

void
pp::impl::Text::ctrlV()
{
    pasteFromClipboard();

    if ( text_tool_->GetClipboardSingleUse() )
    {
        text_tool_->GetClipboard().erase();
    }
}

void
pp::impl::Text::ctrlA()
{
    selection_active_ = true;
    selection_pos_    = 0;
    cursor_pos_       = string_.end() - string_.begin();
    updateRect();
}

void
pp::impl::Text::ctrlW()
{
    ctrlBackspace();
    updateRect();
}
