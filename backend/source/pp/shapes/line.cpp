#include <cassert>
#include <cmath>

#include "dr4/math/vec2.hpp"
#include "pp/canvas.hpp"
#include "pp/shapes/line.hpp"

pp::impl::Line::Line( dr4::Window* window, const pp::ControlsTheme& theme, pp::Canvas* cvs )
    : line_( window->CreateLine() ), cvs_( cvs )
{
    line_->SetThickness( LineThickness );
    line_->SetColor( theme.lineColor );

    for ( auto& rsz_circle : rsz_circles_ )
    {
        rsz_circle.reset( window->CreateCircle() );
        rsz_circle->SetRadius( ResizeCircleRadius );
        rsz_circle->SetFillColor( theme.handleColor );
    }
}

bool
pp::impl::Line::OnMouseDown( const dr4::Event::MouseButton& evt )
{
    if ( evt.button != dr4::MouseButtonType::LEFT )
    {
        return false;
    }

    if ( cvs_->GetSelectedShape() == this )
    {
        active_rsz_circle_ = getResizeCircle( evt.pos );

        if ( active_rsz_circle_ != ResizeCircle::Count )
        {
            is_resized_ = true;
            return true;
        }
    }

    if ( onMe( evt.pos ) )
    {
        OnSelect();
        is_dragged_ = true;
        return true;
    }

    return false;
}

bool
pp::impl::Line::OnMouseUp( const dr4::Event::MouseButton& evt )
{
    if ( evt.button != dr4::MouseButtonType::LEFT )
    {
        return false;
    }

    is_resized_ = false;
    is_dragged_ = false;

    return true;
}

bool
pp::impl::Line::OnMouseMove( const dr4::Event::MouseMove& evt )
{
    if ( is_dragged_ )
    {
        line_->SetStart( line_->GetStart() + evt.rel );
        line_->SetEnd( line_->GetEnd() + evt.rel );
        setResizeCircles();

        return true;
    }

    if ( is_resized_ )
    {
        resize( evt.rel );
        return true;
    }

    return false;
}

void
pp::impl::Line::OnSelect()
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
pp::impl::Line::OnDeselect()
{
}

void
pp::impl::Line::DrawOn( dr4::Texture& texture ) const
{
    line_->DrawOn( texture );

    if ( cvs_->GetSelectedShape() == this )
    {
        drawResizeCircles( texture );
    }
}

void
pp::impl::Line::SetPos( dr4::Vec2f pos )
{
    dr4::Vec2f dir = line_->GetEnd() - line_->GetStart();

    line_->SetStart( pos );
    line_->SetEnd( pos + dir );

    setResizeCircles();
}

dr4::Vec2f
pp::impl::Line::GetPos() const
{
    return line_->GetStart();
}

void
pp::impl::Line::SetStart( dr4::Vec2f pos )
{
    line_->SetStart( pos );
    setResizeCircles();
}

dr4::Vec2f
pp::impl::Line::GetStart() const
{
    return line_->GetStart();
}

void
pp::impl::Line::SetEnd( dr4::Vec2f pos )
{
    line_->SetEnd( pos );
    setResizeCircles();
}

dr4::Vec2f
pp::impl::Line::GetEnd() const
{
    return line_->GetEnd();
}

bool
pp::impl::Line::onMe( dr4::Vec2f rel ) const
{
    dr4::Vec2f s = line_->GetStart();
    dr4::Vec2f e = line_->GetEnd();

    float x0 = rel.x;
    float y0 = rel.y;

    float dx = e.x - s.x;
    float dy = e.y - s.y;

    float len_sq = dx * dx + dy * dy;

    if ( len_sq < 1e-4f )
    {
        float disq_sq = ( ( x0 - s.x ) * ( x0 - s.x ) + ( y0 - s.y ) * ( y0 - s.y ) );
        return disq_sq <= LineThickness * LineThickness;
    }

    float t = ( ( x0 - s.x ) * dx + ( y0 - s.y ) * dy ) / len_sq;

    t = std::clamp( t, 0.0f, 1.0f );

    float proj_x = s.x + t * dx;
    float proj_y = s.y + t * dy;

    float dist_sq = ( x0 - proj_x ) * ( x0 - proj_x ) + ( y0 - proj_y ) * ( y0 - proj_y );

    return dist_sq <= LineThickness * LineThickness;
}

pp::impl::Line::ResizeCircle
pp::impl::Line::getResizeCircle( dr4::Vec2f pos ) const
{
    for ( int i = 0; i < ResizeCircle::Count; ++i )
    {
        dr4::Vec2f rsz_circl_pos = rsz_circles_[i]->GetCenter();

        if ( std::fabs( pos.x - rsz_circl_pos.x ) <= ResizeCircleRadius &&
             std::fabs( pos.y - rsz_circl_pos.y ) <= ResizeCircleRadius )
        {
            return static_cast<ResizeCircle>( i );
        }
    }

    return ResizeCircle::Count;
}

void
pp::impl::Line::resize( dr4::Vec2f offset )
{
    switch ( active_rsz_circle_ )
    {
        case ResizeCircle::Start:
            {
                line_->SetStart( line_->GetStart() + offset );
                break;
            }

        case ResizeCircle::End:
            {
                line_->SetEnd( line_->GetEnd() + offset );
                break;
            }

        default:
            return;
    }

    setResizeCircles();
}

void
pp::impl::Line::setResizeCircles()
{
    rsz_circles_[ResizeCircle::Start]->SetCenter( line_->GetStart() );
    rsz_circles_[ResizeCircle::End]->SetCenter( line_->GetEnd() );
}

void
pp::impl::Line::drawResizeCircles( dr4::Texture& tex ) const
{
    for ( const auto& rsz_circle : rsz_circles_ )
    {
        rsz_circle->DrawOn( tex );
    }
}
