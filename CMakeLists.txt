cmake_minimum_required( VERSION 3.16 )
project( gfx LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

file( GLOB_RECURSE GFX_SOURCES CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

find_package( PkgConfig REQUIRED )
pkg_check_modules( SFML REQUIRED IMPORTED_TARGET sfml-graphics sfml-window sfml-system )

add_library( gfx
             SHARED
             ${GFX_SOURCES} )

target_include_directories( gfx PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/mipt-ded-zemax/include
)

target_link_libraries(gfx PkgConfig::SFML)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
target_compile_options(gfx PRIVATE -O3)
install(TARGETS gfx DESTINATION install/lib)

file( GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp
)

add_executable( example examples/main.cpp )

target_include_directories( example PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/mipt-ded-zemax/include
)

# target_link_libraries( example dl )

install(TARGETS example)
