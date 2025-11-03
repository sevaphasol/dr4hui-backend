#include "dr4/img_impl.hpp"
#include "dr4/math/color.hpp"
#include <SFML/Graphics/Vertex.hpp>

dr4::impl::Image::Image( unsigned width, unsigned height ) : w_( width ), h_( height )
{
    impl_.resize( width * height );
}

void
dr4::impl::Image::SetPixel( unsigned x, unsigned y, dr4::Color color )
{
    impl_[y][x] = { .position = { x, y }, .color = { color.r, color.g, color.b, color.a } };
}

dr4::Color
dr4::impl::Image::GetPixel( unsigned x, unsigned y ) const
{
    const auto& v = impl_[y * w_ + x];

    return { v.color.r, v.color.g, v.color.b, v.color.a };
}

void
dr4::impl::Image::SetSize( dr4::Vec2f size )
{
    w_ = size.x;
    h_ = size.y;

    impl_.resize( w_ * h_ );
}

dr4::Vec2f
dr4::impl::Image::GetSize() const
{
    return { w_, h_ };
}

float
dr4::impl::Image::GetWidth() const
{
    return w_;
}

float
dr4::impl::Image::GetHeight() const
{
    return h_;
}
