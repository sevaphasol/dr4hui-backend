#include <cassert>
#include <cmath>

#include "dr4/math/vec2.hpp"
#include "pp/canvas.hpp"
#include "pp/shapes/circle.hpp"

pp::impl::Circle::Circle( dr4::Window* window, const ::pp::ControlsTheme& theme, ::pp::Canvas* cvs )
    : circle_( window->CreateCircle() ), cvs_( cvs )
{
    circle_->SetBorderThickness( CircleBorderThickness );
    circle_->SetBorderColor( theme.lineColor );
    circle_->SetFillColor( theme.shapeColor );

    for ( auto& rsz_circle : rsz_circles_ )
    {
        rsz_circle.reset( window->CreateCircle() );
        rsz_circle->SetRadius( ResizeCircleRadius );
        rsz_circle->SetFillColor( theme.handleColor );
    }
}

bool
pp::impl::Circle::OnMouseDown( const dr4::Event::MouseButton& evt )
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
pp::impl::Circle::OnMouseUp( const dr4::Event::MouseButton& evt )
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
pp::impl::Circle::OnMouseMove( const dr4::Event::MouseMove& evt )
{
    if ( is_dragged_ )
    {
        SetCenter( GetCenter() + evt.rel );
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
pp::impl::Circle::OnSelect()
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
pp::impl::Circle::OnDeselect()
{
}

void
pp::impl::Circle::DrawOn( dr4::Texture& texture ) const
{
    circle_->DrawOn( texture );

    if ( cvs_->GetSelectedShape() == this )
    {
        drawResizeCircles( texture );
    }
}

void
pp::impl::Circle::SetPos( dr4::Vec2f pos )
{
    circle_->SetCenter( pos );
    setResizeCircles();
}

dr4::Vec2f
pp::impl::Circle::GetPos() const
{
    return circle_->GetPos();
}

void
pp::impl::Circle::SetCenter( dr4::Vec2f pos )
{
    circle_->SetCenter( pos );
    setResizeCircles();
}

dr4::Vec2f
pp::impl::Circle::GetCenter() const
{
    return circle_->GetCenter();
}

void
pp::impl::Circle::SetSize( dr4::Vec2f size )
{
    bool small_x = std::fabs( size.x ) < 1e-4;
    bool small_y = std::fabs( size.y ) < 1e-4;

    if ( small_x && small_y )
    {
        circle_->SetRadius( 0.0f );
        setResizeCircles();
        return;
    }

    if ( !small_y )
    {
        circle_->SetRadius( { size.x, size.y } );
        setResizeCircles();
        return;
    }

    circle_->SetRadius( { size.x, size.y } );
    setResizeCircles();
}

dr4::Vec2f
pp::impl::Circle::GetSize() const
{
    return circle_->GetRadius();
}

bool
pp::impl::Circle::onMe( dr4::Vec2f rel ) const
{
    dr4::Vec2f circle_size   = GetSize();
    dr4::Vec2f circle_center = circle_->GetCenter();

    dr4::Vec2f delta = rel - circle_center;

    auto transformed_dist_sq = ( delta.x * delta.x ) / ( circle_size.x * circle_size.x ) +
                               ( delta.y * delta.y ) / ( circle_size.y * circle_size.y );

    float transformed_radius_sq = 1.0f;

    return transformed_dist_sq < transformed_radius_sq;
}

pp::impl::Circle::ResizeCircle
pp::impl::Circle::getResizeCircle( dr4::Vec2f pos ) const
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
pp::impl::Circle::resize( dr4::Vec2f offset )
{
    dr4::Vec2f rect_center = GetCenter();
    dr4::Vec2f rect_size   = GetSize();

    switch ( active_rsz_circle_ )
    {
        case ResizeCircle::Lft:
            {
                rect_center.x += offset.x;
                rect_size.x -= offset.x;
                break;
            }

        case ResizeCircle::Top:
            {
                rect_center.y += offset.y;
                rect_size.y -= offset.y;
                break;
            }

        case ResizeCircle::Rht:
            {
                rect_center.x -= offset.x;
                rect_size.x += offset.x;
                break;
            }

        case ResizeCircle::Bot:
            {
                rect_center.y -= offset.y;
                rect_size.y += offset.y;
                break;
            }

        default:
            return;
    }

    SetCenter( rect_center );
    SetSize( rect_size );

    setResizeCircles();
}

void
pp::impl::Circle::setResizeCircles()
{
    dr4::Vec2f rect_center = GetCenter();
    dr4::Vec2f rect_size   = GetSize();

    float psx = rect_center.x;
    float psy = rect_center.y;

    float lft = psx - rect_size.x;
    float top = psy - rect_size.y;
    float rht = psx + rect_size.x;
    float bot = psy + rect_size.y;

    rsz_circles_[ResizeCircle::Lft]->SetCenter( { lft, psy } );
    rsz_circles_[ResizeCircle::Top]->SetCenter( { psx, top } );
    rsz_circles_[ResizeCircle::Rht]->SetCenter( { rht, psy } );
    rsz_circles_[ResizeCircle::Bot]->SetCenter( { psx, bot } );
}

void
pp::impl::Circle::drawResizeCircles( dr4::Texture& tex ) const
{
    for ( const auto& rsz_circle : rsz_circles_ )
    {
        rsz_circle->DrawOn( tex );
    }
}
