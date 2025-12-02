#include <cassert>
#include <cmath>

#include "dr4/math/rect.hpp"
#include "dr4/math/vec2.hpp"
#include "pp/shapes/rect.hpp"

pp::impl::Rect::Rect( dr4::Window* window, const ::pp::ControlsTheme& theme, ::pp::Canvas* cvs )
    : rect_( window->CreateRectangle() ), cvs_( cvs )
{
    rect_->SetBorderThickness( RectBorderThickness );
    rect_->SetBorderColor( theme.lineColor );
    rect_->SetFillColor( theme.shapeColor );

    for ( auto& rsz_circle : rsz_circles_ )
    {
        rsz_circle.reset( window->CreateCircle() );
        rsz_circle->SetRadius( ResizeCircleRadius );
        rsz_circle->SetFillColor( theme.handleColor );
    }
}

bool
pp::impl::Rect::OnMouseDown( const dr4::Event::MouseButton& evt )
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
pp::impl::Rect::OnMouseUp( const dr4::Event::MouseButton& evt )
{
    if ( evt.button == dr4::MouseButtonType::LEFT )
    {
        is_resized_ = false;
        is_dragged_ = false;

        return true;
    }

    return false;
}

bool
pp::impl::Rect::OnMouseMove( const dr4::Event::MouseMove& evt )
{
    if ( is_dragged_ )
    {
        SetPos( rect_->GetPos() + evt.rel );
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
pp::impl::Rect::OnSelect()
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
pp::impl::Rect::OnDeselect()
{
}

void
pp::impl::Rect::DrawOn( dr4::Texture& texture ) const
{
    rect_->DrawOn( texture );

    if ( cvs_->GetSelectedShape() == this )
    {
        drawResizeCircles( texture );
    }
}

void
pp::impl::Rect::SetPos( dr4::Vec2f pos )
{
    rect_->SetPos( pos );
    setResizeCircles();
}

dr4::Vec2f
pp::impl::Rect::GetPos() const
{
    return rect_->GetPos();
}

void
pp::impl::Rect::SetSize( dr4::Vec2f size )
{
    rect_->SetSize( { size.x, size.y } );

    setResizeCircles();
}

dr4::Vec2f
pp::impl::Rect::GetSize()
{
    return rect_->GetSize();
}

bool
pp::impl::Rect::onMe( dr4::Vec2f rel ) const
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

pp::impl::Rect::ResizeCircle
pp::impl::Rect::getResizeCircle( dr4::Vec2f pos ) const
{
    for ( int i = 0; i < ResizeCircle::Count; ++i )
    {
        dr4::Vec2f corner_pos = rsz_circles_[i]->GetCenter();

        if ( std::fabs( pos.x - corner_pos.x ) <= ResizeCircleRadius &&
             std::fabs( pos.y - corner_pos.y ) <= ResizeCircleRadius )
        {
            return static_cast<ResizeCircle>( i );
        }
    }

    return ResizeCircle::Count;
}

void
pp::impl::Rect::resize( dr4::Vec2f offset )
{
    dr4::Vec2f rect_pos  = rect_->GetPos();
    dr4::Vec2f rect_size = rect_->GetSize();

    switch ( active_rsz_circle_ )
    {
        case ResizeCircle::CornerLftTop:
            {
                rect_pos += offset;
                rect_size -= offset;
                break;
            }

        case ResizeCircle::CornerLftBot:
            {
                rect_pos.x += offset.x;
                rect_size.x -= offset.x;
                rect_size.y += offset.y;
                break;
            }

        case ResizeCircle::CornerRhtTop:
            {
                rect_pos.y += offset.y;
                rect_size.y -= offset.y;
                rect_size.x += offset.x;
                break;
            }

        case ResizeCircle::CornerRhtBot:
            {
                rect_size += offset;
                break;
            }

        case ResizeCircle::SideLft:
            {
                rect_pos.x += offset.x;
                rect_size.x -= offset.x;
                break;
            }

        case ResizeCircle::SideRht:
            {
                rect_size.x += offset.x;
                break;
            }

        case ResizeCircle::SideTop:
            {
                rect_pos.y += offset.y;
                rect_size.y -= offset.y;
                break;
            }

        case ResizeCircle::SideBot:
            {
                rect_size.y += offset.y;
                break;
            }

        default:
            return;
    }

    rect_->SetPos( rect_pos );
    rect_->SetSize( rect_size );

    setResizeCircles();
}

void
pp::impl::Rect::setResizeCircles()
{
    dr4::Vec2f rect_pos  = rect_->GetPos();
    dr4::Vec2f rect_size = rect_->GetSize();

    float lft   = rect_pos.x;
    float rht   = rect_pos.x + rect_size.x;
    float top   = rect_pos.y;
    float bot   = rect_pos.y + rect_size.y;
    float mid_x = rect_pos.x + rect_size.x / 2;
    float mid_y = rect_pos.y + rect_size.y / 2;

    rsz_circles_[ResizeCircle::CornerLftTop]->SetCenter( { lft, top } );
    rsz_circles_[ResizeCircle::CornerLftBot]->SetCenter( { lft, bot } );
    rsz_circles_[ResizeCircle::CornerRhtTop]->SetCenter( { rht, top } );
    rsz_circles_[ResizeCircle::CornerRhtBot]->SetCenter( { rht, bot } );

    rsz_circles_[ResizeCircle::SideLft]->SetCenter( { lft, mid_y } );
    rsz_circles_[ResizeCircle::SideRht]->SetCenter( { rht, mid_y } );
    rsz_circles_[ResizeCircle::SideBot]->SetCenter( { mid_x, bot } );
    rsz_circles_[ResizeCircle::SideTop]->SetCenter( { mid_x, top } );
}

void
pp::impl::Rect::drawResizeCircles( dr4::Texture& tex ) const
{
    for ( const auto& rsz_circle : rsz_circles_ )
    {
        rsz_circle->DrawOn( tex );
    }
}
