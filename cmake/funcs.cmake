function( setup_constants )
	set( BACKEND_INCLUDE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/backend/include   PARENT_SCOPE )
	set( BACKEND_SOURCE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/backend/source    PARENT_SCOPE )
	set( EXAMPLES_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/examples          PARENT_SCOPE )
endfunction()

#==============================================================================

function( setup_interface )
	add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/interface )
endfunction()

#==============================================================================

function( setup_libdr4 )
	file( GLOB_RECURSE DR4_BACKEND_SOURCES CONFIGURE_DEPENDS
		${BACKEND_SOURCE_DIR}/dr4/*.cpp
	)
	add_library( dr4 SHARED ${DR4_BACKEND_SOURCES} )
	target_include_directories( dr4 PRIVATE ${BACKEND_INCLUDE_DIR} )

	find_package( PkgConfig REQUIRED )
	pkg_check_modules( SFML REQUIRED IMPORTED_TARGET
		sfml-graphics sfml-window sfml-system
	)
	target_link_libraries(dr4 PRIVATE dr4hui-interface PkgConfig::SFML)
endfunction()

#==============================================================================

function( setup_libpp )
	file( GLOB_RECURSE PP_BACKEND_SOURCES CONFIGURE_DEPENDS
		${BACKEND_SOURCE_DIR}/pp/**/*.cpp
		${BACKEND_SOURCE_DIR}/pp/*.cpp
	)
	add_library( pp SHARED ${PP_BACKEND_SOURCES} )
	target_include_directories( pp PRIVATE ${BACKEND_INCLUDE_DIR} )
	target_include_directories( pp PRIVATE ${PROJECT_SOURCE_DIR}/interface/include )

	target_link_libraries(dr4 PRIVATE dr4hui-interface )
endfunction()

#==============================================================================

function( setup_libs )
	setup_libdr4()
	setup_libpp()
endfunction()

#==============================================================================

function( install_libs )
	install( TARGETS dr4 DESTINATION lib )
endfunction()

#==============================================================================

function( build_examples )
	file( GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS
		${EXAMPLES_DIR}/*.cpp
	)
	add_executable( example ${EXAMPLE_SOURCES} )
	target_link_libraries( example dr4hui-interface )
	install( TARGETS example DESTINATION examples )
endfunction()
