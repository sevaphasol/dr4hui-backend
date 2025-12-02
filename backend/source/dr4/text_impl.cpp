#include "dr4/text_impl.hpp"
#include "dr4/font_impl.hpp"
#include "dr4/texture_impl.hpp"
#include <SFML/Graphics/Text.hpp>
#include <iostream>

void
dr4::impl::Text::SetText( const std::string& text )
{
    string_ = text;
    impl_.setString( text );
}

void
dr4::impl::Text::SetColor( Color color )
{
    impl_.setFillColor( { color.r, color.g, color.b, color.a } );
}

void
dr4::impl::Text::SetFontSize( float size )
{
    impl_.setCharacterSize( size );
}

void
dr4::impl::Text::SetVAlign( dr4::Text::VAlign align )
{
    std::cerr << "Sorry, " << __func__ << "unimplemented" << std::endl;
}

void
dr4::impl::Text::SetFont( const dr4::Font* font )
{
    const dr4::impl::Font& my_font = dynamic_cast<const dr4::impl::Font&>( *font );

    impl_.setFont( my_font.GetImpl() );
}

dr4::Vec2f
dr4::impl::Text::GetBounds() const
{
    auto bounds = impl_.getLocalBounds();

    return { bounds.width, bounds.height };
}

const std::string&
dr4::impl::Text::GetText() const
{
    return string_;
}

dr4::Color
dr4::impl::Text::GetColor() const
{
    auto sf_color = impl_.getFillColor();

    return { sf_color.r, sf_color.g, sf_color.b, sf_color.a };
}

float
dr4::impl::Text::GetFontSize() const
{
    return impl_.getCharacterSize();
}

dr4::Text::VAlign
dr4::impl::Text::GetVAlign() const
{
    std::cerr << "Sorry, " << __func__ << "unimplemented" << std::endl;

    return dr4::Text::VAlign::UNKNOWN;
}

const dr4::Font*
dr4::impl::Text::GetFont() const
{
    return &font_;
}

void
dr4::impl::Text::DrawOn( dr4::Texture& texture ) const
{
    auto& my_texture = dynamic_cast<dr4::impl::Texture&>( texture );

    sf::Transform sf_transform;

    auto tex_zero = my_texture.GetZero();

    sf_transform.translate( { tex_zero.x, tex_zero.y } );

    my_texture.GetImpl().draw( impl_, sf_transform );
}

void
dr4::impl::Text::SetPos( dr4::Vec2f pos )
{
    impl_.setPosition( pos.x, pos.y );
}

dr4::Vec2f
dr4::impl::Text::GetPos() const
{
    const auto& sf_pos = impl_.getPosition();

    return { sf_pos.x, sf_pos.y };
}
