cmake_minimum_required( VERSION 3.16 )
project( dr4hui-backend LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

option( BUILD_EXAMPLES "Build the examples" OFF )

include( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/funcs.cmake )

setup_constants()
setup_interface()
setup_plugins()
install_plugins()
install_core()

if ( BUILD_EXAMPLES )
    build_examples()
endif()
