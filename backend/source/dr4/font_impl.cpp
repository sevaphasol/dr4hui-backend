#include "dr4/font_impl.hpp"
#include <cmath>
#include <iostream>

void
dr4::impl::Font::LoadFromFile( const std::string& path )
{
    impl_.loadFromFile( path );
}

void
dr4::impl::Font::LoadFromBuffer( const void* buffer, size_t size )
{
    impl_.loadFromMemory( buffer, size );
}

float
dr4::impl::Font::GetAscent( float fontSize ) const
{
    sf::Glyph ascent = impl_.getGlyph( 'M', fontSize, false );

    return -ascent.bounds.height;
}

float
dr4::impl::Font::GetDescent( float fontSize ) const
{
    sf::Glyph descent = impl_.getGlyph( 'p', fontSize, false );
    return descent.bounds.height + descent.bounds.top;
}
