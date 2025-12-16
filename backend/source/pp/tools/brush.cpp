#include "pp/tools/brush.hpp"
#include "dr4/mouse_buttons.hpp"
#include <cmath>

pp::impl::BrushTool::BrushTool( pp::Canvas* cvs ) : cvs_( cvs ) {}

std::string_view
pp::impl::BrushTool::Icon() const
{
    return "󰃣";
}
std::string_view
pp::impl::BrushTool::Name() const
{
    return "BrushTool";
}

void
pp::impl::BrushTool::OnStart()
{
    cvs_->SetSelectedShape( nullptr );
}

void
pp::impl::BrushTool::OnBreak()
{
    if ( is_drawing_ )
    {
        is_drawing_ = false;
        cvs_->DelShape( brush_ );
        brush_ = nullptr;
    }
}

void
pp::impl::BrushTool::OnEnd()
{
    if ( is_drawing_ )
    {
        is_drawing_ = false;
    }
}

bool
pp::impl::BrushTool::OnMouseDown( const dr4::Event::MouseButton& evt )
{
    if ( evt.button != dr4::MouseButtonType::LEFT )
    {
        return false;
    }

    brush_ = new pp::impl::BrushShape( cvs_->GetWindow(), cvs_->GetControlsTheme(), cvs_ );
    cvs_->AddShape( brush_ );
    last_pos_ = evt.pos;
    brush_->AddPoint( evt.pos );
    is_drawing_ = true;
    return true;
}

bool
pp::impl::BrushTool::OnMouseUp( const dr4::Event::MouseButton& evt )
{
    if ( evt.button == dr4::MouseButtonType::LEFT )
    {
        is_drawing_ = false;
        return true;
    }
    return false;
}

bool
pp::impl::BrushTool::OnMouseMove( const dr4::Event::MouseMove& evt )
{
    if ( !is_drawing_ )
    {
        return false;
    }

    AddSegment( last_pos_, evt.pos );
    last_pos_ = evt.pos;
    return true;
}

void
pp::impl::BrushTool::AddSegment( dr4::Vec2f from, dr4::Vec2f to )
{
    const float brush_radius = pp::impl::BrushShape::BrushRadius;
    const float max_step     = brush_radius * 2.0f;

    dr4::Vec2f delta  = to - from;
    float      length = std::sqrt( delta.x * delta.x + delta.y * delta.y );

    if ( length < 1e-5f )
    {
        brush_->AddPoint( to );
        return;
    }

    int steps = static_cast<int>( std::ceil( length / max_step ) );
    steps     = std::max( 1, steps );

    for ( int i = 1; i <= steps; ++i )
    {
        float      t     = static_cast<float>( i ) / static_cast<float>( steps );
        dr4::Vec2f point = from + delta * t;
        brush_->AddPoint( point );
    }
}
