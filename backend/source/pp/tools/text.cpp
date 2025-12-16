#include <cassert>
#include <cctype>
#include <cmath>

#include "dr4/keycodes.hpp"
#include "dr4/mouse_buttons.hpp"
#include "pp/shapes/text.hpp"
#include "pp/tools/text.hpp"

pp::impl::TextTool::TextTool( pp::Canvas* cvs )
    : cvs_( cvs ), is_drawing_( false ), text_( nullptr )
{
}

std::string_view
pp::impl::TextTool::Icon() const
{
    return "󰊄";
}

std::string_view
pp::impl::TextTool::Name() const
{
    return "TextTool";
}

bool
pp::impl::TextTool::IsCurrentlyDrawing() const
{
    return is_drawing_;
}

void
pp::impl::TextTool::OnStart()
{
    cvs_->SetSelectedShape( nullptr );
}

void
pp::impl::TextTool::OnBreak()
{
    if ( is_drawing_ )
    {
        assert( text_ );
        is_drawing_ = false;
        text_->SetIsDrawing( false );
        cvs_->DelShape( text_ );
        cvs_->SetSelectedShape( nullptr );
        text_ = nullptr;
    }
}

void
pp::impl::TextTool::OnEnd()
{
    if ( is_drawing_ )
    {
        is_drawing_ = false;
        text_->SetIsDrawing( false );
        text_->OnSelect();
        text_ = nullptr;
    }
}

bool
pp::impl::TextTool::OnMouseDown( const dr4::Event::MouseButton& evt )
{
    if ( evt.button != dr4::MouseButtonType::LEFT )
    {
        return false;
    }

    if ( !is_drawing_ )
    {
        is_drawing_ = true;
        if ( cvs_->GetSelectedShape() != nullptr )
        {
            cvs_->GetSelectedShape()->OnDeselect();
        }
        cvs_->SetSelectedShape( nullptr );
        text_ = new pp::impl::Text( cvs_->GetWindow(), cvs_->GetControlsTheme(), cvs_ );
        cvs_->AddShape( text_ );
        text_->SetPos( evt.pos );
        text_->OnSelect();
        text_->SetIsDrawing( true );

        return true;
    }

    if ( text_->OnMouseDown( evt ) )
    {
        return true;
    }

    is_drawing_ = false;
    text_->OnSelect();
    text_->SetIsDrawing( false );

    return true;
}

bool
pp::impl::TextTool::OnMouseUp( const dr4::Event::MouseButton& evt )
{
    if ( !is_drawing_ )
    {
        return false;
    }

    return text_->OnMouseUp( evt );
}

bool
pp::impl::TextTool::OnMouseMove( const dr4::Event::MouseMove& evt )
{
    if ( !is_drawing_ )
    {
        return false;
    }

    return text_->OnMouseMove( evt );
}

bool
pp::impl::TextTool::OnKeyDown( const dr4::Event::KeyEvent& evt )
{
    if ( !is_drawing_ )
    {
        return false;
    }

    return text_->OnKeyDown( evt );
}

bool
pp::impl::TextTool::OnKeyUp( const dr4::Event::KeyEvent& evt )
{
    if ( !is_drawing_ )
    {
        return false;
    }

    return text_->OnKeyUp( evt );
}

bool
pp::impl::TextTool::OnText( const dr4::Event::TextEvent& evt )
{
    if ( !is_drawing_ )
    {
        return false;
    }

    return text_->OnText( evt );
}
