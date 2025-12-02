#include <cassert>
#include <cmath>

#include "dr4/mouse_buttons.hpp"
#include "pp/shapes/rect.hpp"
#include "pp/tools/rect.hpp"

pp::impl::RectTool::RectTool( ::pp::Canvas* cvs )
    : cvs_( cvs ), is_drawing_( false ), rect_( nullptr )
{
}

std::string_view
pp::impl::RectTool::Icon() const
{
    return "R";
}

std::string_view
pp::impl::RectTool::Name() const
{
    return "RectTool";
}

bool
pp::impl::RectTool::IsCurrentlyDrawing() const
{
    return is_drawing_;
}

void
pp::impl::RectTool::OnStart()
{
}

void
pp::impl::RectTool::OnBreak()
{
    if ( is_drawing_ )
    {
        assert( rect_ );
        is_drawing_ = false;
        cvs_->DelShape( rect_ );
    }
}

void
pp::impl::RectTool::OnEnd()
{
    if ( is_drawing_ )
    {
        is_drawing_ = false;
        rect_->OnSelect();
    }
}

bool
pp::impl::RectTool::OnMouseDown( const dr4::Event::MouseButton& evt )
{
    if ( evt.button != dr4::MouseButtonType::LEFT )
    {
        return false;
    }

    if ( !is_drawing_ )
    {
        is_drawing_ = true;

        rect_ = new pp::impl::Rect( cvs_->GetWindow(), cvs_->GetControlsTheme(), cvs_ );
        cvs_->AddShape( rect_ );
        rect_->SetPos( evt.pos );
        return true;
    }

    is_drawing_ = false;
    rect_->OnSelect();
    return true;
}

bool
pp::impl::RectTool::OnMouseUp( const dr4::Event::MouseButton& evt )
{
    return false;
}

bool
pp::impl::RectTool::OnMouseMove( const dr4::Event::MouseMove& evt )
{
    if ( !is_drawing_ )
    {
        return false;
    }

    assert( rect_ );

    rect_->SetSize( evt.pos - rect_->GetPos() );

    return true;
}
