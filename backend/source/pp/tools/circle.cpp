#include <cassert>
#include <cmath>

#include "dr4/event.hpp"
#include "dr4/mouse_buttons.hpp"
#include "pp/shapes/circle.hpp"
#include "pp/tools/circle.hpp"

pp::impl::CircleTool::CircleTool( ::pp::Canvas* cvs )
    : cvs_( cvs ), is_drawing_( false ), circle_{ nullptr }
{
}

std::string_view
pp::impl::CircleTool::Icon() const
{
    return "C";
}

std::string_view
pp::impl::CircleTool::Name() const
{
    return "CircleTool";
}

bool
pp::impl::CircleTool::IsCurrentlyDrawing() const
{
    return is_drawing_;
}

void
pp::impl::CircleTool::OnStart()
{
}

void
pp::impl::CircleTool::OnBreak()
{
    if ( is_drawing_ )
    {
        assert( circle_ );
        is_drawing_ = false;
        cvs_->DelShape( circle_ );
    }
}

void
pp::impl::CircleTool::OnEnd()
{
    if ( is_drawing_ )
    {
        is_drawing_ = false;
        circle_->OnSelect();
    }
}

bool
pp::impl::CircleTool::OnMouseDown( const dr4::Event::MouseButton& evt )
{
    if ( evt.button != dr4::MouseButtonType::LEFT )
    {
        return false;
    }

    if ( !is_drawing_ )
    {
        is_drawing_ = true;

        circle_ = new pp::impl::Circle( cvs_->GetWindow(), cvs_->GetControlsTheme(), cvs_ );
        cvs_->AddShape( circle_ );
        circle_->SetPos( evt.pos );
        return true;
    }

    is_drawing_ = false;
    circle_->OnSelect();
    return true;
}

bool
pp::impl::CircleTool::OnMouseUp( const dr4::Event::MouseButton& evt )
{
    return false;
}

bool
pp::impl::CircleTool::OnMouseMove( const dr4::Event::MouseMove& evt )
{
    if ( !is_drawing_ )
    {
        return false;
    }

    assert( circle_ );

    auto delta = evt.pos - circle_->GetPos();

    circle_->SetSize( { delta.x / 2, delta.y / 2 } );

    return true;
}
