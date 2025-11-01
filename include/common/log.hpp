#pragma once

#include <iostream>

namespace gfx {
namespace common {

void
log( const std::string& msg )
{
    std::cerr << msg << std::endl;
}

} // namespace common
} // namespace gfx
