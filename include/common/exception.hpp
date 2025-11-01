#pragma once

#include "log.hpp"

namespace gfx {
namespace common {

#define GFX_API_CATCH( dr4_suffix )                                                                \
    catch ( const const std::bad_alloc& e )                                                        \
    {                                                                                              \
        log( "bad_alloc in " + __FILE__ + ":" ++__PRETTY_FUNTION_ + ":" __LINE__ );                \
    }

} // namespace common
} // namespace gfx
