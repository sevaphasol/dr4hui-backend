#include <cassert>
#include <cmath>

#include "dr4/event.hpp"
#include "dr4/mouse_buttons.hpp"
#include "pp/shapes/line.hpp"
#include "pp/tools/line.hpp"

pp::impl::LineTool::LineTool( pp::Canvas* cvs )
    : cvs_( cvs ), is_drawing_( false ), line_( nullptr )
{
}

std::string_view
pp::impl::LineTool::Icon() const
{
    return "󰕞";
}

std::string_view
pp::impl::LineTool::Name() const
{
    return "LineTool";
}

bool
pp::impl::LineTool::IsCurrentlyDrawing() const
{
    return is_drawing_;
}

void
pp::impl::LineTool::OnStart()
{
    cvs_->SetSelectedShape( nullptr );
}

void
pp::impl::LineTool::OnBreak()
{
    if ( is_drawing_ )
    {
        assert( line_ );
        is_drawing_ = false;
        cvs_->DelShape( line_ );
    }
}

void
pp::impl::LineTool::OnEnd()
{
    if ( is_drawing_ )
    {
        is_drawing_ = false;
        line_->OnSelect();
    }
}

bool
pp::impl::LineTool::OnMouseDown( const dr4::Event::MouseButton& evt )
{
    if ( evt.button != dr4::MouseButtonType::LEFT )
    {
        return false;
    }

    if ( !is_drawing_ )
    {
        is_drawing_ = true;
        cvs_->SetSelectedShape( nullptr );

        line_ = new pp::impl::Line( cvs_->GetWindow(), cvs_->GetControlsTheme(), cvs_ );
        cvs_->AddShape( line_ );
        line_->SetStart( evt.pos );
        line_->SetEnd( evt.pos );

        return true;
    }

    is_drawing_ = false;
    line_->OnSelect();

    return true;
}

bool
pp::impl::LineTool::OnMouseUp( const dr4::Event::MouseButton& evt )
{
    return false;
}

bool
pp::impl::LineTool::OnMouseMove( const dr4::Event::MouseMove& evt )
{
    if ( !is_drawing_ )
    {
        return false;
    }

    assert( line_ );

    auto delta = evt.pos - line_->GetEnd();

    line_->SetEnd( line_->GetEnd() + delta );

    return true;
}
