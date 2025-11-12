#pragma once

#include "dr4/font_impl.hpp"
#include <cmath>

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
    fprintf( stderr, "Sorry, %s unimplemented", __func__ );

    return 0.0f;
}

float
dr4::impl::Font::GetDescent( float fontSize ) const
{
    fprintf( stderr, "Sorry, %s unimplemented", __func__ );

    return 0.0f;
}
